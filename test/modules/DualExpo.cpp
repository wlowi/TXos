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

#include "DualExpo.h"

DualExpo::DualExpo() : Module( MODULE_DUAL_EXPO_TYPE, TEXT_MODULE_DUAL_EXPO) {

    setDefaults();
}

void DualExpo::run( Controls &controls) {

}

void DualExpo::setDefaults() {

    INIT_PHASED_CONFIGURATION(

        CFG->rate = 100;
        CFG->expo = 0;

    )
}

/* From Module */

moduleSize_t DualExpo::getConfigSize() {

    return (moduleSize_t)sizeof( configuration);
}

uint8_t *DualExpo::getConfig() {

    return (uint8_t*)&configuration;
}

void DualExpo::switchPhase(phase_t ph) {

    LOGV("DualExpo::switchPhase: new phase %d\n", ph);
    SWITCH_PHASE( ph);
}

/* From TableEditable */

uint8_t DualExpo::getRowCount() {

    return 3;
}

const char *DualExpo::getRowName( uint8_t row) {

    if( row == 0) {
        return TEXT_PHASE;
    } else if( row == 1) {
        return TEXT_RATE;
    }

    return TEXT_EXPO;
}

bool DualExpo::isRowEditable( uint8_t row) {

    if( row == 0) {
        return false;
    }

    return true;
}

uint8_t DualExpo::getColCount( uint8_t row) {

    return 1;
}

void DualExpo::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( row == 0) {
        cell->setInt8( 7, phase, 0, PHASES);
    } else if( row == 1) {
        cell->setInt8( 7, CFG->rate, PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
    } else {
        cell->setInt8( 7, CFG->expo, 0, 100);
    }
}

void DualExpo::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( row == 0) {
        /* noop */
    } else if( row == 1) {
        CFG->rate = cell->getInt8();
    } else {
        CFG->expo = cell->getInt8();
    }
}