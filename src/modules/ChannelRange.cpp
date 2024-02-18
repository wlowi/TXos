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

#include "ChannelRange.h"

extern const char* const InputChannelNames[INPUT_CHANNELS];

/* The import/export dictionary. 
 * See ImportExport.h
 */
DICTROWA( r1, COMM_DATATYPE_INTARR, COMM_FIELD_PERCENT_ARRAY, channelRange_t, range_pct, PORT_ANALOG_INPUT_COUNT)
DICT( ChannelRange, COMM_SUBPACKET_CHANNEL_RANGE, &r1)

ChannelRange::ChannelRange() : Module( MODULE_CHANNEL_RANGE_TYPE, TEXT_MODULE_CHANNEL_RANGE, COMM_SUBPACKET_CHANNEL_RANGE) {

    setDefaults();
}

/* From Module */

void ChannelRange::exportConfig( ImportExport *exporter, uint8_t *config) const {

    exporter->runExport( DICT_ptr(ChannelRange), DICTROW_ptr(ChannelRange), config, sizeof(channelRange_t));
}

void ChannelRange::importConfig( ImportExport *importer, uint8_t *config) const {

    importer->runImport( DICT_ptr(ChannelRange), DICTROW_ptr(ChannelRange), config, sizeof(channelRange_t));
}

void ChannelRange::run( Controls &controls) {

    long v;

    for( channel_t ch = 0; ch < PORT_ANALOG_INPUT_COUNT; ch++) {

        v = controls.inputGet( ch);
        v = v * CFG->range_pct[ch] / PERCENT_MAX_LIMIT;
        controls.inputSet( ch, (channelValue_t)v);
        controls.rangeSet( ch, CFG->range_pct[ch]);
    }
}

void ChannelRange::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        for( channel_t ch = 0; ch < PORT_ANALOG_INPUT_COUNT; ch++) {
            CFG->range_pct[ch] = 100;
        }

    )
}

/* From TableEditable */

uint8_t ChannelRange::getRowCount() {

    return PORT_ANALOG_INPUT_COUNT;
}

const char *ChannelRange::getRowName( uint8_t row) {

    return InputChannelNames[row];
}

uint8_t ChannelRange::getColCount( uint8_t row) {

    return 1;
}

void ChannelRange::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 0) {
        cell->setInt8( TEXT_INPUT_length +1, CFG->range_pct[row], 3, 0, PERCENT_MAX_LIMIT);
    }
}

void ChannelRange::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 0) {
        CFG->range_pct[row] = cell->getInt8();
    }
}
