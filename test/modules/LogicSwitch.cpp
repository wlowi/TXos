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

#include "LogicSwitch.h"

extern Controls controls;

const char *logicTypes[TEXT_LOGIC_SW_TYPE_count] = {
    TEXT_LOGIC_SW_TYPE1,
    TEXT_LOGIC_SW_TYPE2,
    TEXT_LOGIC_SW_TYPE3,
    TEXT_LOGIC_SW_TYPE4,
    TEXT_LOGIC_SW_TYPE5,
    TEXT_LOGIC_SW_TYPE6
};

LogicSwitch::LogicSwitch() : Module( MODULE_LOGIC_SWITCH_TYPE, TEXT_MODULE_LOGIC_SWITCH) {

    setDefaults();
}

/* From Module */

void LogicSwitch::run( Controls &controls) {

    bool swA;
    bool swB;
    bool swC;
    bool result;
    switch_t logicSw;

    for( switch_t sw = 0; sw < LOGIC_SWITCHES; sw++) {
        if( IS_SWITCH_UNUSED( CFG->swStateA[sw])
            || IS_SWITCH_UNUSED( CFG->swStateB[sw])) {
            continue;
        }

        swA = controls.evalSwitches( CFG->swStateA[sw]);
        swB = controls.evalSwitches( CFG->swStateB[sw]);
        swC = controls.evalSwitches( CFG->swStateC[sw]);

        switch( CFG->type[sw]) {
        case 0: // A & B
            result = swA && swB;
            break;    
        case 1: // A | B
            result = swA || swB;
            break;    
        case 2: // A & B & C
            result = swA && swB && swC;
            break;    
        case 3: // A | B | C
            result = swA || swB || swC;
            break;    
        case 4: // (A & B) | C
            result = (swA && swB) || swC;
            break;    
        case 5: // (A | B) & C
            result = (swA || swB) && swC;
            break;
        default:
            result = false;  
        }

        if( result) {
            logicSw = controls.getSwitchByType( SW_CONF_LOGIC, sw);
            if( IS_SWITCH_USED(logicSw)) {
                controls.switchSet( logicSw, SW_STATE_1);
            }
        }
    }
}

void LogicSwitch::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        for( switch_t sw = 0; sw < LOGIC_SWITCHES; sw++) {
            CFG->type[sw] = 0;
            INIT_SWITCH( CFG->swStateA[sw]);
            INIT_SWITCH( CFG->swStateB[sw]);
            INIT_SWITCH( CFG->swStateC[sw]);
        }
    )
}

/* From TableEditable */

uint8_t LogicSwitch::getRowCount() {

    return 4 * LOGIC_SWITCHES;
}

const char *LogicSwitch::getRowName( uint8_t row) {

    if( row % 4) {
        return TEXT_MSG_NONE;
    } else {
        controls.copySwitchName( switchName, (switch_t)(row/4) + LOGIC_SWITCHES_FIRST_IDX);
        return switchName;
    }
}

uint8_t LogicSwitch::getColCount( uint8_t row) {

    return (row % 4) ? 2 : 1;
}

void LogicSwitch::getValue( uint8_t row, uint8_t col, Cell *cell) {

    uint8_t sw = row / 4;
    uint8_t r = (row % 4);

    if( r == 0) {
        cell->setList( 4, logicTypes, TEXT_LOGIC_SW_TYPE_count, CFG->type[sw]);
    } else if( r == 1) {
        if( col == 0) {
            cell->setLabel(2, TEXT_LOGIC_SW_A, 1);
        } else {
            cell->setSwitchState( 4, CFG->swStateA[sw]);
        }
    } else if( r == 2) {
        if( col == 0) {
            cell->setLabel(2, TEXT_LOGIC_SW_B, 1);
        } else {
            cell->setSwitchState( 4, CFG->swStateB[sw]);
        }
    } else if( r == 3) {
        if( col == 0) {
            cell->setLabel(2, TEXT_LOGIC_SW_C, 1);
        } else {
            cell->setSwitchState( 4, CFG->swStateC[sw]);
        }
    }
}

void LogicSwitch::setValue( uint8_t row, uint8_t col, Cell *cell) {

    uint8_t sw = row / 4;
    uint8_t r = (row % 4);

    if( r == 0) {
        CFG->type[sw] = cell->getList();
    } else if( r == 1 && col == 1) {
        CFG->swStateA[sw] = cell->getSwitchState();
    } else if( r == 2 && col == 1) {
        CFG->swStateB[sw] = cell->getSwitchState();
    } else if( r == 3 && col == 1) {
        CFG->swStateC[sw] = cell->getSwitchState();
    }
}
