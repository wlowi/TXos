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

#include "AirBrakes.h"

/* The import/export dictionary.
 * See ImportExport.h
 */
DICTROW( r1, COMM_DATATYPE_UINT8, COMM_FIELD_SWITCH, airBrakes_t, swActive)
DICTROW( r2, COMM_DATATYPE_INT8, COMM_FIELD_PERCENT, airBrakes_t, off_pct)
DICTROW( r3, COMM_DATATYPE_BOOL, COMM_FIELD_REVERSE, airBrakes_t, rev)
DICT( AirBrakes, COMM_SUBPACKET_AIRBRAKES, &r1, &r2, &r3)

AirBrakes::AirBrakes() : Module( MODULE_AIRBRAKES_TYPE, TEXT_MODULE_AIRBRAKES, COMM_SUBPACKET_AIRBRAKES) {

    setDefaults();
}

/* From Module */

COMM_RC_t AirBrakes::exportConfig( ImportExport *exporter, uint8_t *config) const {

    return exporter->runExport( DICT_ptr(AirBrakes), DICTROW_ptr(AirBrakes), config, sizeof(airBrakes_t));
}

COMM_RC_t AirBrakes::importConfig( ImportExport *importer, uint8_t *config) const {

    return importer->runImport( DICT_ptr(AirBrakes), DICTROW_ptr(AirBrakes), config, sizeof(airBrakes_t));
}

void AirBrakes::run( Controls &controls) {

    channelValue_t sourceValue;
    bool active = controls.evalSwitches( CFG->swActive);

    sourceValue = controls.logicalGet( CHANNEL_SPOILER );

    if( active) {
        if( CFG->rev) {
            sourceValue = -sourceValue;
        }
    } else {
        sourceValue = PCT_TO_CHANNEL( CFG->off_pct);
    }

    controls.logicalSet( CHANNEL_SPOILER, sourceValue);
}

void AirBrakes::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        INIT_SWITCH( CFG->swActive);
        CFG->off_pct = 0;
        CFG->rev = 0;

    )
}

/* From TableEditable */

uint8_t AirBrakes::getRowCount() {

    return 3;
}

const char *AirBrakes::getRowName( uint8_t row) {

    if( row == 0) {
        return TEXT_ACTIVE;
    } else if (row == 1) {
        return TEXT_OFF_POS;
    } else if (row == 2) {
        return TEXT_REVERSE;
    }

    return "?";
}

uint8_t AirBrakes::getColCount( uint8_t row) {

    return 1;
}

void AirBrakes::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( row == 0) {
        cell->setSwitchState( 8, CFG->swActive);
    } else if( row == 1) {
        cell->setInt8( 9, CFG->off_pct, 0, PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
    } else if( row == 2) {
        cell->setBool( 10, CFG->rev);
    }
}

void AirBrakes::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( row == 0) {
        CFG->swActive = cell->getSwitchState();
    } else if( row == 1) {
        CFG->off_pct = cell->getInt8();
    } else if( row == 2) {
        CFG->rev = cell->getBool();
    }
}
