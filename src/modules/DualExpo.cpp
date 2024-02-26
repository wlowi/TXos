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

#include "DualExpo.h"
#include "ModuleManager.h"

extern TextUI userInterface;
extern ModuleManager moduleManager;

extern const char* const LogicalChannelNames[LOGICAL_CHANNELS];

/* The table size must be divide the input range -1250 to 1250 without remainder. */
#define EXPO_LOOKUP_TABLE_SIZE   25

const channelValue_t expoLookup[EXPO_LOOKUP_TABLE_SIZE+1] = {
    0,
    2,      //  0-  4%
    4,      //  4-  8%
    7,      //  8- 12%
    10,     // 12- 16%
    14,     // 16- 20%
    20,     // 20- 24%
    26,     // 24- 28%
    33,     // 28- 32%
    43,     // 32- 36%
    54,     // 36- 40%
    68,     // 40- 44%
    84,     // 44- 48%
    106,    // 48- 52%
    131,    // 52- 56%
    162,    // 56- 60%
    200,    // 60- 64%
    246,    // 64- 68%
    302,    // 68- 72%
    370,    // 72- 76%
    454,    // 76- 80%
    557,    // 80- 84%
    682,    // 84- 88%
    835,    // 88- 92%
    1022,   // 92- 96%
    1250    // 96-100%
};

/* The import/export dictionary. 
 * See ImportExport.h
 */
DICTROWA( r1, COMM_DATATYPE_INTARR, COMM_FIELD_RATE_ARRAY, dualExpo_t, rate, DUAL_EXPO_CHANNELS)
DICTROWA( r2, COMM_DATATYPE_INTARR, COMM_FIELD_EXPO_ARRAY, dualExpo_t, expo, DUAL_EXPO_CHANNELS)
DICTP( DualExpo, COMM_SUBPACKET_DUAL_EXPO, COMM_SUBPACKET_DUAL_EXPO_PHASE, &r1, &r2)

DualExpo::DualExpo() : Module( MODULE_DUAL_EXPO_TYPE, TEXT_MODULE_DUAL_EXPO, COMM_SUBPACKET_DUAL_EXPO) {

    setDefaults();
}

/* From Module */

COMM_RC_t DualExpo::exportConfig( ImportExport *exporter, uint8_t *config) const {

    return exporter->runExport( DICT_ptr(DualExpo), DICTROW_ptr(DualExpo), config, sizeof(dualExpo_t));
}

COMM_RC_t DualExpo::importConfig( ImportExport *importer, uint8_t *config) const {

    return importer->runImport( DICT_ptr(DualExpo), DICTROW_ptr(DualExpo), config, sizeof(dualExpo_t));
}

void DualExpo::run( Controls &controls) {

    /* Expo */
    applyExpo( controls, CHANNEL_AILERON, CFG->expo[0]);
    applyExpo( controls, CHANNEL_ELEVATOR, CFG->expo[1]);
    applyExpo( controls, CHANNEL_RUDDER, CFG->expo[2]);

    /* Rate */
    applyRate( controls, CHANNEL_AILERON, CFG->rate[0]);
    applyRate( controls, CHANNEL_ELEVATOR, CFG->rate[1]);
    applyRate( controls, CHANNEL_RUDDER, CFG->rate[2]);
}

void DualExpo::applyRate( Controls &controls, channel_t ch, percent_t pct) {

    long v;
    
    v = controls.logicalGet( ch);

    v = v * pct / PERCENT_MAX;

    controls.logicalSet( ch, (channelValue_t)v);
}

void DualExpo::applyExpo( Controls &controls, channel_t ch, percent_t pct) {

    channelValue_t v;
    channelValue_t w;
    long e;

    channelValue_t x1;
    channelValue_t y1;
    channelValue_t y2;

    channelValue_t dx = CHANNELVALUE_MAX_LIMIT / EXPO_LOOKUP_TABLE_SIZE; // PCT_TO_CHANNEL( controls.rangeGet(ch)) / EXPO_LOOKUP_TABLE_SIZE;

    uint8_t interval;
    bool negative = false;

    v = controls.logicalGet( ch);

    if( v < 0) {
        negative = true;
        v = -v;
    }

    /* interval in range 0 to EXPO_LOOKUP_TABLE_SIZE-1 */
    interval = (uint8_t)(v / dx);

    if( interval >= EXPO_LOOKUP_TABLE_SIZE) {
        interval = EXPO_LOOKUP_TABLE_SIZE -1;
    }

    /* x start of interval */
    x1 = interval * dx;

    /* y start and end of interval */
    y1 = expoLookup[interval];
    y2 = expoLookup[interval+1];

    /* interpolate exponential value from table */
    e = ((long)(v-x1) * (long)(y2-y1) / dx) + y1;
    w = ((pct * e) + (100 - pct) * (long)v) / 100;

    controls.logicalSet( ch, (channelValue_t)(negative ? -w : w));
}

void DualExpo::setDefaults() {

    INIT_PHASED_CONFIGURATION(

        for( channel_t ch = 0; ch < DUAL_EXPO_CHANNELS; ch++) {
            CFG->rate[ch] = PERCENT_MAX; /* 100% Rate */
            CFG->expo[ch] = 0; /* Expo */
        }
    )

    switchPhase( 0);
    postRefresh = false;
}

void DualExpo::switchPhase(phase_t ph) {

    LOGV("DualExpo::switchPhase: new phase %d\n", ph);

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
        return LogicalChannelNames[CHANNEL_AILERON];
    } else if( row < 5) {
        return LogicalChannelNames[CHANNEL_ELEVATOR];
    } else {
        return LogicalChannelNames[CHANNEL_RUDDER];
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
            if( row % 2) {
                cell->setInt8( 9, CFG->rate[(row-1) / 2], 0, 0, PERCENT_MAX);
            } else {
                cell->setInt8( 9, CFG->expo[(row-1) / 2], 0, 0, PERCENT_MAX);
            }
        }
    }
}

void DualExpo::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 0) {

        /* no op */

    } else if( row > 0) {
        if( row % 2) {
            CFG->rate[(row-1) / 2] = cell->getInt8();
        } else {
            CFG->expo[(row-1) / 2] = cell->getInt8();
        }
    }
}
