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

#include "ServoSubtrim.h"

extern const char* const OutputChannelNames[PPM_CHANNELS];

ServoSubtrim::ServoSubtrim() : Module( MODULE_SERVO_SUBTRIM_TYPE, TEXT_MODULE_SERVO_SUBTRIM) {

    setDefaults();
}

/* From Module */

void ServoSubtrim::exportConfig( Comm *exporter, uint8_t *config, moduleSize_t configSz) const {

    const servoSubtrim_t *cfg = (servoSubtrim_t*)config;

    exporter->openSub( COMM_SUBPACKET_SERVO_SUBTRIM );
    exporter->addIntArr( COMM_FIELD_PERCENT_ARRAY, (const byte*)cfg->trim_pct, sizeof(cfg->trim_pct), PPM_CHANNELS);
    exporter->close();
}

void ServoSubtrim::run( Controls &controls) {

    for( channel_t ch = 0; ch < PPM_CHANNELS; ch++) {
        controls.outputSet( ch, controls.outputGet( ch) + PCT_TO_CHANNEL( CFG->trim_pct[ch]));
    }
}

void ServoSubtrim::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        for( channel_t ch = 0; ch < PPM_CHANNELS; ch++) {
            CFG->trim_pct[ch] = 0;
        }

    )
}

/* From TableEditable */

uint8_t ServoSubtrim::getRowCount() {

    return PPM_CHANNELS;
}

const char *ServoSubtrim::getRowName( uint8_t row) {

    return OutputChannelNames[row];
}

uint8_t ServoSubtrim::getColCount( uint8_t row) {

    return 1;
}

void ServoSubtrim::getValue( uint8_t row, uint8_t col, Cell *cell) {

    cell->setInt8( TEXT_OUT_length +1, CFG->trim_pct[row], 0, PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
}

void ServoSubtrim::setValue( uint8_t row, uint8_t col, Cell *cell) {

    CFG->trim_pct[row] = cell->getInt8();
}
