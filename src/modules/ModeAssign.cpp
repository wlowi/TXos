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

#include "ModeAssign.h"

extern const char* const InputChannelNames[INPUT_CHANNELS];
extern const char* const LogicalChannelNames[LOGICAL_CHANNELS];

ModeAssign::ModeAssign() : Module( MODULE_MODE_ASSIGN_TYPE, TEXT_MODULE_MODE_ASSIGN) {

    setDefaults();
}

channel_t ModeAssign::getModeChannel( channel_t ch) const {

    return CFG->source[ch];
}

/* From Module */

void ModeAssign::run( Controls &controls) {

    /* noop */
}

void ModeAssign::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        for( channel_t ch = 0; ch < MODE_CHANNELS; ch++) {
            CFG->source[ch] = ch;
        }
    )
}

/* From TableEditable */

uint8_t ModeAssign::getRowCount() {

    return MODE_CHANNELS;
}

const char *ModeAssign::getRowName( uint8_t row) {

    return LogicalChannelNames[row];
}

uint8_t ModeAssign::getColCount( uint8_t row) {

    return 2;
}

void ModeAssign::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 0) {
        cell->setLabel( TEXT_CONTROL_length +1, TEXT_ARROW_LEFT, 1);
    } else if( col == 1) {
        cell->setList( TEXT_CONTROL_length +3, InputChannelNames, MODE_CHANNELS, CFG->source[row]);
    }
}

void ModeAssign::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 1) {
        CFG->source[row] = cell->getList();
    }
}