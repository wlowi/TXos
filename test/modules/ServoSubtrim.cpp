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

ServoSubtrim::ServoSubtrim() : Module( MODULE_SERVO_SUBTRIM_TYPE, TEXT_MODULE_SERVO_SUBTRIM) {

    setDefaults();
}

void ServoSubtrim::run( Controls &controls) {

    for( channel_t ch = 0; ch < CHANNELS; ch++) {
        controls.analogSet( ch, controls.analogGet( ch) + PCT_TO_CHANNEL( CFG->trim_pct[ch]));
    }
}

void ServoSubtrim::setDefaults() {

    INIT_PHASED_CONFIGURATION(

        for( channel_t ch = 0; ch < CHANNELS; ch++) {
            CFG->trim_pct[ch] = 0;
        }

    )
}

/* From Module */

moduleSize_t ServoSubtrim::getConfigSize() {

    return (moduleSize_t)sizeof( configuration);
}

uint8_t *ServoSubtrim::getConfig() {

    return (uint8_t*)&configuration;
}

void ServoSubtrim::switchPhase(phase_t ph) {

    LOGV("ServoSubtrim::switchPhase: new phase %d\n", ph);
    SWITCH_PHASE( ph);
}

/* From TableEditable */

uint8_t ServoSubtrim::getRowCount() {

    return CHANNELS;
}

const char *ServoSubtrim::getRowName( uint8_t row) {

    return ChannelNames[row];
}

uint8_t ServoSubtrim::getColCount( uint8_t row) {

    return 1;
}

void ServoSubtrim::getValue( uint8_t row, uint8_t col, Cell *cell) {

    cell->setInt8( 4, CFG->trim_pct[row], PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
}

void ServoSubtrim::setValue( uint8_t row, uint8_t col, Cell *cell) {

    CFG->trim_pct[row] = cell->getInt8();
}
