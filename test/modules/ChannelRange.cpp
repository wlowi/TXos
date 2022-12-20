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

#include "ChannelRange.h"

extern const char* const InputChannelNames[ANALOG_CHANNELS];

ChannelRange::ChannelRange() : Module( MODULE_CHANNEL_RANGE_TYPE, TEXT_MODULE_CHANNEL_RANGE) {

    setDefaults();
}

/* From Module */

void ChannelRange::run( Controls &controls) {

    long v;

    for( channel_t ch = 0; ch < PORT_ANALOG_INPUT_COUNT; ch++) {

        v = controls.inputGet( ch);
        v = v * CFG->range_pct[ch] / PERCENT_MAX_LIMIT;
        controls.inputSet( ch, (channelValue_t)v);
        controls.rangeSet( ch, CFG->range_pct[ch]);
    }
}

void ChannelRange::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        for( channel_t ch = 0; ch < PORT_ANALOG_INPUT_COUNT; ch++) {
            CFG->range_pct[ch] = 100;
        }

    )
}

/* From TableEditable */

uint8_t ChannelRange::getRowCount() {

    return PORT_ANALOG_INPUT_COUNT;
}

const char *ChannelRange::getRowName( uint8_t row) {

    return InputChannelNames[row];
}

uint8_t ChannelRange::getColCount( uint8_t row) {

    return 1;
}

void ChannelRange::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 0) {
        cell->setInt8( TEXT_INPUT_length +1, CFG->range_pct[row], 3, 0, PERCENT_MAX_LIMIT);
    }
}

void ChannelRange::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 0) {
        CFG->range_pct[row] = cell->getInt8();
    }
}