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
extern const char *MixChannelNames[MIX_CHANNELS];
extern const channel_t MixChannelMap[MIX_CHANNELS];

AssignInput::AssignInput() : Module( MODULE_ASSIGN_INPUT_TYPE, TEXT_MODULE_ASSIGN_INPUT) {

    setDefaults();
}

/* From Module */

void AssignInput::run( Controls &controls) {

    for( channel_t ch = 0; ch < ANALOG_CHANNELS; ch++) {
        controls.logicalSet( MIX_TO_CHANNEL( CFG->source[ch]), controls.inputGet( ch));
    }
}

void AssignInput::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        for( channel_t ch = 0; ch < ANALOG_CHANNELS; ch++) {
            if( ch < MIX_CHANNELS) {
                CFG->source[ch] = ch;
            } else {
                CFG->source[ch] = MIX_CHANNELS-1;
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

    cell->setList( 4, MixChannelNames, MIX_CHANNELS, CFG->source[row]);
}

void AssignInput::setValue( uint8_t row, uint8_t col, Cell *cell) {

    CFG->source[row] = cell->getList();
}