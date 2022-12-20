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

#include "Statistics.h"

#define STATISTIC_COUNT 5

const char* const statisticNames[STATISTIC_COUNT] {
    TEXT_STATISTIC_TIMING,
    TEXT_STATISTIC_UI,
    TEXT_STATISTIC_MODULE,
    TEXT_STATISTIC_PPMOVER,
    TEXT_STATISTIC_FRAMETIME
};

Statistics::Statistics() : Module( MODULE_STATISTICS_TYPE, TEXT_MODULE_STATISTICS) {

}

void Statistics::updateUITime( uint16_t t) {

    if( t > timeUI_msec) {
        timeUI_msec = t;
    }
}

void Statistics::updateModulesTime( uint16_t t) {

    if( t > timeModules_msec) {
        timeModules_msec = t;
    }
}

void Statistics::updatePPMOverrun( uint16_t c) {

    if( c > ppmOverrun) {
        ppmOverrun = c;
    }
}

void Statistics::updateFrameTime( timingUsec_t t) {

    if( t > maxFrameTime) {
        maxFrameTime = t;
    }
}


bool Statistics::debugTiming() {

    return timing;
}

/* From Module */

void Statistics::run( Controls &controls) {

}

void Statistics::setDefaults() {

    timeUI_msec = 0;
    timeModules_msec = 0;
    ppmOverrun = 0;
    maxFrameTime = 0;
    timing = false;
}

/* From TableEditable */

bool Statistics::isRowEditable( uint8_t row) {

    return (row == 0);
}

uint8_t Statistics::getRowCount() {

    return STATISTIC_COUNT;
}

const char *Statistics::getRowName( uint8_t row) {

    return statisticNames[row];
}

uint8_t Statistics::getColCount( uint8_t row) {

    return 1;
}

void Statistics::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( row == 0) {
        cell->setBool( 10, timing);
    } else if( row == 1) {
        cell->setInt16( 7, timeUI_msec, 0, 0, 0);
    } else if( row == 2) {
        cell->setInt16( 7, timeModules_msec, 0, 0, 0);
    } else if( row == 3) {
        cell->setInt16( 7, ppmOverrun, 0, 0, 0);
    } else if( row == 4) {
        cell->setInt16( 7, maxFrameTime, 0, 0, 0);
    }
}

void Statistics::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( row == 0) {
        timing = cell->getBool();
    }
}

bool Statistics::hasChanged( uint8_t row, uint8_t col) {

    return (row != 0);
}