/*
 * PPM.cpp
 * 
 * Generate a PPM signal.
 * This modules uses 16 bit Timer 1 to generate a PPM compatible 
 * Signal.
 * 
 * For configuration see the header file PPM.h.
 * 
 * History:
 * 
 *   wolfix   31.Dec.2019  Creation
 *   
 */

#include <util/atomic.h>

#include "AnalogInput.h"
#include "PPM.h"

#define PIN_LOW() PORTD &= ~_BV( PPM_PORT)
#define PIN_HIGH() PORTD |= _BV( PPM_PORT)

/* Convert timer low, high values to microseconds.
 * Division by 2 is because the timer runs in 0.5 usec resolution.
 */
#define CONVERT_TIMER_TO_USEC( l, h)  ((((ppmTiming_t)h << 8) + l) >> 1)

/* PPM generation state machine states */
#define BEGIN_OF_FIRST_SPACE    0
#define END_OF_SPACE            1
#define END_OF_MARK             2
#define END_OF_LAST_SPACE       4

PPM ppm;

ISR( TIMER1_COMPA_vect) {
    
    ppmTiming_t nextTimerValue_usec;
    uint8_t h, l;
    
    l = TCNT1L;
    h = TCNT1H;

    /* Resetting the counter is the first thing to do to guarantee correct timing. */
    TCNT1H = (byte)0;
    TCNT1L = (byte)0;
    
    ppm.inFrameTime_usec += CONVERT_TIMER_TO_USEC( l, h);
    
    switch( ppm.outputState) {
    case BEGIN_OF_FIRST_SPACE:
        PIN_LOW();
        nextTimerValue_usec = PPM_SPACE_usec;

        ppm.switchSet();
        AnalogInput::start();

        ppm.inFrameTime_usec = 0;
        ppm.outputState = END_OF_SPACE;
        break;
        
    case END_OF_SPACE:
        PIN_HIGH();
        nextTimerValue_usec = ppm.activeSet->channel[ppm.outputChannel];

        ppm.outputState = END_OF_MARK;
        break;

    case END_OF_MARK:
        PIN_LOW();
        nextTimerValue_usec = PPM_SPACE_usec;
        
        ppm.outputChannel++;
        if( ppm.outputChannel < PPM_CHANNELS)  {
            ppm.outputState = END_OF_SPACE; // repeat for next channel
        } else {
            ppm.outputChannel = 0;
            ppm.outputState = END_OF_LAST_SPACE; // done
        }
        break;
        
    case END_OF_LAST_SPACE:
        PIN_HIGH();
        nextTimerValue_usec = PPM_FRAME_usec - ppm.inFrameTime_usec;

        ppm.outputState = BEGIN_OF_FIRST_SPACE;
        break;
        
    default:
        nextTimerValue_usec = 0;
    }
    
    /* Multiply by 2 because the timer resulution is 0.5 micro sec. */
    nextTimerValue_usec <<= 1;

    /* Set output compare register. HIGH BYTE FIRST ! */
    OCR1AH = HIGH_BYTE( nextTimerValue_usec);
    OCR1AL = LOW_BYTE( nextTimerValue_usec);
}

PPM::PPM() {
    
    activeSet = (timingSet_t*)malloc( sizeof( timingSet_t));
    modifiableSet = (timingSet_t*)malloc( sizeof( timingSet_t));
    
    init();
}

/* Initialization:
 * - Set all channels to mid.
 * - Switch PPM pin to output.
 * - Initialize timer
 *     Timer 1 counts at a rate of 2Mhz ( 0.5 micro sec. )
 */
void PPM::init() {

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {

        for( channel_t i = 0; i < PPM_CHANNELS; i++) {
            activeSet->channel[i] = PPM_MID_usec;
            modifiableSet->channel[i] = PPM_MID_usec;
        }

        outputChannel = 0;
        outputState = BEGIN_OF_FIRST_SPACE;
        ppmOverrun = 0;
        channelSetDone = true;

        /* Set port line to output */
        DDRD |= _BV(PPM_PORT);

        /* Don't use output compare pins */
        TCCR1A = (byte)0;
        /* Prescaler /8 = 2Mhz = 0.5 usec */
        TCCR1B = _BV(CS11);
        /* Set initial timer counter value */
        TCNT1H = (byte)0;
        TCNT1L = (byte)0;
        /* Set output compare register. HIGH BYTE FIRST ! */
        OCR1AH = HIGH_BYTE( PPM_INIT_usec);
        OCR1AL = LOW_BYTE( PPM_INIT_usec);
        /* Enable output compare match A interrupt */
        TIMSK1 = _BV(OCIE1A);
    }
}

/* Current in frame time, in usec.
 */
ppmTiming_t PPM::getInFrameTime() {
    
    ppmTiming_t t;
    uint8_t l, h;

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {
        t = ppm.inFrameTime_usec;
        l = TCNT1L; // Read low byte first!
        h = TCNT1H;
    }
    
    return t + CONVERT_TIMER_TO_USEC( l, h);
}

/* An overrun occurs if not all channels in the modifiable set
 * were set and a switch occurs.
 */
uint16_t PPM::getOverrunCounter() {

    return ppm.ppmOverrun;
}

/* Switch active and modifiable set.
 * Increate the ppmOverrun counter if the channelSetDone flag has not 
 * been set.
 */
void PPM::switchSet() {
    
    volatile timingSet_t *tmpSet;
    
    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {

        if( ppm.channelSetDone) {
            ppm.channelSetDone = false;
            tmpSet = ppm.activeSet;
            ppm.activeSet = ppm.modifiableSet;
            ppm.modifiableSet = tmpSet;
        } else {
            ppm.ppmOverrun++;
        }
    }
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
void PPM::setChannel( channel_t ch, value_t v) {

    ppmTiming_t t;

    /* Convert:
     * 
     * value_t
     * =======
     * CHANNELVALUE_MID   0
     * CHANNELVALUE_MIN   -1000
     * CHANNELVALUE_MAX   1000
     *
     * To:
     * 
     * ppmTiming_t
     * ===========
     * PPM_MID_usec        ((ppmTiming_t) 1200)
     * PPM_MIN_usec        (PPM_MID_usec - PPM_RANGE_usec)
     * PPM_MAX_usec        (PPM_MID_usec + PPM_RANGE_usec)
     *  
     */

    if( ch < PPM_CHANNELS) {

        t = PPM_MID_usec + (v / 2);

        if( t < PPM_MIN_usec) t = PPM_MIN_usec;
        if( t > PPM_MAX_usec) t = PPM_MAX_usec;
        
        ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {

            ppm.modifiableSet->channel[ ch ] = t;

            if( ch == PPM_CHANNELS-1) {
                ppm.channelSetDone = true;
            }
        }
    }
}
