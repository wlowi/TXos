/*
  TXos. A remote control transmitter OS.

  MIT License

  Copyright (c) 2023 wlowi

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include "SwitchedChannels.h"

extern const char* const InputChannelNames[INPUT_CHANNELS];

SwitchedChannels::SwitchedChannels() : Module( MODULE_SWITCHED_CHANNELS_TYPE, TEXT_MODULE_SWITCHED_CHANNELS) {

    setDefaults();
}

/* From Module */

void SwitchedChannels::run( Controls &controls) {

    channel_t ch;
    switchState_t state;

    for( channel_t sc = 0; sc < SWITCHED_CHANNELS; sc++) {
        if( IS_SWITCH_USED( CFG->sw[sc])) {
            ch = ANALOG_CHANNELS - SWITCHED_CHANNELS +sc;
            state = controls.switchGet( CFG->sw[sc]);
            if( state == 0) {
                controls.inputSet( ch, PCT_TO_CHANNEL( CFG->state0_pct[sc]));
            } else if( state == 1) {
                controls.inputSet( ch, PCT_TO_CHANNEL( CFG->state1_pct[sc]));
            } else {
                controls.inputSet( ch, PCT_TO_CHANNEL( CFG->state2_pct[sc]));
            }
        }
    }
}

void SwitchedChannels::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        for( channel_t sc = 0; sc < SWITCHED_CHANNELS; sc++) {
            INIT_SWITCH( CFG->sw[sc]);
            CFG->state0_pct[sc] = -100; /* min  */
            CFG->state1_pct[sc] = 0;    /* neutral */
            CFG->state2_pct[sc] = 100;  /* max */
        }
    )
}

/* From TableEditable */

uint8_t SwitchedChannels::getRowCount() {

    return 4 * SWITCHED_CHANNELS;
}

const char *SwitchedChannels::getRowName( uint8_t row) {

    if( (row % 4) == 0) {
        uint8_t ch = (row / 4);
        return InputChannelNames[ANALOG_CHANNELS - SWITCHED_CHANNELS +ch];
    } else {
        return TEXT_MSG_NONE;
    }
}

uint8_t SwitchedChannels::getColCount( uint8_t row) {

    return 2;
}

void SwitchedChannels::getValue( uint8_t row, uint8_t col, Cell *cell) {

    uint8_t r = row % 4;
    uint8_t ch = (row / 4);

    const char *label;

    if( col == 0) {

        if( r == 0) {
            label = TEXT_SW;
        } else if( r == 1) {
            label = TEXT_POS0;
        } else if( r == 2) {
            label = TEXT_POS1;
        } else {
            label = TEXT_POS2;
        }

        cell->setLabel( 4, label, 4);

    } else if( col == 1) {

        if( r == 0) {
            cell->setSwitch( 9, CFG->sw[ch]);
        } else if( r == 1) {
            cell->setInt8( 9, CFG->state0_pct[ch], 4, PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
        } else if( r == 2) {
            cell->setInt8( 9, CFG->state1_pct[ch], 4, PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
        } else if( r == 3) {
            cell->setInt8( 9, CFG->state2_pct[ch], 4, PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
        }
    }
}

void SwitchedChannels::setValue( uint8_t row, uint8_t col, Cell *cell) {

    uint8_t r = row % 4;
    uint8_t ch = (row / 4);

    if( col == 1) {

        if( r == 0) {
            CFG->sw[ch] = cell->getSwitch();
        } else if( r == 1) {
            CFG->state0_pct[ch] = cell->getInt8();
        } else if( r == 2) {
            CFG->state1_pct[ch] = cell->getInt8();
        } else if( r == 3) {
            CFG->state2_pct[ch] = cell->getInt8();
        }
    }
}
