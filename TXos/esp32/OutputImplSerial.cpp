/*
  TXos. A remote control transmitter OS.

  MIT License

  Copyright (c) 2023 wlowi

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include "TXos.h"

#if HF_MODULE == HF_SPEKTRUM_SERIAL

#include "driver/gptimer.h"
#include "driver/uart.h"
#include "OutputImplSerial.h"
#include "Ports.h"

extern Ports ports;
extern OutputImplSerial* outputImpl;

// In 0.1 mSec units
#define OUT_INIT_mSec (110)
#define RESET_mSec (0)

#define UART_RX_BUFFER_SZ (256)
#define UART_TX_BUFFER_SZ (256)

/* Send Buffer
 * For Spektrum X1TX1 module supporting 6 channels
 * 2 header bytes + 6 * (2 bytes per channel)
 */
#define SEND_BUFFER_LEN (14)

const uint8_t BINDMODE_COUNT = 2;
static bindmode_t BINDMODES[BINDMODE_COUNT] = {
    BINDMODE_DSM2,
    BINDMODE_DSMX
};

static uint8_t sendBuffer[SEND_BUFFER_LEN];
const uart_port_t uart_num = UART_NUM_2;
static volatile bool inISR = false;
static volatile bindmode_t bindMode = BINDMODE_DSM2;

// Spektrum Header byte 0

#define DSMX         0x08
#define DSM2         0x00

#define EU_FREQ      0x10
#define FRANCE_FREQ  0x00

#define RANGE_TEST   0x20

#define US_EXT_RANGE 0x40

#define BIND_MODE    0x80


/* 10khz results in 0.1 msec resolution */
gptimer_handle_t gptimer = NULL;

static portMUX_TYPE outMux = portMUX_INITIALIZER_UNLOCKED;

static bool outTimerISR(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx)
{
    uint8_t ch;
    uint16_t v;

    inISR = true;

    portENTER_CRITICAL(&outMux);

    outputImpl->switchSet();

    if( bindMode == BINDMODE_DSMX) {
        sendBuffer[0] = DSMX | EU_FREQ;
    } else {
        sendBuffer[0] = EU_FREQ;
    }

    if( outputImpl->rangeTest) {
        sendBuffer[0] |= RANGE_TEST;
    }

    if( outputImpl->bind) {
        sendBuffer[0] |= BIND_MODE;
    }

    sendBuffer[1] = outputImpl->modelID;

    for( ch = 0; ch < 6; ch++) {
        v = map(outputImpl->ppmSet[outputImpl->currentSet].channel[ch],
                PPM_MIN_usec, PPM_MAX_usec, 0, 1023);
        sendBuffer[2 +2*ch] = (uint8_t)((ch << 2) | ((v >> 8) & 0x03));
        sendBuffer[3 +2*ch] = (uint8_t)(v & 0xff);
    }

    portEXIT_CRITICAL(&outMux);

    uart_write_bytes(uart_num, &sendBuffer[0], SEND_BUFFER_LEN);

    inISR = false;

    return false;
}

OutputImplSerial::OutputImplSerial() {

    init();
}

/* Initialization:
 * - Set all channels to mid.
 * - Switch PPM pin to output.
 * - Initialize timer
 *     Timer counts at a rate of 1Mhz ( 1 micro sec. )
 */
void OutputImplSerial::init() {

    for (channel_t i = 0; i < PPM_CHANNELS; i++) {
        ppmSet[0].channel[i] = PPM_MID_usec;
        ppmSet[1].channel[i] = PPM_MID_usec;
    }

    currentSet = 0;
    overrun = 0;
    channelSetDone = true;
    bind = false;
    rangeTest = false;


    // UART
    uart_driver_install( uart_num, UART_RX_BUFFER_SZ, UART_TX_BUFFER_SZ, 0, nullptr, 0);

    uart_config_t uart_config = {
        .baud_rate = 125000,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    uart_param_config( uart_num, &uart_config);
    uart_set_pin( uart_num,
        PORT_SERIAL_TX, PORT_SERIAL_RX,
        UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);


    // TIMER interrupt. 0.1 mSec
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 10000 // Resolution is 0.1 mSec
    };

    gptimer_new_timer( &timer_config, &gptimer);

    gptimer_alarm_config_t alarm_config;
    alarm_config.alarm_count = 100; // 10 mSec
    alarm_config.reload_count = 0;
    alarm_config.flags.auto_reload_on_alarm = true;

    gptimer_set_alarm_action( gptimer, &alarm_config);

    gptimer_event_callbacks_t cb_config = {
        .on_alarm = outTimerISR
    };

    gptimer_register_event_callbacks( gptimer, &cb_config, NULL);
    gptimer_enable( gptimer);
    gptimer_start( gptimer);
}

timingUsec_t OutputImplSerial::getMaxFrameTime() {

    return 0;
}

/* An overrun occurs if not all channels in the modifiable set
 * were set and a switch occurs.
 */
uint16_t OutputImplSerial::getOverrunCounter() {

    uint16_t c;

    portENTER_CRITICAL(&outMux);
    c = overrun;
    portEXIT_CRITICAL(&outMux);

    return c;
}

/* Switch active and modifiable set.
 * Increase the ppmOverrun counter if the channelSetDone flag has not
 * been set in time.
 */
void OutputImplSerial::switchSet() {

    if (channelSetDone) {
        channelSetDone = false;
        currentSet = OTHER_PPMSET(currentSet);
    }
    else {
        overrun++;
    }
}

void OutputImplSerial::isrDisable() {

    gptimer_stop( gptimer);

    while( inISR ) {
        // busy wait
    }
}

void OutputImplSerial::isrEnable() {

    gptimer_start( gptimer);
}

bool OutputImplSerial::acceptChannels() {

    return !channelSetDone;
}

/* Set timing for a channel.
 * The value must be between -PPM_RANGE_usec and PPM_RANGE_usec.
 * A value outside is clipped.
 *
 * Setting the last channel will set the channelSetDone flag.
 *
 * SO SET THE LAST CHANNEL LAST !!
 *
 */
void OutputImplSerial::SetChannelValue(int channel, int value) {

    timingUsec_t t;

    /* Convert:
     *
     * value
     * =======
     * CHANNELVALUE_MID   0
     * CHANNELVALUE_MIN   -1250 == -125%  == -500
     * CHANNELVALUE_MAX    1250 ==  125%  ==  500
     *
     * To:
     *
     * ppmTiming_t
     * ===========
     * PPM_MID_usec        ((timingUsec_t) 1500)
     * PPM_MIN_usec        (PPM_MID_usec - PPM_RANGE_usec) = 900
     * PPM_MAX_usec        (PPM_MID_usec + PPM_RANGE_usec) = 2100
     *
     */

    if (channel < PPM_CHANNELS) {

        t = PPM_MID_usec + (value * 2 / 5);

        if (t < PPM_MIN_usec) t = PPM_MIN_usec;
        if (t > PPM_MAX_usec) t = PPM_MAX_usec;

        portENTER_CRITICAL(&outMux);
        ppmSet[OTHER_PPMSET(currentSet)].channel[channel] = t;

        if (channel == PPM_CHANNELS - 1) {
            channelSetDone = true;
        }
        portEXIT_CRITICAL(&outMux);
    }
}

bool OutputImplSerial::isBindSupported() const {

    return true;
};

bool OutputImplSerial::isRangeTestSupported() const {

    return true;
}

uint8_t OutputImplSerial::getBindModeCount() const {

    return BINDMODE_COUNT;
}

bindmode_t* OutputImplSerial::getBindModes() const {

    return BINDMODES;
}

void OutputImplSerial::bindActivate( bindmode_t bm) {

    HFoff();
    bindMode = bm;
    bind = true;
    HFon();
}

void OutputImplSerial::bindDeactivate() {

    HFoff();
    bind = false;
    HFon();
}

void OutputImplSerial::rangeTestActivate() {

    HFoff();
    rangeTest = true;
    HFon();
}

void OutputImplSerial::rangeTestDeactivate() {

    HFoff();
    rangeTest = false;
    HFon();
}

void OutputImplSerial::setModelID( uint8_t mID) {

    modelID = mID;
}

uint8_t OutputImplSerial::getModelID() {

    return modelID;
}

void OutputImplSerial::setBindMode( bindmode_t bm) {

    bindMode = bm;
}

bindmode_t OutputImplSerial::getBindMode() {

    return bindMode;
}

void OutputImplSerial::HFoff() {

    ports.hfOff();
    delay(500);
}

void OutputImplSerial::HFon() {

    ports.hfOn();
}

#endif
