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

#include "PhasesTrim.h"
#include "ModuleManager.h"
#include "UserInterface.h"

extern UserInterface userInterface;
extern ModuleManager moduleManager;

extern const char *LogicalChannelNames[LOGICAL_CHANNELS];

PhasesTrim::PhasesTrim() : Module( MODULE_PHASES_TRIM_TYPE, TEXT_MODULE_PHASES_TRIM) {

    setDefaults();
}

/* From Module */

void PhasesTrim::run( Controls &controls) {

    uint8_t ch;
    channelValue_t offset;

    for( channel_t pc = 0; pc < PHASED_TRIM_CHANNELS; pc++) {

        if( pc == 0) {
            ch = CHANNEL_AILERON;
        } else if( pc == 1) {
            ch = CHANNEL_ELEVATOR;
        } else if( pc == 2) {
            ch = CHANNEL_RUDDER;
        } else {
            ch = CHANNEL_FLAP;
        }

        offset = PCT_TO_CHANNEL( CFG->trim_pct[pc]);

        controls.logicalSet( ch, controls.logicalGet( ch) + offset);

        if( ch == CHANNEL_AILERON) {
            controls.logicalSet( CHANNEL_AILERON2, controls.logicalGet( CHANNEL_AILERON2) + offset);
        } else if( ch == CHANNEL_FLAP) {
            controls.logicalSet( CHANNEL_FLAP2, controls.logicalGet( CHANNEL_FLAP2) + offset);
        }
    }
}

void PhasesTrim::setDefaults() {

    INIT_PHASED_CONFIGURATION(

        for( channel_t ch = 0; ch < PHASED_TRIM_CHANNELS; ch++) {
            CFG->trim_pct[ch] = 0;
        }
    )

    switchPhase( 0);
    postRefresh = false;
}

void PhasesTrim::switchPhase(phase_t ph) {

    LOGV("PhasesTrim::switchPhase: new phase %d\n", ph);

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

bool PhasesTrim::needsRefresh() { 
    
    if( postRefresh) {
        postRefresh = false;
        return true;
    }

    return false;
}

uint8_t PhasesTrim::getRowCount() {

    return PHASED_TRIM_CHANNELS +1;
}

const char *PhasesTrim::getRowName( uint8_t row) {

    if( row == 0) {
        return TEXT_PHASE;
    } else if( row == 1) {
        return LogicalChannelNames[CHANNEL_AILERON];
    } else if( row == 2) {
        return LogicalChannelNames[CHANNEL_ELEVATOR];
    } else if( row == 3) {
        return LogicalChannelNames[CHANNEL_RUDDER];
    } else {
        return LogicalChannelNames[CHANNEL_FLAP];
    }
}

bool PhasesTrim::isRowEditable( uint8_t row) {

    if( row == 0) {
        return false;
    }

    return true;
}

uint8_t PhasesTrim::getColCount( uint8_t row) {

    return 1;
}

void PhasesTrim::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 0) {
        if( row == 0) {
            cell->setLabel( 6, phaseName, TEXT_PHASE_NAME_length);
        } else {
            cell->setInt8( 9, CFG->trim_pct[row-1], 0, PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
        }
    }
}

void PhasesTrim::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 0) {
        if( row > 0) {
            CFG->trim_pct[row-1] = cell->getInt8();    
        }
    }
}