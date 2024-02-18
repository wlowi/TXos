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

#include "ServoReverse.h"

extern const char* const OutputChannelNames[PPM_CHANNELS];

/* The import/export dictionary. 
 * See ImportExport.h
 */
DICTROW( r1, COMM_DATATYPE_UINT16, COMM_FIELD_BITS, servoReverse_t, revBits)
DICT( ServoReverse, COMM_SUBPACKET_SERVO_REVERSE, &r1)

ServoReverse::ServoReverse() : Module( MODULE_SERVO_REVERSE_TYPE, TEXT_MODULE_SERVO_REVERSE, COMM_SUBPACKET_SERVO_REVERSE) {

    setDefaults();
}

/* From Module */

void ServoReverse::exportConfig( ImportExport *exporter, uint8_t *config) const {

    exporter->runExport( DICT_ptr(ServoReverse), DICTROW_ptr(ServoReverse), config, sizeof(servoReverse_t));
}

void ServoReverse::importConfig( ImportExport *importer, uint8_t *config) const {

    importer->runImport( DICT_ptr(ServoReverse), DICTROW_ptr(ServoReverse), config, sizeof(servoReverse_t));
}

void ServoReverse::run( Controls &controls) {

    for( channel_t ch = 0; ch < PPM_CHANNELS; ch++) {
        if( IS_BIT_SET( CFG->revBits, ch)) {
            controls.outputSet( ch, -controls.outputGet(ch));
        }
    }
}

void ServoReverse::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        CFG->revBits = 0;

    )
}

/* From TableEditable */

uint8_t ServoReverse::getRowCount() {

    return PPM_CHANNELS;
}

const char *ServoReverse::getRowName( uint8_t row) {

    return OutputChannelNames[row];
}

uint8_t ServoReverse::getColCount( uint8_t row) {

    return 1;
}

void ServoReverse::getValue( uint8_t row, uint8_t col, Cell *cell) {

    cell->setBool( TEXT_OUT_length +1, IS_BIT_SET( CFG->revBits, row));
}

void ServoReverse::setValue( uint8_t row, uint8_t col, Cell *cell) {
    
    if( cell->getBool()) {
        BIT_SET( CFG->revBits, row);
    } else {
        BIT_CLEAR( CFG->revBits, row);
    }
}
