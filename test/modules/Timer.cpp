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

#include "Timer.h"

Timer::Timer() : Module( MODULE_TIMER_TYPE, TEXT_MODULE_TIMER) {

    setDefaults();
}

void Timer::reset() {

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {
        time_sec = CFG->time_sec;
    }
}

uint16_t Timer::timeSec() {

    uint16_t t;

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {
        t = time_sec;
    }

    return t;
}

/* From Module */

void Timer::run( Controls &controls) {

    uint16_t t;

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {
        t = time_sec;

        time_sec = t;
    }
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