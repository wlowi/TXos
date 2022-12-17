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

#include "ServoSubtrim.h"

extern const char *OutputChannelNames[PPM_CHANNELS];

ServoSubtrim::ServoSubtrim() : Module( MODULE_SERVO_SUBTRIM_TYPE, TEXT_MODULE_SERVO_SUBTRIM) {

    setDefaults();
}

/* From Module */

void ServoSubtrim::run( Controls &controls) {

    for( channel_t ch = 0; ch < PPM_CHANNELS; ch++) {
        controls.outputSet( ch, controls.outputGet( ch) + PCT_TO_CHANNEL( CFG->trim_pct[ch]));
    }
}

void ServoSubtrim::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        for( channel_t ch = 0; ch < PPM_CHANNELS; ch++) {
            CFG->trim_pct[ch] = 0;
        }

    )
}

/* From TableEditable */

uint8_t ServoSubtrim::getRowCount() {

    return PPM_CHANNELS;
}

const char *ServoSubtrim::getRowName( uint8_t row) {

    return OutputChannelNames[row];
}

uint8_t ServoSubtrim::getColCount( uint8_t row) {

    return 1;
}

void ServoSubtrim::getValue( uint8_t row, uint8_t col, Cell *cell) {

    cell->setInt8( TEXT_OUT_length +1, CFG->trim_pct[row], 0, PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
}

void ServoSubtrim::setValue( uint8_t row, uint8_t col, Cell *cell) {

    CFG->trim_pct[row] = cell->getInt8();
}
