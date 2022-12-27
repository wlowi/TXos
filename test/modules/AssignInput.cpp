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

extern const char* const InputChannelNames[ANALOG_CHANNELS];
extern const char* const MixChannelNames[MIX_CHANNELS];
extern const channel_t MixChannelMap[MIX_CHANNELS];

AssignInput::AssignInput() : Module( MODULE_ASSIGN_INPUT_TYPE, TEXT_MODULE_ASSIGN_INPUT) {

    setDefaults();
}

/* From Module */

void AssignInput::run( Controls &controls) {

    /* Mix channel numbers are an index to MixChannelMap. 
     * To get the real channel number they need to be 
     * mapped via real_channel = mixChannelMap[ source ]
     */
    for( channel_t ch = 0; ch < MIX_CHANNELS; ch++) {
        controls.logicalSet( MIX_TO_CHANNEL(ch), controls.inputGet( CFG->source[ch]));
    }
}

void AssignInput::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        for( channel_t ch = 0; ch < MIX_CHANNELS; ch++) {
            if( ch < ANALOG_CHANNELS) {
                CFG->source[ch] = ch;
            } else {
                CFG->source[ch] = ANALOG_CHANNELS-1;
            }
        }
    )
}

/* From TableEditable */

uint8_t AssignInput::getRowCount() {

    return MIX_CHANNELS;
}

const char *AssignInput::getRowName( uint8_t row) {

    return MixChannelNames[row];
}

uint8_t AssignInput::getColCount( uint8_t row) {

    return 2;
}

void AssignInput::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 0) {
        cell->setLabel( TEXT_CONTROL_length +1, TEXT_ARROW_LEFT, 1);
    } else if( col == 1) {
        cell->setList( TEXT_CONTROL_length +3, InputChannelNames, ANALOG_CHANNELS, CFG->source[row]);
    }
}

void AssignInput::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 1) {
        CFG->source[row] = cell->getList();
    }
}