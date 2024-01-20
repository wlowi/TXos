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

#include "ServoLimit.h"

extern const char* const OutputChannelNames[PPM_CHANNELS];

ServoLimit::ServoLimit() : Module( MODULE_SERVO_LIMIT_TYPE, TEXT_MODULE_SERVO_LIMIT) {

    setDefaults();
}

void ServoLimit::exportConfig( Comm *exporter, uint8_t *config, moduleSize_t configSz) const {

    const servoLimit_t *cfg = (servoLimit_t*)config;

    exporter->openSub( COMM_SUBPACKET_SERVO_LIMIT );
    exporter->addIntArr( COMM_FIELD_PERCENT_ARRAY, (const byte *)cfg->posLimit_pct, sizeof(cfg->posLimit_pct), PPM_CHANNELS);
    exporter->addIntArr( COMM_FIELD_NEG_PERCENT_ARRAY, (const byte*)cfg->negLimit_pct, sizeof(cfg->negLimit_pct), PPM_CHANNELS);
    exporter->close();
}

/* From Module */

void ServoLimit::run( Controls &controls) {

    channelValue_t v;

    for( channel_t ch = 0; ch < PPM_CHANNELS; ch++) {

        v = PCT_TO_CHANNEL( CFG->posLimit_pct[ch]);
        if( controls.outputGet( ch) > v) {
            controls.outputSet( ch, v);
        }

        v = PCT_TO_CHANNEL( CFG->negLimit_pct[ch]);
        if( controls.outputGet( ch) < v) {
            controls.outputSet( ch, v);
        }
    }
}

void ServoLimit::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        for( channel_t ch = 0; ch < PPM_CHANNELS; ch++) {
            CFG->posLimit_pct[ch] = PERCENT_MAX_LIMIT;
            CFG->negLimit_pct[ch] = PERCENT_MIN_LIMIT;
        }

    )
}

/* From TableEditable */

uint8_t ServoLimit::getRowCount() {

    return PPM_CHANNELS;
}

const char *ServoLimit::getRowName( uint8_t row) {

    return OutputChannelNames[row];
}

uint8_t ServoLimit::getColCount( uint8_t row) {

    return 2;
}

void ServoLimit::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 0) {
        cell->setInt8( 4, CFG->negLimit_pct[row], 0, PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
    } else {
        cell->setInt8( 9, CFG->posLimit_pct[row], 0, PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
    }
}

void ServoLimit::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 0) {
        CFG->negLimit_pct[row] = cell->getInt8();
    } else {
        CFG->posLimit_pct[row] = cell->getInt8();
    }
}
