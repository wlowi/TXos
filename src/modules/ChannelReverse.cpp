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

#include "ChannelReverse.h"

extern const char* const InputChannelNames[INPUT_CHANNELS];

/* The import/export dictionary. 
 * See ImportExport.h
 */
DICTROW( r1, COMM_DATATYPE_UINT16, COMM_FIELD_BITS, channelReverse_t, revBits)
DICT( ChannelReverse, COMM_SUBPACKET_CHANNEL_REVERSE, &r1)

ChannelReverse::ChannelReverse() : Module( MODULE_CHANNEL_REVERSE_TYPE, TEXT_MODULE_CHANNEL_REVERSE, COMM_SUBPACKET_CHANNEL_REVERSE) {

    setDefaults();
}

/* From Module */

COMM_RC_t ChannelReverse::exportConfig( ImportExport *exporter, uint8_t *config) const {

    return exporter->runExport( DICT_ptr(ChannelReverse), DICTROW_ptr(ChannelReverse), config, sizeof(channelReverse_t));
}

COMM_RC_t ChannelReverse::importConfig( ImportExport *importer, uint8_t *config) const {

    return importer->runImport( DICT_ptr(ChannelReverse), DICTROW_ptr(ChannelReverse), config, sizeof(channelReverse_t));
}

void ChannelReverse::run( Controls &controls) {

    for( channel_t ch = 0; ch < PORT_ANALOG_INPUT_COUNT; ch++) {
        if( IS_BIT_SET( CFG->revBits, ch)) {
            controls.inputSet( ch, -controls.inputGet(ch));
        }
    }
}

void ChannelReverse::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        CFG->revBits = 0;
    )
}

/* From TableEditable */

uint8_t ChannelReverse::getRowCount() {

    return PORT_ANALOG_INPUT_COUNT;
}

const char *ChannelReverse::getRowName( uint8_t row) {

    return InputChannelNames[row];
}

uint8_t ChannelReverse::getColCount( uint8_t row) {

    return 1;
}

void ChannelReverse::getValue( uint8_t row, uint8_t col, Cell *cell) {

    cell->setBool( TEXT_INPUT_length +1, IS_BIT_SET( CFG->revBits, row));
}

void ChannelReverse::setValue( uint8_t row, uint8_t col, Cell *cell) {
    
    if( cell->getBool()) {
        BIT_SET( CFG->revBits, row);
    } else {
        BIT_CLEAR( CFG->revBits, row);
    }
}
