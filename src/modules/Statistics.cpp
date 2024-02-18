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

#include "Statistics.h"

#define STATISTIC_COUNT 8

const char* const statisticNames[STATISTIC_COUNT] {
    TEXT_STATISTIC_TIMING,
    TEXT_STATISTIC_OVERRUN,
    TEXT_STATISTIC_UI,
    TEXT_STATISTIC_MODULE,
    TEXT_STATISTIC_PPMOVER,
    TEXT_STATISTIC_FRAMETIME,
    TEXT_STATISTIC_WDT,
    TEXT_STATISTIC_MEMFREE
};

Statistics::Statistics() : Module( MODULE_STATISTICS_TYPE, TEXT_MODULE_STATISTICS, COMM_SUBPACKET_NONE) {

    setDefaults();
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

void Statistics::updateWdTimeout( uint16_t t) {

    if( t > wdTimeout) {
        wdTimeout = t;
    }
}

void Statistics::updateMemFree( size_t m) {

    memfree = m;
}

bool Statistics::debugTiming() const {

    return dumpTiming;
}

bool Statistics::debugOverrun() const {

    return dumpOverrun;
}

/* From Module */

void Statistics::run( Controls &controls) {

}

void Statistics::setDefaults() {

    timeUI_msec = 0;
    timeModules_msec = 0;
    wdTimeout = 0;
    ppmOverrun = 0;
    maxFrameTime = 0;
    memfree = 0;
    dumpTiming = false;
    dumpOverrun = false;
}

/* From TableEditable */

bool Statistics::isRowEditable( uint8_t row) {

    return (row < 2);
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
        cell->setBool( 10, dumpTiming);
    } else if( row == 1) {
        cell->setBool( 10, dumpOverrun);
    } else if( row == 2) {
        cell->setInt16( 7, timeUI_msec, 0, 0, 0);
    } else if( row == 3) {
        cell->setInt16( 7, timeModules_msec, 0, 0, 0);
    } else if( row == 4) {
        cell->setInt16( 7, ppmOverrun, 0, 0, 0);
    } else if( row == 5) {
        cell->setInt16( 7, maxFrameTime, 0, 0, 0);
    } else if( row == 6) {
        cell->setInt16( 7, wdTimeout, 0, 0, 0);
    } else if( row == 7) {
        cell->setInt16( 7, (int16_t)memfree, 0, 0, 0);
    }
}

void Statistics::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( row == 0) {
        dumpTiming = cell->getBool();
    } else if( row == 1) {
        dumpOverrun = cell->getBool();
    }
}

bool Statistics::hasChanged( uint8_t row, uint8_t col) {

    return (row > 1);
}
