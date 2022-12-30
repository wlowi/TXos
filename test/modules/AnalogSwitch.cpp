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

#include "AnalogSwitch.h"

extern Controls controls;
extern const char* const InputChannelNames[ANALOG_CHANNELS];

AnalogSwitch::AnalogSwitch() : Module( MODULE_ANALOG_SWITCH_TYPE, TEXT_MODULE_ANALOG_SWITCH) {

    setDefaults();
}

/* From Module */

void AnalogSwitch::run( Controls &controls) {

    long v;
    switchState_t state;

    for( switch_t sw = 0; sw < CHANNEL_SWITCHES; sw++) {
        /* Convert v from range -125 - 125 tp -100 - 100 before compare */
        v = (long)controls.inputGet(CFG->source[sw]) * PERCENT_MAX / PERCENT_MAX_LIMIT;
        
        state = ((channelValue_t)v >= PCT_TO_CHANNEL(CFG->trigger_pct[sw])) ? SW_STATE_1 : SW_STATE_0;
        controls.switchSet( CHANNEL_SWITCHES_FIRST_IDX + sw, state);
    }
}

void AnalogSwitch::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        for( switch_t sw = 0; sw < CHANNEL_SWITCHES; sw++) {
            CFG->source[sw] = 0;
            CFG->trigger_pct[sw] = 0; /* neutral */
        }
    )
}

/* From TableEditable */

uint8_t AnalogSwitch::getRowCount() {

    return 2 * CHANNEL_SWITCHES;
}

const char *AnalogSwitch::getRowName( uint8_t row) {

    if( row % 2) {
        return TEXT_MSG_NONE;
    } else {
        controls.copySwitchName( switchName, (switch_t)(row/2) + CHANNEL_SWITCHES_FIRST_IDX);
        return switchName;
    }
}

uint8_t AnalogSwitch::getColCount( uint8_t row) {

    return 1;
}

void AnalogSwitch::getValue( uint8_t row, uint8_t col, Cell *cell) {

    uint8_t sw = row / 2;
    uint8_t r = (row % 2);

    if( r == 0) {
        cell->setList( 6, InputChannelNames, ANALOG_CHANNELS, CFG->source[sw]);
    } else if( r == 1) {
        cell->setInt8( 6, CFG->trigger_pct[sw], 0, PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
    }
}

void AnalogSwitch::setValue( uint8_t row, uint8_t col, Cell *cell) {

    uint8_t sw = row / 2;
    uint8_t r = (row % 2);

    if( r == 0) {
        CFG->source[sw] = cell->getList();
    } else if( r == 1) {
        CFG->trigger_pct[sw] = cell->getInt8();
    }
}
