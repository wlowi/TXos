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

#include "OutputImpl.h"
#include "InputImpl.h"

extern OutputImpl* outputImpl;
extern InputImpl* inputImpl;

/* PPM generation state machine state.
 * Used in the interrupt routine.
 */
static uint8_t outputState;

/* PPM generation state machine states */
#define BEGIN_OF_FIRST_SPACE    0
#define END_OF_SPACE            1
#define END_OF_MARK             2
#define END_OF_LAST_SPACE       4

static timingUsec_t lastChStart_uSec;
static timingUsec_t maxFrameTime_uSec;
static channel_t outputChannel;

/* 1Mhz results in 1 usec resolution */
static hw_timer_t *ppmTimer = NULL;

/*
#define PIN_LOW()  digitalWrite( PPM_PORT, false)
#define PIN_HIGH() digitalWrite( PPM_PORT, true)
*/
#define PIN_LOW()  gpio_set_level( (gpio_num_t)PPM_PORT, 0)
#define PIN_HIGH() gpio_set_level( (gpio_num_t)PPM_PORT, 1)

/* This compensates time from begin of ISR to actual timer reset */
#define RESET_ADJUST 6

/* This compensates ISR service time */
#define ISR_ADJUST   3

portMUX_TYPE ppmMux = portMUX_INITIALIZER_UNLOCKED;

void ARDUINO_ISR_ATTR ppmTimerISR() {

    timingUsec_t inFrameTime_uSec;
    timingUsec_t nextTimerValue_uSec;

    inFrameTime_uSec = (timingUsec_t)timerRead( ppmTimer);

    portENTER_CRITICAL(&ppmMux);

    switch (outputState) {
    case END_OF_SPACE:
        PIN_HIGH();
        nextTimerValue_uSec = lastChStart_uSec + outputImpl->ppmSet[outputImpl->currentSet].channel[outputChannel];
        outputState = END_OF_MARK;
        break;

    case END_OF_MARK:
        PIN_LOW();
        nextTimerValue_uSec = inFrameTime_uSec + PPM_SPACE_usec;
        lastChStart_uSec = inFrameTime_uSec;

        outputChannel++;
        if (outputChannel < PPM_CHANNELS) {
            outputState = END_OF_SPACE; // repeat for next channel
        }
        else {
            outputImpl->switchSet();
            outputChannel = 0;
            outputState = END_OF_LAST_SPACE; // done
        }
        break;

    case BEGIN_OF_FIRST_SPACE:
        PIN_LOW();
        timerWrite( ppmTimer, RESET_ADJUST);

        if( inFrameTime_uSec > maxFrameTime_uSec) {
            maxFrameTime_uSec = inFrameTime_uSec;
        }

        nextTimerValue_uSec = PPM_SPACE_usec;
        lastChStart_uSec = 0;
        outputState = END_OF_SPACE;
        break;

    case END_OF_LAST_SPACE:
        PIN_HIGH();
        nextTimerValue_uSec = PPM_FRAME_usec;
        outputState = BEGIN_OF_FIRST_SPACE;
        break;

    default:
        PIN_LOW();
        timerWrite( ppmTimer, 0);
        nextTimerValue_uSec = PPM_INIT_usec;
        outputState = BEGIN_OF_FIRST_SPACE;
    }

    portEXIT_CRITICAL(&ppmMux);

    nextTimerValue_uSec -= ISR_ADJUST;

    timerAlarmWrite( ppmTimer, nextTimerValue_uSec, false);
}

OutputImpl::OutputImpl() {

    init();
}

/* Initialization:
 * - Set all channels to mid.
 * - Switch PPM pin to output.
 * - Initialize timer
 *     Timer counts at a rate of 1Mhz ( 1 micro sec. )
 */
void OutputImpl::init() {

    for (channel_t i = 0; i < PPM_CHANNELS; i++) {
        ppmSet[0].channel[i] = PPM_MID_usec;
        ppmSet[1].channel[i] = PPM_MID_usec;
    }

    currentSet = 0;
    ppmOverrun = 0;
    channelSetDone = true;

    outputChannel = 0;
    maxFrameTime_uSec = 0;
    outputState = BEGIN_OF_FIRST_SPACE;

    pinMode(PPM_PORT, OUTPUT);
    PIN_HIGH();

    ppmTimer = timerBegin( 0, 80, true);

    if( ppmTimer)
    {
        timerAttachInterrupt( ppmTimer, &ppmTimerISR, false);
        timerAlarmWrite( ppmTimer, PPM_INIT_usec, false);
        timerAlarmEnable( ppmTimer);
    }
}

timingUsec_t OutputImpl::getMaxFrameTime() {

    timingUsec_t t;

    portENTER_CRITICAL(&ppmMux);
    t = maxFrameTime_uSec;
    portEXIT_CRITICAL(&ppmMux);

    return t;
}


/* An overrun occurs if not all channels in the modifiable set
 * were set and a switch occurs.
 */
uint16_t OutputImpl::getOverrunCounter() {

    uint16_t c;

    portENTER_CRITICAL(&ppmMux);
    c = ppmOverrun;
    portEXIT_CRITICAL(&ppmMux);

    return c;
}

/* Switch active and modifiable set.
 * Increase the ppmOverrun counter if the channelSetDone flag has not
 * been set in time.
 */
void OutputImpl::switchSet() {

    if (channelSetDone) {
        channelSetDone = false;
        currentSet = OTHER_PPMSET(currentSet);
    }
    else {
        ppmOverrun++;
    }
}

bool OutputImpl::acceptChannels() {

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
void OutputImpl::SetChannelValue(int channel, int value) {

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

        portENTER_CRITICAL(&ppmMux);
        ppmSet[OTHER_PPMSET(currentSet)].channel[channel] = t;

        if (channel == PPM_CHANNELS - 1) {
            channelSetDone = true;
        }
        portEXIT_CRITICAL(&ppmMux);
    }
}
