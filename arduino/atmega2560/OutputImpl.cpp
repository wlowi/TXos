
#include <util/atomic.h>

#include "OutputImpl.h"
#include "InputImpl.h"

extern OutputImpl *outputImpl;
extern InputImpl *inputImpl;

/* PPM generation state machine state.
 * Used in the interrupt routine.
 */
static volatile uint8_t outputState;

/* inFrameTime is used to compute the remaining time until frame end */
static volatile timingUsec_t inFrameTime_usec;
static volatile timingUsec_t maxFrameTime_usec;

static volatile channel_t outputChannel;

/* Convert timer low, high values to microseconds.
 * Division by 2 is because the timer runs in 0.5 usec resolution.
 */
#define CONVERT_TIMER_TO_USEC( l, h)  ((((timingUsec_t)h << 8) + l) >> 1)

/* PPM generation state machine states */
#define BEGIN_OF_FIRST_SPACE    0
#define END_OF_SPACE            1
#define END_OF_MARK             2
#define END_OF_LAST_SPACE       4

#define PIN_LOW()   digitalWrite( PPM_PORT, LOW)
#define PIN_HIGH()  digitalWrite( PPM_PORT, HIGH)

ISR( TIMER1_COMPA_vect) {
    
    timingUsec_t nextTimerValue_usec;
    uint8_t h, l;
    
    l = TCNT1L;
    h = TCNT1H;

    /* Resetting the counter is the first thing to do to guarantee correct timing. */
    TCNT1H = (byte)0;
    TCNT1L = (byte)0;
    
    inFrameTime_usec += CONVERT_TIMER_TO_USEC( l, h);

    switch( outputState) {
    case BEGIN_OF_FIRST_SPACE:
        PIN_LOW();
        nextTimerValue_usec = PPM_SPACE_usec;

        outputImpl->switchSet();
        inputImpl->start();

        if( inFrameTime_usec > maxFrameTime_usec) {
          maxFrameTime_usec = inFrameTime_usec;
        }

        inFrameTime_usec = 0;
        outputState = END_OF_SPACE;
        break;
        
    case END_OF_SPACE:
        PIN_HIGH();
        nextTimerValue_usec = outputImpl->ppmSet[outputImpl->currentSet].channel[outputChannel];

        outputState = END_OF_MARK;
        break;

    case END_OF_MARK:
        PIN_LOW();
        nextTimerValue_usec = PPM_SPACE_usec;
        
        outputChannel++;
        if( outputChannel < PPM_CHANNELS)  {
            outputState = END_OF_SPACE; // repeat for next channel
        } else {
            outputChannel = 0;
            outputState = END_OF_LAST_SPACE; // done
        }
        break;
        
    case END_OF_LAST_SPACE:
        PIN_HIGH();
        nextTimerValue_usec = PPM_FRAME_usec - inFrameTime_usec;

        outputState = BEGIN_OF_FIRST_SPACE;
        break;
        
    default:
        nextTimerValue_usec = 0;
    }
    
    /* Multiply by 2 because the timer resulution is 0.5 micro sec. */
    nextTimerValue_usec <<= 1;

    /* Set output compare register. HIGH BYTE FIRST ! */
    OCR1AH = H( nextTimerValue_usec);
    OCR1AL = L( nextTimerValue_usec);
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

        inFrameTime_usec = 0;
        maxFrameTime_usec = 0;
        outputChannel = 0;
        outputState = BEGIN_OF_FIRST_SPACE;
        ppmOverrun = 0;
        channelSetDone = true;

        /* Set port line to output */
        pinMode( PPM_PORT, OUTPUT);

        /* Don't use output compare pins */
        TCCR1A = (byte)0;
        /* Prescaler /8 = 2Mhz = 0.5 usec */
        TCCR1B = _BV(CS11);
        /* Set initial timer counter value */
        TCNT1H = (byte)0;
        TCNT1L = (byte)0;
        /* Set output compare register. HIGH BYTE FIRST ! */
        OCR1AH = H( PPM_INIT_usec);
        OCR1AL = L( PPM_INIT_usec);
        /* Enable output compare match A interrupt */
        TIMSK1 = _BV(OCIE1A);
    }
}

/* Current in frame time, in usec.
 */
timingUsec_t OutputImpl::getInFrameTime() {
    
    timingUsec_t t;
    uint8_t l, h;

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {
        t = inFrameTime_usec;
        l = TCNT1L; // Read low byte first!
        h = TCNT1H;
    }
    
    return t + CONVERT_TIMER_TO_USEC( l, h);
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
    
    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {

        if( channelSetDone) {
            channelSetDone = false; 
            currentSet = OTHER_PPMSET( currentSet);

        } else {
            ppmOverrun++;
        }
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
