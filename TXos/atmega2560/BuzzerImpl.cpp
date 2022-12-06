/*
    TXos. A remote control transmitter OS.

    Copyright (C) 2022 Wolfgang Lohwasser

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#include <util/atomic.h>

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

ISR( TIMER3_COMPA_vect) {

    buzzerImpl->processNext();
}

BuzzerImpl::BuzzerImpl() : ports( NULL) {
  
}

void BuzzerImpl::init( Ports &p) {

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {
      
        ports = &p;
        sound[0] = BUZZER_STOP();
        
        off();
    
        /* Don't use output compare pins */
        TCCR3A = (byte)0;
        
        /* Prescaler /1024 = 16khz = 62.5 usec */
        TCCR3B = _BV(CS32) | _BV(CS30);
        
        /* */
        TCCR3C = (byte)0;
        
        /* Set initial timer counter value */
        TCNT3H = (byte)0;
        TCNT3L = (byte)0;
    }
}

/* 
 * Cancel sound sequence.
 */
void BuzzerImpl::off() {

    alarm = false;
    stopSound(); 
}

/* 
 * Cancel sound sequence.
 */
void BuzzerImpl::stopSound() {

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {
      
        /* Disable all timer 3 compare A interrupt */
        TIMSK3 &= ~(_BV(OCIE3A));
        TIFR3 = _BV(OCF3A);

        sndIdx = 0;
    }

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

    uint8_t i;
    
    stopSound();

    alarm = true;

    for( i = 0; (i < BUZZER_SOUND_LEN-1) && (BUZZER_CMD( s[i]) != BUZZER_STOP_CMD); i++ ) {
        sound[i] = soundAlarm[i] = s[i];
    }
    sound[i] = soundAlarm[i] = BUZZER_STOP();

    processNext();
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

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {

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
    
    }
}

/* 
 *  Schedule next timer interrupt.
 *   t is in 100msec units
 */
void BuzzerImpl::scheduleInterrupt( uint8_t t) {

    uint16_t ticks = t * TICKS_PER_100msec;

    /* Does not need to be in atomic block because it's always called from
     * an atomic block.
     */
      
    /* Disable timer 3 interrupt */
    TIMSK3 &= ~(_BV(OCIE3A));
    TIFR3 = _BV(OCF3A);
    
    /* Set initial timer counter value */
    TCNT3H = (byte)0;
    TCNT3L = (byte)0;
    
    /* Set output compare register. HIGH BYTE FIRST ! */
    OCR3AH = H( ticks);
    OCR3AL = L( ticks);
    
    /* Enable output compare match A interrupt */
    TIMSK3 |= _BV(OCIE3A);
}
