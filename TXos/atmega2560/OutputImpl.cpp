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

#include <util/atomic.h>

#include "OutputImpl.h"
#include "InputImpl.h"

#include "Ports.h"

extern Ports ports;

extern OutputImpl *outputImpl;
extern InputImpl *inputImpl;

static channel_t outputChannel = 0;

/* inFrameTime is used to compute the remaining time until frame end.
 * Unit is 0.5 usec because timer is running at 2 Mhz.
 */
static volatile timingUsec_t inFrameTime_half_uSec = 0;
static volatile timingUsec_t maxFrameTime_half_uSec = 0;

ISR(TIMER3_OVF_vect) {

  timingUsec_t nextTimerTop;

  /* Output compare register is set to trigger at space end which is 400 usec.
   * The pin will be set to high at output compare match and
   * reset at TOP.
   */
  if( outputChannel >= PPM_CHANNELS) {

    outputImpl->switchSet();
    inputImpl->start();

    /* Fill gap until end of frame time (22msec) */
    nextTimerTop = (PPM_FRAME_usec << 1) - inFrameTime_half_uSec;
    inFrameTime_half_uSec += nextTimerTop;

    if( inFrameTime_half_uSec > maxFrameTime_half_uSec) {
        maxFrameTime_half_uSec = inFrameTime_half_uSec;
    }

    inFrameTime_half_uSec = 0;
    outputChannel = 0;

  } else {

    nextTimerTop = (outputImpl->ppmSet[outputImpl->currentSet].channel[outputChannel]) << 1;
    inFrameTime_half_uSec += nextTimerTop;
    outputChannel++;
  }

  /* We nedd to count 1498..1499..0..1
   * not 1498..1499..1500..0..1
   */
  nextTimerTop--;
  ICR3 = nextTimerTop;
}

OutputImpl::OutputImpl() {

    init();
}

/* Initialization:
 * - Set all channels to mid.
 * - Switch PPM pin to output.
 * - Initialize timer
 *     Timer 3 counts at a rate of 2Mhz ( 0.5 micro sec. )
 */
void OutputImpl::init() {

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {

        for( channel_t i = 0; i < PPM_CHANNELS; i++) {
            ppmSet[0].channel[i] = PPM_MID_usec;
            ppmSet[1].channel[i] = PPM_MID_usec;
        }

        maxFrameTime_half_uSec = 0;
        ppmOverrun = 0;
        channelSetDone = true;

        outputChannel = 0;
        inFrameTime_half_uSec = 0;

        TCCR3A = (byte)0;
        TCCR3B = (byte)0;
        TCCR3C = (byte)0;

        pinMode( PPM_PORT, OUTPUT);

        /* Enable timer in power reduction register */
        PRR1 &= ~bit(PRTIM3);

        /* Set initial timer counter value */
        TCNT3 = 0;

        /* COM3A1, COM3A0     :  Set OC3A on compare match, clear at TOP
         * WGM33, WGM32, WGM31: TOP is ICR3
         */
        TCCR3A = bit(COM3A1) | bit(COM3A0) | bit(WGM31);

        /* CS31:  Prescaler /8 = 2Mhz = 0.5 usec */
        TCCR3B = bit(WGM33) | bit(WGM32) | bit(CS31);

        TCCR3C = (byte)0;

        /* Set TOP */
        ICR3 = ((PPM_SPACE_usec + PPM_INIT_usec) << 1) -1;

        /* Set compare to end of pulse
         * Subtract 1 because we count 798..799..0..1
         */
        OCR3A = (PPM_SPACE_usec << 1) -1;

        /* TOIE3: Timer overflow interrupt */
        TIMSK3 = bit(TOIE3);
    }
}

timingUsec_t OutputImpl::getMaxFrameTime() {

    timingUsec_t t;

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {
        t = maxFrameTime_half_uSec / 2;
    }

    return t;
}


/* An overrun occurs if not all channels in the modifiable set
 * were set and a switch occurs.
 */
uint16_t OutputImpl::getOverrunCounter() {

    return ppmOverrun;
}

/* Switch active and modifiable set.
 * Increate the ppmOverrun counter if the channelSetDone flag has not
 * been set.
 */
void OutputImpl::switchSet() {

    if( channelSetDone) {
        channelSetDone = false;
        currentSet = OTHER_PPMSET( currentSet);

    } else {
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

    if( channel < PPM_CHANNELS) {

        t = PPM_MID_usec + (value * 2 / 5);

        if( t < PPM_MIN_usec) t = PPM_MIN_usec;
        if( t > PPM_MAX_usec) t = PPM_MAX_usec;

        ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {

            ppmSet[ OTHER_PPMSET( currentSet) ].channel[ channel ] = t;

            if( channel == PPM_CHANNELS-1) {
                channelSetDone = true;
            }
        }
    }
}

bool OutputImpl::isBindSupported() const {

    return true;
};

bool OutputImpl::isRangeTestSupported() const {

    return true;
}

void OutputImpl::bindActivate() {

    ports.hfOff();
    delay( 500);
    ports.bindOn();
    delay( 100);
    ports.hfOn();
}

void OutputImpl::bindDeactivate() {

    ports.hfOff();
    delay( 500);
    ports.bindOff();
    delay( 100);
    ports.hfOn();
}

void OutputImpl::rangeTestActivate() {

    ports.bindOn();
}

void OutputImpl::rangeTestDeactivate() {

    ports.bindOff();
}
