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

#if HF_MODULE == HF_SPEKTRUM_SERIAL || HF_MODULE == HF_CRSF_SERIAL

#include "Bind.h"
#include "EngineCut.h"
#include "ModuleManager.h"
#include "OutputImplSerial.h"
#include "Ports.h"
#include "Telemetry.h"
#include "driver/gptimer.h"
#include "driver/uart.h"

extern ModuleManager moduleManager;
extern Ports ports;
extern OutputImplSerial *outputImpl;
extern Telemetry telemetry;

// In 0.1 mSec units
#define OUT_INIT_mSec (110)
#define RESET_mSec (0)

#define UART_RX_BUFFER_SZ (256)
#define UART_TX_BUFFER_SZ (256)

/* Send Buffer
 * For Spektrum X1TX1 module supporting 6 channels
 * 2 header bytes + 6 * (2 bytes per channel)
 *
 * For CRSF it is 64 bytes
 */

/*********************** CRSF ***********************************************/

#if HF_MODULE == HF_CRSF_SERIAL
#define SEND_BUFFER_LEN (64)
const uint8_t BINDMODE_COUNT = 2;
static bindmode_t BINDMODES[BINDMODE_COUNT] = {BINDMODE_CRSF, BINDMODE_CRS4};
static volatile bindmode_t bindMode = BINDMODE_CRSF;

typedef struct __attribute__((packed)) crsfChannelsPacked_t {
    uint16_t ch01 : 11;
    uint16_t ch02 : 11;
    uint16_t ch03 : 11;
    uint16_t ch04 : 11;
    uint16_t ch05 : 11;
    uint16_t ch06 : 11;
    uint16_t ch07 : 11;
    uint16_t ch08 : 11;
    uint16_t ch09 : 11;
    uint16_t ch10 : 11;
    uint16_t ch11 : 11;
    uint16_t ch12 : 11;
    uint16_t ch13 : 11;
    uint16_t ch14 : 11;
    uint16_t ch15 : 11;
    uint16_t ch16 : 11;
} crsfChannelsPacked_t;

/*********************** Spektrum *******************************************/

#elif HF_MODULE == HF_SPEKTRUM_SERIAL
#define SEND_BUFFER_LEN (14)
const uint8_t BINDMODE_COUNT = 2;
static bindmode_t BINDMODES[BINDMODE_COUNT] = {BINDMODE_DSM2, BINDMODE_DSMX};
static volatile bindmode_t bindMode = BINDMODE_DSM2;

/*********************** END ************************************************/

#else
#error "Define HF_CRSF_SERIAL or HF_SPEKTRUM_SERIAL"
#endif

static uint8_t sendBuffer[SEND_BUFFER_LEN];
const uart_port_t uart_num = UART_NUM_2;
static volatile bool inISR = false;

#if HF_MODULE == HF_CRSF_SERIAL
/*
 * All packets are in the CRSF format [sync] [len] [type] [payload] [crc8]
 * with a maximum total size of 64 bytes.
 */

// Device addresses
#define CRSF_BROADCAST_ADDRESS 0x00
#define CRSF_SYNC 0xC8
#define CRSF_RADIO_ADDRESS 0xEA
#define CRSF_MODULE_ADDRESS 0xEE
#define CRSF_RECEIVER_ADDRESS 0xEC

#define CRSF_FRAMETYPE_RC_CHANNELS_PACKED 0x16

#elif HF_MODULE == HF_SPEKTRUM_SERIAL
// Spektrum Header byte 0

#define DSMX 0x08
#define DSM2 0x00

#define EU_FREQ 0x10
#define FRANCE_FREQ 0x00

#define RANGE_TEST 0x20

#define US_EXT_RANGE 0x40

#define BIND_MODE 0x80

#endif

/* 10khz results in 0.1 msec resolution */
gptimer_handle_t gptimer = NULL;

static portMUX_TYPE outMux = portMUX_INITIALIZER_UNLOCKED;

/*********************** CRSF ***********************************************/

#if HF_MODULE == HF_CRSF_SERIAL

/**
 * @brief CRSF crc8 checksum
 *
 * Checksum includes type and payload
 *
 * @param buffer Pointer to buffer start
 * @param start Starting index in buffer
 * @param end Ending index if buffer. Ending index is excluded"
 * @param poly Initializing Polynome
 * @return crc8 checksum
 */
static uint8_t crc8(uint8_t *buffer, uint8_t start, uint8_t end, uint8_t poly) {

    uint8_t crc = 0;

    for (uint8_t i = start; i < end; i++) {
        crc ^= buffer[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ poly;
            } else {
                crc <<= 1;
            }
        }
    }

    return crc;
}

static size_t buildCRSFPackage() {

    size_t filled = 0;
    crsfChannelsPacked_t *overlay;

    EngineCut *engineCut = (EngineCut *)moduleManager.getModuleByType(
        MODULE_SET_MODEL, MODULE_ENGINE_CUT_TYPE);

    /* It is possible that engineCut module is not yet initialized. */
    bool armed = (engineCut != nullptr) ? !engineCut->isSave() : false;

    // [sync]
    sendBuffer[filled++] = CRSF_MODULE_ADDRESS;

    // [len]
    sendBuffer[filled++] =
        (bindMode == BINDMODE_CRS4)
            ? 3 + sizeof(crsfChannelsPacked_t) // type + payload + arming + crc8
            : 2 + sizeof(crsfChannelsPacked_t); // type + payload + crc8

    // [type]
    sendBuffer[filled++] = CRSF_FRAMETYPE_RC_CHANNELS_PACKED;

    // [payload]
    overlay = (crsfChannelsPacked_t *)&sendBuffer[filled];
    filled += sizeof(crsfChannelsPacked_t);

#define US2CRSF(u) 992 + (((int)(u)-1500) * 8 / 5)

    overlay->ch01 =
        US2CRSF(outputImpl->ppmSet[outputImpl->currentSet].channel[0]);
    overlay->ch02 =
        US2CRSF(outputImpl->ppmSet[outputImpl->currentSet].channel[1]);
    overlay->ch03 =
        US2CRSF(outputImpl->ppmSet[outputImpl->currentSet].channel[2]);
    overlay->ch04 =
        US2CRSF(outputImpl->ppmSet[outputImpl->currentSet].channel[3]);

    /* No arming on ch 5 for CRSF V4 */
    if (bindMode == BINDMODE_CRS4) {
        overlay->ch05 =
            US2CRSF(outputImpl->ppmSet[outputImpl->currentSet].channel[4]);
    } else {
        overlay->ch05 = US2CRSF(armed ? 2000 : 1000);
    }

    overlay->ch06 =
        US2CRSF(outputImpl->ppmSet[outputImpl->currentSet].channel[5]);
    overlay->ch07 =
        US2CRSF(outputImpl->ppmSet[outputImpl->currentSet].channel[6]);
    overlay->ch08 =
        US2CRSF(outputImpl->ppmSet[outputImpl->currentSet].channel[7]);
    overlay->ch09 =
        US2CRSF(outputImpl->ppmSet[outputImpl->currentSet].channel[8]);
    overlay->ch10 =
        US2CRSF(outputImpl->ppmSet[outputImpl->currentSet].channel[9]);
    overlay->ch11 =
        US2CRSF(outputImpl->ppmSet[outputImpl->currentSet].channel[10]);
    overlay->ch12 =
        US2CRSF(outputImpl->ppmSet[outputImpl->currentSet].channel[11]);

    /* Remapped channel 5 for CRSF < V4 */
    if (bindMode == BINDMODE_CRS4) {
        overlay->ch13 = US2CRSF(1500);
    } else {
        overlay->ch13 =
            US2CRSF(outputImpl->ppmSet[outputImpl->currentSet].channel[4]);
    }

    overlay->ch14 = US2CRSF(1500);
    overlay->ch15 = US2CRSF(1500);
    overlay->ch16 = US2CRSF(1500);

    // [arming] if ELRS V4
    if (bindMode == BINDMODE_CRS4) {
        sendBuffer[filled++] = armed;
    }

    // [crc8]
    sendBuffer[filled] = crc8(&sendBuffer[0], 2, filled, 0xD5);
    filled++;

    return filled;
}

void OutputImplSerial::processTelemetry() {

    /*
        int uart_read_bytes(uart_port_t uart_num, uint8_t *buf, uint32_t length,
       TickType_t ticks_to_wait)

        UART read bytes from UART buffer.

        Return
                (-1) Error
                OTHERS (>=0) The number of bytes read from UART FIFO

        Parameters
                uart_num: UART_NUM_0, UART_NUM_1 or UART_NUM_2
                buf: pointer to the buffer.
                length: data length
                ticks_to_wait: sTimeout, count in RTOS ticks
    */

    size_t len;
    uint8_t idx = 0;
    uint8_t ch;
    bool processed;
    uint8_t crc;
    uint8_t crc2;

#define STATE_WAIT_FOR_SYNC 0
#define STATE_READ_LEN 1
#define STATE_READ_TYPE 2
#define STATE_READ_PAYLOAD 3
#define STATE_READ_CRC 4
    static uint8_t state = STATE_WAIT_FOR_SYNC;
    static uint8_t crsf_sync;
    static uint8_t crsf_len;
    static uint8_t payload_len;
    static uint8_t crsf_type;
    static uint8_t payload[SEND_BUFFER_LEN];
    static uint8_t payload_idx;

    // sync len type payload crc
    uart_get_buffered_data_len(uart_num, &len);

    if (len) {
        len = (SEND_BUFFER_LEN < len) ? SEND_BUFFER_LEN : len;
        uart_read_bytes(uart_num, sendBuffer, len, 1);

        while (len--) {
            ch = sendBuffer[idx++];
            processed = false;

            while (!processed) {

                switch (state) {
                case STATE_WAIT_FOR_SYNC:
                    if (ch == CRSF_SYNC || ch == CRSF_RADIO_ADDRESS) {
                        crsf_sync = ch;
                        state = STATE_READ_LEN;
                    }
                    processed = true;
                    break;

                case STATE_READ_LEN:
                    crsf_len = ch;
                    if (crsf_len >= 2 && crsf_len <= 62) {
                        payload_len = crsf_len - 2;
                        state = STATE_READ_TYPE;
                        processed = true;
                    } else {
                        state = STATE_WAIT_FOR_SYNC;
                    }
                    break;

                case STATE_READ_TYPE:
                    crsf_type = ch;
                    payload_idx = 0;
                    payload[payload_idx++] = ch;
                    state = STATE_READ_PAYLOAD;
                    processed = true;
                    break;

                case STATE_READ_PAYLOAD:
                    if (payload_len) {
                        payload[payload_idx++] = ch;
                        payload_len--;
                        processed = true;
                    } else {
                        state = STATE_READ_CRC;
                    }
                    break;

                case STATE_READ_CRC:
                    crc = ch;
                    crc2 = crc8(&payload[0], 0, crsf_len - 1, 0xD5);
                    if (crc == crc2) {
                        telemetry.parseTelemetryFrame(crsf_type, &payload[0]);
                    }

                    state = STATE_WAIT_FOR_SYNC;
                    processed = true;
                    break;
                }
            }
        }
    }
}

/*********************** Spektrum *******************************************/

#elif HF_MODULE == HF_SPEKTRUM_SERIAL
static size_t buildSpektrumPackage() {

    uint8_t ch;
    uint16_t v;

    if (bindMode == BINDMODE_DSMX) {
        sendBuffer[0] = DSMX | EU_FREQ;
    } else {
        sendBuffer[0] = EU_FREQ;
    }

    if (outputImpl->rangeTest) {
        sendBuffer[0] |= RANGE_TEST;
    }

    if (outputImpl->bind) {
        sendBuffer[0] |= BIND_MODE;
    }

    sendBuffer[1] = outputImpl->modelID;

    for (ch = 0; ch < 6; ch++) {
        v = map(outputImpl->ppmSet[outputImpl->currentSet].channel[ch],
                PPM_MIN_usec, PPM_MAX_usec, 0, 1023);
        sendBuffer[2 + 2 * ch] = (uint8_t)((ch << 2) | ((v >> 8) & 0x03));
        sendBuffer[3 + 2 * ch] = (uint8_t)(v & 0xff);
    }

    return SEND_BUFFER_LEN;
}

void OutputImplSerial::processTelemetry() {}

/*********************** END ************************************************/

#endif

static bool outTimerISR(gptimer_handle_t timer,
                        const gptimer_alarm_event_data_t *edata,
                        void *user_ctx) {
    size_t filled;

    inISR = true;

    portENTER_CRITICAL(&outMux);

    outputImpl->switchSet();

#if HF_MODULE == HF_CRSF_SERIAL
    filled = buildCRSFPackage();
#elif HF_MODULE == HF_SPEKTRUM_SERIAL
    filled = buildSpektrumPackage();
#endif

    /*
      Serial.printf("\nl=%d\n", filled);
      for (uint8_t i = 0; i < filled; i++) {
        Serial.printf("%02x ", sendBuffer[i]);
      }
      Serial.println();
    */

    portEXIT_CRITICAL(&outMux);

    uart_write_bytes(uart_num, &sendBuffer[0], filled);

    outputImpl->processTelemetry();

    inISR = false;

    return false;
}

OutputImplSerial::OutputImplSerial() { init(); }

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
    uart_driver_install(uart_num, UART_RX_BUFFER_SZ, UART_TX_BUFFER_SZ, 0,
                        nullptr, 0);

#if HF_MODULE == HF_CRSF_SERIAL
    uart_config_t uart_config = {
        .baud_rate = 400000,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

#elif HF_MODULE == HF_SPEKTRUM_SERIAL
    uart_config_t uart_config = {
        .baud_rate = 125000,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
#endif

    uart_param_config(uart_num, &uart_config);
    uart_set_pin(uart_num, PORT_SERIAL_TX, PORT_SERIAL_RX, UART_PIN_NO_CHANGE,
                 UART_PIN_NO_CHANGE);

    // TIMER interrupt. 0.1 mSec
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 10000 // Resolution is 0.1 mSec
    };

    gptimer_new_timer(&timer_config, &gptimer);

    gptimer_alarm_config_t alarm_config;
    alarm_config.alarm_count = 100; // 10 mSec
    alarm_config.reload_count = 0;
    alarm_config.flags.auto_reload_on_alarm = true;

    gptimer_set_alarm_action(gptimer, &alarm_config);

    gptimer_event_callbacks_t cb_config = {.on_alarm = outTimerISR};

    gptimer_register_event_callbacks(gptimer, &cb_config, NULL);
    gptimer_enable(gptimer);
    gptimer_start(gptimer);
}

timingUsec_t OutputImplSerial::getMaxFrameTime() { return 0; }

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
    } else {
        overrun++;
    }
}

void OutputImplSerial::isrDisable() {

    gptimer_stop(gptimer);

    while (inISR) {
        // busy wait
    }
}

void OutputImplSerial::isrEnable() { gptimer_start(gptimer); }

bool OutputImplSerial::acceptChannels() { return !channelSetDone; }

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

        if (t < PPM_MIN_usec)
            t = PPM_MIN_usec;
        if (t > PPM_MAX_usec)
            t = PPM_MAX_usec;

        portENTER_CRITICAL(&outMux);
        ppmSet[OTHER_PPMSET(currentSet)].channel[channel] = t;

        if (channel == PPM_CHANNELS - 1) {
            channelSetDone = true;
        }
        portEXIT_CRITICAL(&outMux);
    }
}

bool OutputImplSerial::isBindSupported() const { return true; };

bool OutputImplSerial::isRangeTestSupported() const { return true; }

uint8_t OutputImplSerial::getBindModeCount() const { return BINDMODE_COUNT; }

bindmode_t *OutputImplSerial::getBindModes() const { return BINDMODES; }

void OutputImplSerial::bindActivate(bindmode_t bm) {

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

void OutputImplSerial::setModelID(uint8_t mID) { modelID = mID; }

uint8_t OutputImplSerial::getModelID() { return modelID; }

void OutputImplSerial::setBindMode(bindmode_t bm) { bindMode = bm; }

bindmode_t OutputImplSerial::getBindMode() { return bindMode; }

void OutputImplSerial::HFoff() {

    ports.hfOff();
    delay(500);
}

void OutputImplSerial::HFon() { ports.hfOn(); }

#endif
