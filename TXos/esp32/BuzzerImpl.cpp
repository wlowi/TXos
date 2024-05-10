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

#include "BuzzerImpl.h"

/* System dependent implementations of Buzzer.
 * For more documentation see Buzzer.h
 * 
 * This implementation uses Atmel Timer 3 compare A interrupt.
 */

extern BuzzerImpl *buzzerImpl;

static volatile uint8_t sndIdx;

#define H( v ) ((uint8_t)(v >> 8))
#define L( v ) ((uint8_t)(v & 0xff))

/* Frequency is 16Khz => 62.5 usec.
 * 1600 ticks are 100 msec
 */
#define TICKS_PER_100msec   1600

#if defined( ARDUINO_ARCH_AVR )
ISR( TIMER1_COMPA_vect) {

    buzzerImpl->processNext();
}
#else
# warning FIXIT
#endif

BuzzerImpl::BuzzerImpl() : ports( NULL) {
  
}

void BuzzerImpl::init( Ports &p) {
  
#if defined( ARDUINO_ARCH_AVR )
    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {
      
        ports = &p;
        sound[0] = BUZZER_STOP();
        
        off();
    
        /* Don't use output compare pins */
        TCCR1A = (byte)0;
        
        /* Prescaler /1024 = 16khz = 62.5 usec */
        TCCR1B = _BV(CS12) | _BV(CS10);
        
        /* */
        TCCR1C = (byte)0;
        
        /* Set initial timer counter value */
        TCNT1H = (byte)0;
        TCNT1L = (byte)0;
    }

#else
# warning FIXIT
#endif
}

/* 
 * Cancel sound sequence.
 */
void BuzzerImpl::off() {

    stopSound(); 

    alarm = false;
    soundAlarm = NULL;
}

/* 
 * Cancel sound sequence.
 */
void BuzzerImpl::stopSound() {
#if defined( ARDUINO_ARCH_AVR )
    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {
      
        /* Disable all timer 1 compare A interrupt */
        TIMSK1 &= ~(_BV(OCIE1A));
        TIFR1 = _BV(OCF1A);

        sndIdx = 0;
    }
#else
# warning FIXIT
#endif
    ports->buzzerOff();
}

/*
 * The last command in array s must be BUZZER_STOP.
 */
void BuzzerImpl::play( const buzzerCmd_t s[]) {

    uint8_t i;
    
    stopSound();

    for( i = 0; (i < BUZZER_SOUND_LEN-1) && (BUZZER_CMD( s[i]) != BUZZER_STOP_CMD); i++ ) {
        sound[i] = s[i];
    }
    sound[i] = BUZZER_STOP();

    processNext();
}

void BuzzerImpl::playPermanent( const buzzerCmd_t s[]) {

    if( alarm && s == soundAlarm) {
      return;
    }

    stopSound();
    alarm = true;
    soundAlarm = s;
    
    play( s);
}

/*
 * Process next buzzer sequence command.
 */
void BuzzerImpl::processNext() {

    bool notDone;
    uint8_t i;
  
    if( sndIdx >= BUZZER_SOUND_LEN) {
        stopSound();
        return;
    }
#if defined( ARDUINO_ARCH_AVR )
    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {
#endif
        do {
            notDone = false;
    
            switch( BUZZER_CMD( sound[sndIdx])) {
                case BUZZER_STOP_CMD:
                    stopSound();
                    if( alarm) {
                      /* Repeat alarm */
                      for( i = 0; (i < BUZZER_SOUND_LEN-1) && (BUZZER_CMD( soundAlarm[i]) != BUZZER_STOP_CMD); i++ ) {
                        sound[i] = soundAlarm[i];
                      }
                      sound[i] = BUZZER_STOP();
                      notDone = true;
                    }
                    break;
      
                case BUZZER_PLAY_CMD:
                    ports->buzzerOn();
                    scheduleInterrupt( BUZZER_TIME( sound[sndIdx]));
                    sndIdx++;
                    break;

                case BUZZER_PAUSE_CMD:
                    ports->buzzerOff();
                    scheduleInterrupt( BUZZER_TIME( sound[sndIdx]));
                    sndIdx++;
                    break;

                case BUZZER_REPEAT_CMD:
                    if( BUZZER_COUNT( sound[sndIdx]) == 0) {
                        /* Repeat forever */
                        sndIdx = BUZZER_STEP( sound[sndIdx]);
        
                    } else if( BUZZER_COUNT( sound[sndIdx]) == 1) {
                        /* Done repeating */
                        sndIdx++;
        
                    } else {
                        /* Decrease count by one.
                         * This works because the counter is implemented as the 3 LSB.
                         */
                        sound[sndIdx]--;
                        sndIdx = BUZZER_STEP( sound[sndIdx]); 
                    }
                    notDone = true;
                    break;
            }
        } while( notDone);

#if defined( ARDUINO_ARCH_AVR )
    }
#endif
}

/* 
 *  Schedule next timer interrupt.
 *   t is in 100msec units
 */
void BuzzerImpl::scheduleInterrupt( uint8_t t) {
  
#if defined( ARDUINO_ARCH_AVR )
    uint16_t ticks = t * TICKS_PER_100msec;

    /* Does not need to be in atomic block because it's always called from
     * an atomic block.
     */

    /* Disable timer 1 interrupt */
    TIMSK1 &= ~(_BV(OCIE1A));
    TIFR1 = _BV(OCF1A);
    
    /* Set initial timer counter value */
    TCNT1H = (byte)0;
    TCNT1L = (byte)0;
    
    /* Set output compare register. HIGH BYTE FIRST ! */
    OCR1AH = H( ticks);
    OCR1AL = L( ticks);
    
    /* Enable output compare match A interrupt */
    TIMSK1 |= _BV(OCIE1A);
#else
# warning FIXIT
#endif
}
