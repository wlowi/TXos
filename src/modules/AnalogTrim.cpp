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

#include "AnalogTrim.h"

extern const char* const InputChannelNames[INPUT_CHANNELS];

AnalogTrim::AnalogTrim() : Module( MODULE_ANALOG_TRIM_TYPE, TEXT_MODULE_ANALOG_TRIM) {

    setDefaults();
}

/* From Module */

void AnalogTrim::run( Controls &controls) {

    channelValue_t v;
    percent_t currentTrim_pct;
    percent_t adj_pct;

    current = &controls;

    switch( execStep) {
    case ANALOGTRIM_STEP_SAVE:
        for( channel_t ch = 0; ch < PORT_TRIM_INPUT_COUNT; ch++) {
            trim_pct[ch] = CHANNEL_TO_PCT( controls.trimGet(ch));
        }
        execStep = ANALOGTRIM_STEP_ADJUST;
        break;

    case ANALOGTRIM_STEP_ADJUST:
        for( channel_t ch = 0; ch < PORT_TRIM_INPUT_COUNT; ch++) {
            currentTrim_pct = CHANNEL_TO_PCT( controls.trimGet(ch));
            
            if( trim_pct[ch] < 0 && currentTrim_pct > trim_pct[ch]) {
                adj_pct = (currentTrim_pct < 0 ? currentTrim_pct : 0) - trim_pct[ch];
                trim_pct[ch] += adj_pct;
                CFG->storedTrim_pct[ch] -=adj_pct;
                
            } else if( trim_pct[ch] > 0 && currentTrim_pct < trim_pct[ch]) {
                adj_pct = trim_pct[ch] - (currentTrim_pct > 0 ? currentTrim_pct : 0);
                trim_pct[ch] -= adj_pct;
                CFG->storedTrim_pct[ch] +=adj_pct;
            }
        }
        break;

    case ANALOGTRIM_STEP_NONE:
        [[fallthrough]];

    default:
        break;
    }

    for( channel_t ch = 0; ch < PORT_TRIM_INPUT_COUNT; ch++) {
        v = controls.inputGet(ch) + controls.trimGet( ch) +  PCT_TO_CHANNEL(CFG->storedTrim_pct[ch]);
        if( v < CHANNELVALUE_MIN_LIMIT) {
            v = CHANNELVALUE_MIN_LIMIT;
        } else if( v > CHANNELVALUE_MAX_LIMIT) {
            v = CHANNELVALUE_MAX_LIMIT;
        }
        controls.inputSet( ch, v);
    }
}

void AnalogTrim::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION (

        for( channel_t ch = 0; ch < PORT_TRIM_INPUT_COUNT; ch++) {
            CFG->storedTrim_pct[ch] = 0;
        }
    )

    execStep = ANALOGTRIM_STEP_NONE;
}

void AnalogTrim::moduleEnter() {

    LOG("AnalogTrim::moduleEnter()\n");

    /* Save trim values on module entry. */
    execStep = ANALOGTRIM_STEP_SAVE;
}

void AnalogTrim::moduleExit() {

    LOG("AnalogTrim::moduleExit()\n");
    execStep = ANALOGTRIM_STEP_NONE;
}

/* From TableEditable */

uint8_t AnalogTrim::getRowCount() {

    return PORT_TRIM_INPUT_COUNT;
}

const char *AnalogTrim::getRowName( uint8_t row) {

    return InputChannelNames[row];
}

uint8_t AnalogTrim::getColCount( uint8_t row) {

    return 2;
}

bool AnalogTrim::isColEditable( uint8_t row, uint8_t col) {

    return col == 1;
}

void AnalogTrim::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 0) {
        cell->setInt8( TEXT_INPUT_length + 1, CHANNEL_TO_PCT(current->trimGet(row)), 0, PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
    } else {
        cell->setInt8( TEXT_INPUT_length + 6, CFG->storedTrim_pct[row], 0, PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
    }
}

void AnalogTrim::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 1) {
        CFG->storedTrim_pct[row] = cell->getInt8();
    }
}
