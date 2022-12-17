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

#include "ServoRemap.h"

extern const char *LogicalChannelNames[LOGICAL_CHANNELS];
extern const char *OutputChannelNames[PPM_CHANNELS];

ServoRemap::ServoRemap() : Module( MODULE_SERVO_REMAP_TYPE, TEXT_MODULE_SERVO_REMAP) {

    setDefaults();
}

/* From Module */

void ServoRemap::run( Controls &controls) {

    for( channel_t ch = 0; ch < PPM_CHANNELS; ch++) {
        controls.outputSet( ch, controls.logicalGet( CFG->source[ch]));
    }
}

void ServoRemap::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        for( channel_t ch = 0; ch < PPM_CHANNELS; ch++) {
            if( ch == 0) {
                CFG->source[ch] = CHANNEL_THROTTLE;
            } else if( ch == 1) {
                CFG->source[ch] = CHANNEL_AILERON;
            } else if( ch == 2) {
                CFG->source[ch] = CHANNEL_ELEVATOR;
            } else if( ch == 3) {
                CFG->source[ch] = CHANNEL_RUDDER;
            } else if( ch == 4) {
                CFG->source[ch] = CHANNEL_AILERON2;
            } else if( ch == 5) {
                CFG->source[ch] = CHANNEL_FLAP;
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

uint8_t ServoRemap::getRowCount() {

    return PPM_CHANNELS;
}

const char *ServoRemap::getRowName( uint8_t row) {

    return OutputChannelNames[row];
}

uint8_t ServoRemap::getColCount( uint8_t row) {

    return 2;
}

void ServoRemap::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 0) {
        cell->setLabel( TEXT_OUT_length +1, TEXT_ARROW_LEFT, 1);
    } else if( col == 1) {
        cell->setList( TEXT_OUT_length +3, LogicalChannelNames, LOGICAL_CHANNELS, CFG->source[row]);
    }
}

void ServoRemap::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 1) {
        CFG->source[row] = cell->getList();
    }
}