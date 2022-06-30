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

#include "ServoReverse.h"

ServoReverse::ServoReverse() : Module( MODULE_SERVO_REVERSE_TYPE, TEXT_MODULE_SERVO_REVERSE) {

    setDefaults();
}

void ServoReverse::run( channelSet_t &channels) {

    for( channel_t ch = 0; ch < CHANNELS; ch++) {
        if( IS_BIT_SET( cfg.revBits, ch)) {
            channels.analogChannel[ch] = -channels.analogChannel[ch];
        }
    }
}

void ServoReverse::setDefaults() {

    cfg.revBits = 0;
}

/* From Module */

moduleSize_t ServoReverse::getConfigSize() {

    return (moduleSize_t)sizeof( cfg);
}

uint8_t *ServoReverse::getConfig() {

    return (uint8_t*)&cfg;
}

/* From TableEditable */

uint8_t ServoReverse::getItemCount() {

    return CHANNELS;
}

const char *ServoReverse::getItemName( uint8_t row) {

    return ChannelNames[row];
}

uint8_t ServoReverse::getValueCount() {

    return 1;
}

void ServoReverse::getValue( uint8_t row, uint8_t col, Cell *cell) {

    cell->setBool( IS_BIT_SET( cfg.revBits, row));
}

void ServoReverse::setValue( uint8_t row, uint8_t col, Cell *cell) {
    
    if( cell->getBool()) {
        BIT_SET( cfg.revBits, row);
    } else {
        BIT_CLEAR( cfg.revBits, row);
    }
}
