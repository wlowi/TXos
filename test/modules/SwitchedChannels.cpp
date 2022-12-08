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

#include "SwitchedChannels.h"

extern const char *InputChannelNames[ANALOG_CHANNELS];

SwitchedChannels::SwitchedChannels() : Module( MODULE_SWITCHED_CHANNELS_TYPE, TEXT_MODULE_SWITCHED_CHANNELS) {

    setDefaults();
}

/* From Module */

void SwitchedChannels::run( Controls &controls) {

    channel_t ch;
    switchState_t state;

    for( channel_t sc = 0; sc < SWITCHED_CHANNELS; sc++) {
        if( CFG->sw[sc] != SWITCH_NONE) {
            ch = CFG->ch[sc];
            if( ch < ANALOG_CHANNELS) {
                state = controls.switchGet( CFG->sw[sc]);
                if( state == 0) {
                    controls.inputSet( ch, PCT_TO_CHANNEL( CFG->value[3*sc]));
                } else if( state == 1) {
                    controls.inputSet( ch, PCT_TO_CHANNEL( CFG->value[3*sc+1]));
                } else {
                    controls.inputSet( ch, PCT_TO_CHANNEL( CFG->value[3*sc+2]));
                }
            }
        }
    }
}

void SwitchedChannels::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        for( channel_t ch = 0; ch < SWITCHED_CHANNELS; ch++) {
            CFG->sw[ch] = SWITCH_NONE;
            CFG->ch[ch] = 5+ch;
            CFG->value[3*ch] = -100; /* min  */
            CFG->value[3*ch+1] = 0; /* neutral */
            CFG->value[3*ch+2] = 100; /* max */
        }
    )

    strcpy( switchName, "SC*");
}

/* From TableEditable */

uint8_t SwitchedChannels::getRowCount() {

    return 5 * SWITCHED_CHANNELS;
}

const char *SwitchedChannels::getRowName( uint8_t row) {

    uint8_t ch = (row / 5);

    switchName[2] = '1' +ch;

    return switchName;
}

uint8_t SwitchedChannels::getColCount( uint8_t row) {

    return 2;
}

void SwitchedChannels::getValue( uint8_t row, uint8_t col, Cell *cell) {

    uint8_t r = row % 5;
    uint8_t ch = (row / 5);

    const char *label;

    if( col == 0) {

        if( r == 0) {
            label = TEXT_SW;
        } else if( r == 1) {
            label = TEXT_CH;
        } else if( r == 2) {
            label = TEXT_POS0;
        } else if( r == 3) {
            label = TEXT_POS1;
        } else if( r == 4) {
            label = TEXT_POS2;
        }

        cell->setLabel( 4, label, 4);

    } else if( col == 1) {

        if( r == 0) {
            cell->setSwitch( 9, CFG->sw[ch]);
        } else if( r == 1) {
            cell->setList( 9, InputChannelNames, ANALOG_CHANNELS, CFG->ch[ch]);
        } else if( r == 2) {
            cell->setInt8( 9, CFG->value[3*ch], 4, PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
        } else if( r == 3) {
            cell->setInt8( 9, CFG->value[3*ch+1], 4, PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
        } else if( r == 4) {
            cell->setInt8( 9, CFG->value[3*ch+2], 4, PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
        }

    }
}

void SwitchedChannels::setValue( uint8_t row, uint8_t col, Cell *cell) {

    uint8_t r = row % 5;
    uint8_t ch = (row / 5);

    if( col == 1) {

        if( r == 0) {
            CFG->sw[ch] = cell->getSwitch();
        } else if( r == 1) {
            CFG->ch[ch] = cell->getList();
        } else if( r == 2) {
            CFG->value[3*ch] = cell->getInt8();
        } else if( r == 3) {
            CFG->value[3*ch+1] = cell->getInt8();
        } else if( r == 4) {
            CFG->value[3*ch+2] = cell->getInt8();
        }

    }
}