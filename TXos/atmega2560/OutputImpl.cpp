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

extern OutputImpl *outputImpl;
extern InputImpl *inputImpl;

static channel_t outputChannel;

/* inFrameTime is used to compute the remaining time until frame end */
static volatile timingUsec_t inFrameTime;
static volatile timingUsec_t maxFrameTime_usec;

ISR(TIMER3_OVF_vect) {

  timingUsec_t nextTimerValue;
  
  if( outputChannel >= PPM_CHANNELS) {
    
    nextTimerValue = (PPM_FRAME_usec << 1) - inFrameTime;

    outputImpl->switchSet();
    inputImpl->start();

    inFrameTime = 0;
    outputChannel = 0;
    
  } else {
    
    nextTimerValue = (PPM_SPACE_usec + outputImpl->ppmSet[outputImpl->currentSet].channel[outputChannel]) << 1;
    inFrameTime += nextTimerValue;
    outputChannel++;
  }
  
  /* We nedd to count 1498..1499..0..1
   * not 1498..1499..1500..0..1
   * Subtract -2 because of 0.5 usec timer resolution.
   */
  nextTimerValue -= 2;
  
  ICR3H = H(nextTimerValue);
  ICR3L = L(nextTimerValue);
}

OutputImpl::OutputImpl() {
    
    init();
}

/* Initialization:
 * - Set all channels to mid.
 * - Switch PPM pin to output.
 * - Initialize timer
 *     Timer 1 counts at a rate of 2Mhz ( 0.5 micro sec. )
 */
void OutputImpl::init() {

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {

        for( channel_t i = 0; i < PPM_CHANNELS; i++) {
            ppmSet[0].channel[i] = PPM_MID_usec;
            ppmSet[1].channel[i] = PPM_MID_usec;
        }

        maxFrameTime_usec = 0;
        ppmOverrun = 0;
        channelSetDone = true;

        outputChannel = 0;
        inFrameTime = 0;
    
        pinMode( PPM_PORT, OUTPUT);
    
        /* Enable timer in power reduction register */
        PRR1 &= ~_BV(PRTIM3);
        TCCR3B = (byte)0;
    
        /* Set initial timer counter value */
        TCNT3H = (byte)0;
        TCNT3L = (byte)0;
       
        /* COM3A1, COM3A0     :  Set OC3A on compare match, clear at TOP
         * WGM33, WGM32, WGM31: TOP is ICR3
         */
        TCCR3A = _BV(COM3A1) | _BV(COM3A0) | _BV(WGM31);

        /* CS31:  Prescaler /8 = 2Mhz = 0.5 usec */
        TCCR3B = _BV(WGM33) | _BV(WGM32) | _BV(CS31);

        TCCR3C = (byte)0;

        /* Set TOP */
        ICR3H = H((PPM_SPACE_usec + PPM_INIT_usec) << 1);
        ICR3L = L((PPM_SPACE_usec + PPM_INIT_usec) << 1);

        /* Set compare to end of pulse
         *  
         * Subtract 2 because output is set to the next falling edge after compare.
         */
        OCR3AH = H((PPM_SPACE_usec << 1) -2);
        OCR3AL = L((PPM_SPACE_usec << 1) -2);
    
        /* TOIE3: Timer overflow interrupt */
        TIMSK3 = _BV(TOIE3);
    }
}

timingUsec_t OutputImpl::getMaxFrameTime() {
    
    timingUsec_t t;

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {
        t = maxFrameTime_usec;
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
     * PPM_MID_usec        ((timingUsec_t) 1200)
     * PPM_MIN_usec        (PPM_MID_usec - PPM_RANGE_usec)
     * PPM_MAX_usec        (PPM_MID_usec + PPM_RANGE_usec)
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
