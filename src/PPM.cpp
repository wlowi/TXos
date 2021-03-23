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

PPM ppm;

ISR( TIMER1_COMPA_vect) {
    
    ppmTiming_t t_usec;
    uint8_t h, l;
    
    l = TCNT1L;
    h = TCNT1H;

    /* Reset counter */
    TCNT1H = (byte)0;
    TCNT1L = (byte)0;
    
    ppm.inFrameTime += ((((ppmTiming_t)h << 8) + l) >> 1);
    
    switch( ppm.outputState) {
    case 0: /* begin of first space of a frame, set pin to low */
        PORTD &= ~_BV( PPM_PORT);

        ppm.switchSet();
        AnalogInput::start();

        t_usec = PPM_SPACE_usec;

        ppm.outputState++;
        ppm.channelTime = ppm.inFrameTime = 0;
        break;
        
    case 1: /* end of space, set to high */
        PORTD |= _BV( PPM_PORT);
        
        t_usec = ppm.activeSet->channel[ppm.outputChannel];

        ppm.outputState++;
        break;

    case 2: /* end of mask, set to low */
        PORTD &= ~_BV( PPM_PORT);
        
        t_usec = PPM_SPACE_usec;
        
        ppm.outputChannel++;
        if( ppm.outputChannel >= PPM_CHANNELS)  {
            ppm.outputChannel = 0;
            ppm.outputState++; // done
        } else {
            ppm.outputState--; // repeat, back to state 1
        }
        break;
        
    case 3: /* end of last space, set to high */
        PORTD |= _BV( PPM_PORT);

        t_usec = PPM_FRAME_usec - ppm.channelTime;

        ppm.outputState = 0;
        break;
        
    default:
        t_usec = 0;
    }

    ppm.channelTime += t_usec;
    
    /* Multiply by 2 because the timer resulution is 0.5 micro sec. */
    t_usec <<= 1;

    /* Set output compare register. HIGH BYTE FIRST ! */
    OCR1AH = HIGH_BYTE( t_usec);
    OCR1AL = LOW_BYTE( t_usec);
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
        outputState = 0;
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

ppmTiming_t PPM::getInFrameTime() {
    
    ppmTiming_t t;
    uint8_t l, h;

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {
        t = ppm.inFrameTime;
        l = TCNT1L; // Read low byte first!
        h = TCNT1H;
    }
    
    return t + ((((ppmTiming_t)h << 8) + l) >> 1);
}

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
