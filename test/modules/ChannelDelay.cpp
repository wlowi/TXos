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

#include "ChannelDelay.h"

extern const char* const MixChannelNames[MIX_CHANNELS];
extern const channel_t MixChannelMap[MIX_CHANNELS];

ChannelDelay::ChannelDelay() : Module( MODULE_CHANNEL_DELAY_TYPE, TEXT_MODULE_CHANNEL_DELAY) {

    setDefaults();
}

/* From Module */

void ChannelDelay::run( Controls &controls) {

    channel_t ch;
    int16_t target10; // multiplied by 10 for increased precision
    int16_t limit10;  // same here
    int16_t delayMs;

    for( uint8_t mix = 0; mix < MIX_CHANNELS; mix++) {

        ch = MIX_TO_CHANNEL( mix);
        target10 = controls.logicalGet( ch) * 10;

        if( CFG->posDelaySec[mix] == 0 && CFG->negDelaySec[mix] == 0) {
            lastChannelValue10[mix] = target10;
            continue;
        }

        if( lastChannelValue10[mix] < target10) {
            if( CFG->posDelaySec[mix] > 0) {
                delayMs = CFG->posDelaySec[mix] * 100;
                limit10 = ((CHANNELVALUE_MAX - CHANNELVALUE_MIN) * 10) / (delayMs / 22);
                lastChannelValue10[mix] += limit10;
                if( lastChannelValue10[mix] > target10) {
                    lastChannelValue10[mix] = target10;
                }
            } else {
               lastChannelValue10[mix] = target10;
            }

        } else if( lastChannelValue10[mix] > target10) {
            if( CFG->negDelaySec[mix] > 0) {
                delayMs = CFG->negDelaySec[mix] * 100;
                limit10 = ((CHANNELVALUE_MAX - CHANNELVALUE_MIN) * 10) / (delayMs / 22);
                lastChannelValue10[mix] -= limit10;
                if( lastChannelValue10[mix] < target10) {
                    lastChannelValue10[mix] = target10;
                }
            } else {
                lastChannelValue10[mix] = target10;
            }

        }

        controls.logicalSet( ch, (channelValue_t)(lastChannelValue10[mix] / 10));
        
    }
}

void ChannelDelay::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        for( channel_t ch = 0; ch < MIX_CHANNELS; ch++) {
            CFG->posDelaySec[ch] = 0;
            CFG->negDelaySec[ch] = 0;
        }

    )

    for( channel_t ch = 0; ch < MIX_CHANNELS; ch++) {
        lastChannelValue10[ch] = 0;
    }
}

/* From TableEditable */

uint8_t ChannelDelay::getRowCount() {

    return MIX_CHANNELS;
}

const char *ChannelDelay::getRowName( uint8_t row) {

    return MixChannelNames[row];
}

uint8_t ChannelDelay::getColCount( uint8_t row) {

    return 2;
}

void ChannelDelay::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 0) {
        cell->setFloat1( 4, CFG->posDelaySec[row], 4, 0, 100 /* 10.0 sec */);
    } else {
        cell->setFloat1( 9, CFG->negDelaySec[row], 4, 0, 100 /* 10.0 sec */);
        //cell->setLabel(10, TEXT_SEC, 3);
    }
}

void ChannelDelay::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 0) {
        CFG->posDelaySec[row] = cell->getFloat1();
    } else {
        CFG->negDelaySec[row] = cell->getFloat1();
    }
}
