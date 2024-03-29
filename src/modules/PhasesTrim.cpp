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

#include "PhasesTrim.h"
#include "ModuleManager.h"

extern TextUI userInterface;
extern ModuleManager moduleManager;

extern const char* const LogicalChannelNames[LOGICAL_CHANNELS];

/* The import/export dictionary. 
 * See ImportExport.h
 */
DICTROWA( r1, COMM_DATATYPE_INTARR, COMM_FIELD_PERCENT_ARRAY, phasesTrim_t, trim_pct, PHASED_TRIM_CHANNELS)
DICTP( PhasesTrim, COMM_SUBPACKET_PHASES_TRIM, COMM_SUBPACKET_PHASES_TRIM_PHASE, &r1)

PhasesTrim::PhasesTrim() : Module( MODULE_PHASES_TRIM_TYPE, TEXT_MODULE_PHASES_TRIM, COMM_SUBPACKET_PHASES_TRIM) {

    setDefaults();
}

/* From Module */

COMM_RC_t PhasesTrim::exportConfig( ImportExport *exporter, uint8_t *config) const {

    return exporter->runExport( DICT_ptr(PhasesTrim), DICTROW_ptr(PhasesTrim), config, sizeof(phasesTrim_t));
}

COMM_RC_t PhasesTrim::importConfig( ImportExport *importer, uint8_t *config) const {

    return importer->runImport( DICT_ptr(PhasesTrim), DICTROW_ptr(PhasesTrim), config, sizeof(phasesTrim_t));
}

void PhasesTrim::run( Controls &controls) {

    uint8_t ch;
    channelValue_t offset;

    for( channel_t pc = 0; pc < PHASED_TRIM_CHANNELS; pc++) {

        if( pc == 0) {
            ch = CHANNEL_AILERON;
        } else if( pc == 1) {
            ch = CHANNEL_ELEVATOR;
        } else if( pc == 2) {
            ch = CHANNEL_FLAP;
        } else {
            ch = CHANNEL_SPOILER;
        }

        offset = PCT_TO_CHANNEL( CFG->trim_pct[pc]);

        controls.logicalSet( ch, controls.logicalGet( ch) + offset);

        if( ch == CHANNEL_AILERON) {
            controls.logicalSet( CHANNEL_AILERON2, controls.logicalGet( CHANNEL_AILERON2) + offset);
        } else if( ch == CHANNEL_FLAP) {
            controls.logicalSet( CHANNEL_FLAP2, controls.logicalGet( CHANNEL_FLAP2) + offset);
        } else if( ch == CHANNEL_SPOILER) {
            controls.logicalSet( CHANNEL_SPOILER2, controls.logicalGet( CHANNEL_SPOILER2) + offset);
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

    userInterface.cancelEdit( this);

    SWITCH_PHASE( ph);
    
    Phases *phases = (Phases*)moduleManager.getModuleByType( MODULE_SET_MODEL, MODULE_PHASES_TYPE);
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
        return LogicalChannelNames[CHANNEL_FLAP];
    } else {
        return LogicalChannelNames[CHANNEL_SPOILER];
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
            cell->setInt8( TEXT_CONTROL_length +1, CFG->trim_pct[row-1], 0, PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
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
