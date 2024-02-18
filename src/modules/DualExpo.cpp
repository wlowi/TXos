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

#define EXPO_LOOKUP_TABLE_SIZE   20

const channelValue_t expoLookup[EXPO_LOOKUP_TABLE_SIZE] = {
    3,      //  0- 5%       0 -  62
    6,      //  5-10%      62 - 125
    10,     // 10-15%     125 - 187
    15,     // 15-20%     187 - 250
    21,     // 20-25%     250 - 312
    29,     // 25-30%     312 - 375
    40,     // 30-35%     375 - 437
    54,     // 35-40%     437 - 500
    72,     // 40-45%     500 - 562
    95,     // 45-50%     562 - 625
    124,    // 50-55%     625 - 687
    162,    // 55-60%     687 - 750
    210,    // 60-65%     750 - 812
    272,    // 65-70%     812 - 875
    352,    // 70-75%     875 - 937
    454,    // 75-80%     937 - 1000
    586,    // 80-85%    1000 - 1062
    755,    // 85-90%    1062 - 1125
    972,    // 90-95%    1125 - 1187
    1250   // 95-100%   1187 - 1250
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

    channelValue_t x1;
    channelValue_t x2;
    channelValue_t y1;
    channelValue_t y2;

    channelValue_t dx = PCT_TO_CHANNEL( controls.rangeGet(ch)) / EXPO_LOOKUP_TABLE_SIZE;

    uint8_t interval;
    bool negative = false;

    v = controls.logicalGet( ch);

    if( v < 0) {
        negative = true;
        v = -v;
    }

    interval = (uint8_t)(v / dx); // interval in range 0 - 20

    if( interval >= EXPO_LOOKUP_TABLE_SIZE) {
        interval = EXPO_LOOKUP_TABLE_SIZE-1;
    }

    x1 = interval * dx;
    x2 = (interval+1) * dx;
    
    y1 = (interval == 0) ? 0
        : ((pct * ((long)expoLookup[interval-1]*dx/125)) + ((long)(100 - pct) * x1)) / 100L;
 
    y2 =  ((pct * ((long)expoLookup[interval]*dx/125)) + ((long)(100 - pct) * x2)) / 100L;

    v = (channelValue_t)((long)(v-x1) * (long)(y2-y1) / dx) + y1;
    controls.logicalSet( ch, (channelValue_t)(negative ? -v : v));
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
