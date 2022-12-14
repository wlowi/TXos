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

#include "AssignInput.h"

extern const char *InputChannelNames[ANALOG_CHANNELS];
extern const char *LogicalChannelNames[LOGICAL_CHANNELS];

AssignInput::AssignInput() : Module( MODULE_ASSIGN_INPUT_TYPE, TEXT_MODULE_ASSIGN_INPUT) {

    setDefaults();
}

/* From Module */

void AssignInput::run( Controls &controls) {

    for( channel_t ch = 0; ch < ANALOG_CHANNELS; ch++) {
        controls.logicalSet( CFG->source[ch], controls.inputGet( ch));
    }
}

void AssignInput::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        for( channel_t ch = 0; ch < ANALOG_CHANNELS; ch++) {
            if( ch == 0) {
                CFG->source[ch] = CHANNEL_THROTTLE;
            } else if( ch == 1) {
                CFG->source[ch] = CHANNEL_AILERON;
            } else if( ch == 2) {
                CFG->source[ch] = CHANNEL_ELEVATOR;
            } else if( ch == 3) {
                CFG->source[ch] = CHANNEL_RUDDER;
            } else if( ch == 4) {
                CFG->source[ch] = CHANNEL_FLAP;
            } else if( ch == 5) {
                CFG->source[ch] = CHANNEL_SPOILER;
            } else if( ch == 6) {
                CFG->source[ch] = CHANNEL_GEAR;
            } else if( ch == 7) {
                CFG->source[ch] = CHANNEL_8;
            } else {
                CFG->source[ch] = CHANNEL_9;
            }
        }
    )
}

/* From TableEditable */

uint8_t AssignInput::getRowCount() {

    return ANALOG_CHANNELS;
}

const char *AssignInput::getRowName( uint8_t row) {

    return InputChannelNames[row];
}

uint8_t AssignInput::getColCount( uint8_t row) {

    return 1;
}

void AssignInput::getValue( uint8_t row, uint8_t col, Cell *cell) {

    cell->setList( 4, LogicalChannelNames, LOGICAL_CHANNELS, CFG->source[row]);
}

void AssignInput::setValue( uint8_t row, uint8_t col, Cell *cell) {

    CFG->source[row] = cell->getList();
}