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

extern const char* const LogicalChannelNames[LOGICAL_CHANNELS];

ChannelDelay::ChannelDelay() : Module( MODULE_CHANNEL_DELAY_TYPE, TEXT_MODULE_CHANNEL_DELAY) {

    setDefaults();
}

/* From Module */

void ChannelDelay::run( Controls &controls) {

#define SCALING_F (10)

    int16_t target10;   // scaled by 10 for increased precision
    int16_t limit10;    // same here
    int16_t delay_msec;

    for( uint8_t mix = 0; mix < MIX_CHANNELS; mix++) {

        target10 = controls.logicalGet( mix) * SCALING_F;

        if( CFG->posDelay_sec[mix] == 0 && CFG->negDelay_sec[mix] == 0) {
            lastChannelValue10[mix] = target10;
            continue;
        }

        if( lastChannelValue10[mix] < target10) {
            if( CFG->posDelay_sec[mix] > 0) {
                // posDelay_sec is a scaled float in 1/10 sec resolution.
                delay_msec = CFG->posDelay_sec[mix] * 100;
                limit10 = ((CHANNELVALUE_MAX - CHANNELVALUE_MIN) * SCALING_F) / (delay_msec / PPM_FRAME_TIME_MSEC);
                lastChannelValue10[mix] += limit10;
                if( lastChannelValue10[mix] > target10) {
                    lastChannelValue10[mix] = target10;
                }
            } else {
               lastChannelValue10[mix] = target10;
            }

        } else if( lastChannelValue10[mix] > target10) {
            if( CFG->negDelay_sec[mix] > 0) {
                delay_msec = CFG->negDelay_sec[mix] * 100;
                limit10 = ((CHANNELVALUE_MAX - CHANNELVALUE_MIN) * SCALING_F) / (delay_msec / PPM_FRAME_TIME_MSEC);
                lastChannelValue10[mix] -= limit10;
                if( lastChannelValue10[mix] < target10) {
                    lastChannelValue10[mix] = target10;
                }
            } else {
                lastChannelValue10[mix] = target10;
            }

        }

        controls.logicalSet( mix, (channelValue_t)(lastChannelValue10[mix] / SCALING_F));
    }
}

void ChannelDelay::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        for( channel_t ch = 0; ch < MIX_CHANNELS; ch++) {
            CFG->posDelay_sec[ch] = 0;
            CFG->negDelay_sec[ch] = 0;
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

    return LogicalChannelNames[row];
}

uint8_t ChannelDelay::getColCount( uint8_t row) {

    return 2;
}

void ChannelDelay::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 0) {
        cell->setFloat1( 4, CFG->posDelay_sec[row], 4, 0, 100 /* 10.0 sec */);
    } else if( col == 1) {
        cell->setFloat1( 9, CFG->negDelay_sec[row], 4, 0, 100 /* 10.0 sec */);
    }
}

void ChannelDelay::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 0) {
        CFG->posDelay_sec[row] = cell->getFloat1();
    } else if( col == 1) {
        CFG->negDelay_sec[row] = cell->getFloat1();
    }
}
