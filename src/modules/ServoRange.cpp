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

#include "ServoRange.h"

extern const char* const OutputChannelNames[PPM_CHANNELS];

/* The import/export dictionary.
 * See ImportExport.h
 */
DICTROWA( r1, COMM_DATATYPE_INTARR, COMM_FIELD_PERCENT_ARRAY, servoRange_t, posRange_pct, PPM_CHANNELS)
DICTROWA( r2, COMM_DATATYPE_INTARR, COMM_FIELD_NEG_PERCENT_ARRAY, servoRange_t, negRange_pct, PPM_CHANNELS)
DICT( ServoRange, COMM_SUBPACKET_SERVO_RANGE, &r1, &r2)

ServoRange::ServoRange() : Module( MODULE_SERVO_RANGE_TYPE, TEXT_MODULE_SERVO_RANGE, COMM_SUBPACKET_SERVO_RANGE) {

    setDefaults();
}

COMM_RC_t ServoRange::exportConfig( ImportExport *exporter, uint8_t *config) const {

    return exporter->runExport( DICT_ptr(ServoRange), DICTROW_ptr(ServoRange), config, sizeof(servoRange_t));
}

COMM_RC_t ServoRange::importConfig( ImportExport *importer, uint8_t *config) const {

    return importer->runImport( DICT_ptr(ServoRange), DICTROW_ptr(ServoRange), config, sizeof(servoRange_t));
}

/* From Module */

void ServoRange::run( Controls &controls) {

    long v;

    for( channel_t ch = 0; ch < PPM_CHANNELS; ch++) {

        v = controls.outputGet( ch);

        if( v > 0) {
            v = v * CFG->posRange_pct[ch] / PERCENT_MAX;
        } else if( v < 0) {
            v = v * CFG->negRange_pct[ch] / PERCENT_MIN;
        }
        controls.outputSet( ch, (channelValue_t)v);
    }
}

void ServoRange::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        for( channel_t ch = 0; ch < PPM_CHANNELS; ch++) {
            CFG->posRange_pct[ch] = PERCENT_MAX;
            CFG->negRange_pct[ch] = PERCENT_MIN;
        }
    )
}

/* From TableEditable */

uint8_t ServoRange::getRowCount() {

    return PPM_CHANNELS;
}

const char *ServoRange::getRowName( uint8_t row) {

    return OutputChannelNames[row];
}

uint8_t ServoRange::getColCount( uint8_t row) {

    return 2;
}

void ServoRange::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 0) {
        cell->setInt8( 4, CFG->negRange_pct[row], 4, PERCENT_MIN_LIMIT, 0);
    } else {
        cell->setInt8( 9, CFG->posRange_pct[row], 4, 0, PERCENT_MAX_LIMIT);
    }
}

void ServoRange::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 0) {
        CFG->negRange_pct[row] = cell->getInt8();
    } else {
        CFG->posRange_pct[row] = cell->getInt8();
    }
}
