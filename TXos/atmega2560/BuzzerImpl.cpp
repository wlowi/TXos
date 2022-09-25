
#include <util/atomic.h>

#include "BuzzerImpl.h"

extern BuzzerImpl *buzzerImpl;

static volatile uint8_t sndIdx;

#define H( v ) ((uint8_t)(v >> 8))
#define L( v ) ((uint8_t)(v & 0xff))

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
        /* Set initial timer counter value */
        TCNT3H = (byte)0;
        TCNT3L = (byte)0;
    }
}

void BuzzerImpl::off() {

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {
        /* Disable all timer 3 interrupts */
        TIMSK3 = (byte)0;
        TIFR3 = (byte)0;

        sndIdx = 0;
    }

    ports->buzzerOff();
}

/*
 * The last command in array s must be BUZZER_STOP.
 */
void BuzzerImpl::play( const buzzerCmd_t s[]) {

    uint8_t i = 0;
    
    off();

    while( (i < BUZZER_SOUND_LEN-1) && (BUZZER_CMD( s[i]) != BUZZER_STOP_CMD) ) {
      sound[i] = s[i];
      i++;
    }

    sound[i] = BUZZER_STOP();

    processNext();
}

void BuzzerImpl::processNext() {

  if( sndIdx >= BUZZER_SOUND_LEN) {
    off();
    return;
  }

  switch( BUZZER_CMD( sound[sndIdx])) {
    case BUZZER_STOP_CMD:
      off();
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
        /* Decrease count by one */
        sound[sndIdx]--;
        sndIdx = BUZZER_STEP( sound[sndIdx]); 
      }
      
      processNext();
      break;
  }
}

void BuzzerImpl::scheduleInterrupt( uint8_t t) {

    uint16_t ticks = t * 1600;
    
    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {
        TIMSK3 = (byte)0;
        TIFR3 = (byte)0;

        /* Set initial timer counter value */
        TCNT3H = (byte)0;
        TCNT3L = (byte)0;
        /* Set output compare register. HIGH BYTE FIRST ! */
        OCR3AH = H( ticks);
        OCR3AL = L( ticks);
        /* Enable output compare match A interrupt */
        TIMSK3 = _BV(OCIE3A);
    }
}
