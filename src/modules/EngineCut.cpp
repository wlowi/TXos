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

#include "EngineCut.h"

EngineCut::EngineCut() : Module( MODULE_ENGINE_CUT_TYPE, TEXT_MODULE_ENGINE_CUT) {

    setDefaults();
}

bool EngineCut::isSave() {

    return save;
}

/* From Module */

void EngineCut::run( Controls &controls) {

    save = controls.evalSwitches( CFG->swState);

    if( save) {
        controls.logicalSet( CHANNEL_THROTTLE, PCT_TO_CHANNEL(CFG->cut_pct));
    }
    
}

void EngineCut::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        CFG->cut_pct = -100;
        INIT_SWITCH( CFG->swState);

    )

    save = false;
}

/* From TableEditable */
uint8_t EngineCut::getRowCount() {

    return 2;
}

const char *EngineCut::getRowName( uint8_t row) {

    if( row == 0) {
        return TEXT_SWITCH;
    } else {
        return TEXT_THR;
    }
}

uint8_t EngineCut::getColCount( uint8_t row) {

    return 1;
}

void EngineCut::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( row == 0) {
        cell->setSwitchState( 7, CFG->swState);
    } else {
        cell->setInt8( 7, CFG->cut_pct, 0, PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
    }
}

void EngineCut::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( row == 0) {
        CFG->swState = cell->getSwitchState();
    } else {
        CFG->cut_pct = cell->getInt8();
    }
}