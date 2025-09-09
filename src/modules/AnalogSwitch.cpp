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

#include "AnalogSwitch.h"

extern Controls controls;
extern const char* const InputChannelNames[INPUT_CHANNELS];

/* The import/export dictionary.
 * See ImportExport.h
 */
DICTROWA( r1, COMM_DATATYPE_UINTARR, COMM_FIELD_CHANNEL_ARRAY, analogSwitch_t, source, CHANNEL_SWITCHES)
DICTROWA( r2, COMM_DATATYPE_INTARR, COMM_FIELD_PERCENT_ARRAY, analogSwitch_t, trigger_pct, CHANNEL_SWITCHES)
DICT( AnalogSwitch, COMM_SUBPACKET_ANALOG_SWITCH, &r1, &r2)

AnalogSwitch::AnalogSwitch() : Module( MODULE_ANALOG_SWITCH_TYPE, TEXT_MODULE_ANALOG_SWITCH, COMM_SUBPACKET_ANALOG_SWITCH) {

    setDefaults();
}

/* From Module */

COMM_RC_t AnalogSwitch::exportConfig( ImportExport *exporter, uint8_t *config) const {

    return exporter->runExport( DICT_ptr(AnalogSwitch), DICTROW_ptr(AnalogSwitch), config, sizeof(analogSwitch_t));
}

COMM_RC_t AnalogSwitch::importConfig( ImportExport *importer, uint8_t *config) const {

    return importer->runImport( DICT_ptr(AnalogSwitch), DICTROW_ptr(AnalogSwitch), config, sizeof(analogSwitch_t));
}

void AnalogSwitch::run( Controls &controls) {

    long v;
    switchState_t state;

    for( switch_t sw = 0; sw < CHANNEL_SWITCHES; sw++) {
        /* Convert v from range -125 - 125 to -100 - 100 before compare */
        v = (long)controls.inputGet(CFG->source[sw]) * PERCENT_MAX / PERCENT_MAX_LIMIT;

        state = ((channelValue_t)v >= PCT_TO_CHANNEL(CFG->trigger_pct[sw])) ? SW_STATE_1 : SW_STATE_0;
        controls.switchSet( CHANNEL_SWITCHES_FIRST_IDX + sw, state);
    }
}

void AnalogSwitch::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        for( switch_t sw = 0; sw < CHANNEL_SWITCHES; sw++) {
            CFG->source[sw] = 0;
            CFG->trigger_pct[sw] = 0; /* neutral */
        }
    )
}

/* From TableEditable */

uint8_t AnalogSwitch::getRowCount() {

    return 2 * CHANNEL_SWITCHES;
}

const char *AnalogSwitch::getRowName( uint8_t row) {

    if( row % 2) {
        return TEXT_MSG_NONE;
    } else {
        controls.copySwitchName( switchName, (switch_t)(row/2) + CHANNEL_SWITCHES_FIRST_IDX);
        return switchName;
    }
}

uint8_t AnalogSwitch::getColCount( uint8_t row) {

    return 1;
}

void AnalogSwitch::getValue( uint8_t row, uint8_t col, Cell *cell) {

    uint8_t sw = row / 2;
    uint8_t r = (row % 2);

    if( r == 0) {
        cell->setList( 6, InputChannelNames, ANALOG_CHANNELS, CFG->source[sw]);
    } else if( r == 1) {
        cell->setInt8( 6, CFG->trigger_pct[sw], 0, PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
    }
}

void AnalogSwitch::setValue( uint8_t row, uint8_t col, Cell *cell) {

    uint8_t sw = row / 2;
    uint8_t r = (row % 2);

    if( r == 0) {
        CFG->source[sw] = cell->getList();
    } else if( r == 1) {
        CFG->trigger_pct[sw] = cell->getInt8();
    }
}
