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

extern const char *OutputChannelNames[PPM_CHANNELS];

ServoReverse::ServoReverse() : Module( MODULE_SERVO_REVERSE_TYPE, TEXT_MODULE_SERVO_REVERSE) {

    setDefaults();
}

/* From Module */

void ServoReverse::run( Controls &controls) {

    for( channel_t ch = 0; ch < PPM_CHANNELS; ch++) {
        if( IS_BIT_SET( CFG->revBits, ch)) {
            controls.outputSet( ch, -controls.outputGet(ch));
        }
    }
}

void ServoReverse::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        CFG->revBits = 0;

    )
}

/* From TableEditable */

uint8_t ServoReverse::getRowCount() {

    return PPM_CHANNELS;
}

const char *ServoReverse::getRowName( uint8_t row) {

    return OutputChannelNames[row];
}

uint8_t ServoReverse::getColCount( uint8_t row) {

    return 1;
}

void ServoReverse::getValue( uint8_t row, uint8_t col, Cell *cell) {

    cell->setBool( TEXT_OUT_length +1, IS_BIT_SET( CFG->revBits, row));
}

void ServoReverse::setValue( uint8_t row, uint8_t col, Cell *cell) {
    
    if( cell->getBool()) {
        BIT_SET( CFG->revBits, row);
    } else {
        BIT_CLEAR( CFG->revBits, row);
    }
}
