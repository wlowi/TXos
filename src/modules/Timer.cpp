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

#include "Timer.h"
#include "Buzzer.h"

extern Buzzer buzzer;

const buzzerCmd_t SoundMinute[] = {
  BUZZER_PLAY( 2),
  BUZZER_STOP()
};

const buzzerCmd_t SoundSecond[] = {
  BUZZER_PLAY( 1),
  BUZZER_STOP()
};

const buzzerCmd_t SoundZero[] = {
  BUZZER_PLAY( 5),
  BUZZER_STOP()
};

/* The import/export dictionary. 
 * See ImportExport.h
 */
DICTROW( r1, COMM_DATATYPE_UINT8, COMM_FIELD_SWITCH, flightTimer_t, swState)
DICTROW( r2, COMM_DATATYPE_UINT16, COMM_FIELD_TIME, flightTimer_t, time_sec)
DICT( Timer, COMM_SUBPACKET_TIMER, &r1, &r2)

Timer::Timer() : Module( MODULE_TIMER_TYPE, TEXT_MODULE_TIMER) {

    setDefaults();
}

void Timer::reset() {

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {
        countdown_sec = CFG->time_sec;
        lastMillis = 0;
    }
    LOGV("Timer::reset: %d\n", countdown_sec);
}

uint16_t Timer::timeSec() {

    uint16_t t;

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {
        t = countdown_sec;
    }

    return t;
}

char *Timer::timeStr() {

    uint16_t t;
    uint8_t min;
    uint8_t sec;

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {
        t = countdown_sec;
    }

    min = t / 60;
    sec = t % 60;

    if(min < 10) {
        timeAsString[0] = ' ';
    } else {
        timeAsString[0] = '0' + min / 10;
    }

    timeAsString[1] = '0' + min % 10;
    timeAsString[2] = ':';
    timeAsString[3] = '0' + sec / 10;
    timeAsString[4] = '0' + sec % 10;
    timeAsString[5] = '\0';
    
    return timeAsString;
}

/* From Module */

void Timer::exportConfig( ImportExport *exporter, uint8_t *config, moduleSize_t configSz) const {

    exporter->runExport( DICT_ptr(Timer), DICTROW_ptr(Timer), config, sizeof(flightTimer_t));
}

void Timer::run( Controls &controls) {

    long c;
    bool update = false;

    if( controls.evalSwitches( CFG->swState) && countdown_sec > 0) {
        ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {
            c = millis();
            if( c > lastMillis +1500) {
                lastMillis = c;
                countdown_sec--;
                update = true;
            } else if(c > lastMillis +1000) {
                lastMillis += 1000;
                countdown_sec--;
                update = true;
            }
        }

        if( update) {
            if( countdown_sec == 0) {
                LOG("SoundZero\n");
                buzzer.play( SoundZero);
            } else if( countdown_sec <= 10) {
                LOG("SoundSecond\n");
                buzzer.play( SoundSecond);
            } else if( (countdown_sec % 60) == 0) {
                LOG("SoundMinute\n");
                buzzer.play( SoundMinute);
            }
        }
    }
}

void Timer::init() {

    reset();
    LOGV("Timer::init: %d\n", countdown_sec);
}

void Timer::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        INIT_SWITCH( CFG->swState);
        CFG->time_sec = 0;
    )

    reset();
}

/* From TableEditable */

uint8_t Timer::getRowCount() {

    return 2;
}

const char *Timer::getRowName( uint8_t row) {

    if( row == 0) {
        return TEXT_SWITCH;
    }

    return TEXT_TIME;
}

uint8_t Timer::getColCount( uint8_t row) {

    if( row == 0) {
        return 1;
    }

    return 3;
}

void Timer::getValue( uint8_t row, uint8_t col, Cell *cell) {

    uint8_t min;
    uint8_t sec;
    
    if( row == 0) {
        cell->setSwitchState( 7, CFG->swState);
    } else {
        if( col == 0) {
            min = CFG->time_sec / 60;
            cell->setInt8( 7, min, 2, 0, 59);
        } else if( col == 1) {
            cell->setLabel( 9, ":", 1);
        } else if( col == 2) {
            sec = CFG->time_sec % 60;
            cell->setInt8( 10, sec, 2, 0, 59);
        }
    }
}

void Timer::setValue( uint8_t row, uint8_t col, Cell *cell) {

    uint8_t min;
    uint8_t sec;

    if( row == 0) {
        CFG->swState = cell->getSwitchState();
    } else {
        
        min = CFG->time_sec / 60;
        sec = CFG->time_sec % 60;

        if( col == 0) {
            min = cell->getInt8();
        } else if ( col == 2) {
            sec = cell->getInt8();
        }

        CFG->time_sec = (uint16_t)60 * min + sec;
    }
}
