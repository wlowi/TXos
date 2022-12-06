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

#include "DualExpo.h"
#include "ModuleManager.h"
#include "UserInterface.h"

extern UserInterface userInterface;
extern ModuleManager moduleManager;

extern const char *ChannelNames[CHANNELS];

#define EXPO_LOOKUP_TABLE_SIZE   10

const channelValue_t expoLookup[EXPO_LOOKUP_TABLE_SIZE] = {
    0,      // 0-10%       0 - 125
    7,      // 10-10%    126 - 250
    17,     // 20-30%    251 - 375
    35,     // 30-40%    376 - 500
    64,     // 40-50%    501 - 625
    111,    // 50-60%    626 - 750
    190,    // 60-70%    751 - 875
    320,    // 70-80%    876 - 1000
    534,    // 80-90%   1001 - 1125
    886     // 90-100%  1126 - 1200
};

DualExpo::DualExpo() : Module( MODULE_DUAL_EXPO_TYPE, TEXT_MODULE_DUAL_EXPO) {

    setDefaults();
}

/* From Module */

void DualExpo::run( Controls &controls) {

    /* Expo */
    applyExpo( controls, CHANNEL_AILERON, CFG->value[1]);
    applyExpo( controls, CHANNEL_ELEVATOR, CFG->value[3]);
    applyExpo( controls, CHANNEL_RUDDER, CFG->value[5]);

    /* Rate */
    applyRate( controls, CHANNEL_AILERON, CFG->value[0]);
    applyRate( controls, CHANNEL_ELEVATOR, CFG->value[2]);
    applyRate( controls, CHANNEL_RUDDER, CFG->value[4]);
}

void DualExpo::applyRate( Controls &controls, channel_t ch, percent_t pct) {

    long v;
    
    v = controls.analogGet( ch);

    v = v * pct / PERCENT_MAX_LIMIT;

    controls.analogSet( ch, (channelValue_t)v);
}

void DualExpo::applyExpo( Controls &controls, channel_t ch, percent_t pct) {

    long v, w;
    uint8_t lookupIdx;

    v = controls.analogGet( ch);

    for( lookupIdx = 0; lookupIdx < (EXPO_LOOKUP_TABLE_SIZE-1); lookupIdx++) {
        if( v < (CHANNELVALUE_MAX_LIMIT / 10) * (lookupIdx +1)) {
            break;
        }
    }

    w = (((long)pct * expoLookup[lookupIdx]) + ((100 - pct) * v)) / 100L;
    if( ch == CHANNEL_AILERON)
        LOGV("in: %ld pct: %d lookup: %d out:%ld\n", v, pct, lookupIdx, w);

    controls.analogSet( ch, (channelValue_t)w);
}

void DualExpo::setDefaults() {

    INIT_PHASED_CONFIGURATION(

        for( channel_t ch = 0; ch < DUAL_EXPO_CHANNELS; ch += 2) {
            CFG->value[ch] = 100; /* Rate */
            CFG->value[ch+1] = 0; /* Expo */
        }
    )

    switchPhase( 0);
    postRefresh = false;
}

void DualExpo::switchPhase(phase_t ph) {

    LOGV("DualExpo::switchPhase: new phase %d\n", ph);

    userInterface.cancelEdit();

    SWITCH_PHASE( ph);
    
    Phases *phases = (Phases*)moduleManager.getModelMenu()->getModuleByType( MODULE_PHASES_TYPE);
    if( phases) {
        phaseName = phases->getPhaseName();
    } else {
        phaseName = TEXT_NOPHASE;
    }
    
    postRefresh = true;
}

/* From TableEditable */

bool DualExpo::needsRefresh() { 
    
    if( postRefresh) {
        postRefresh = false;
        return true;
    }

    return false;
}

uint8_t DualExpo::getRowCount() {

    return 2 * DUAL_EXPO_CHANNELS +1;
}

const char *DualExpo::getRowName( uint8_t row) {

    if( row == 0) {
        return TEXT_PHASE;
    } else if( row < 3) {
        return ChannelNames[CHANNEL_AILERON];
    } else if( row < 5) {
        return ChannelNames[CHANNEL_ELEVATOR];
    } else {
        return ChannelNames[CHANNEL_RUDDER];
    }
}

bool DualExpo::isRowEditable( uint8_t row) {

    if( row == 0) {
        return false;
    }

    return true;
}

bool DualExpo::isColEditable( uint8_t row, uint8_t col) {

    if( col == 1) {
        return true;
    }

    return false;
}

uint8_t DualExpo::getColCount( uint8_t row) {

    if( row == 0) {
        return 1;
    }

    return 2;
}

void DualExpo::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 0) {
        if( row == 0) {
            cell->setLabel( 6, phaseName, TEXT_PHASE_NAME_length);
        } else {
            if( row % 2) {
                cell->setLabel( 4, TEXT_RATE, 4);
            } else {
                cell->setLabel( 4, TEXT_EXPO, 4);
            }
        }
    } else {
        if( row > 0) {
            cell->setInt8( 9, CFG->value[row-1], 0, row % 2 ? PERCENT_MAX_LIMIT : 100);
        }
    }
}

void DualExpo::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 0) {

        /* no op */

    } else if( row > 0) {

        CFG->value[row-1] = cell->getInt8();    
    }
}