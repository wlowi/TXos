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

#include "ServoRemap.h"

extern const char* const LogicalChannelNames[LOGICAL_CHANNELS];
extern const char* const OutputChannelNames[PPM_CHANNELS];

ServoRemap::ServoRemap() : Module( MODULE_SERVO_REMAP_TYPE, TEXT_MODULE_SERVO_REMAP) {

    setDefaults();
}

/* From Module */

void ServoRemap::exportConfig( Comm *exporter, uint8_t *config, moduleSize_t configSz) const {

    const servoRemap_t *cfg = (servoRemap_t*)config;

    exporter->openSub( COMM_SUBPACKET_SERVO_REMAP );
    exporter->addUIntArr( COMM_FIELD_CHANNEL_ARRAY, (const byte*)cfg->source, sizeof(cfg->source), PPM_CHANNELS);
    exporter->close();
}

void ServoRemap::run( Controls &controls) {

    for( channel_t ch = 0; ch < PPM_CHANNELS; ch++) {
        controls.outputSet( ch, controls.logicalGet( CFG->source[ch]));
    }
}

void ServoRemap::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        for( channel_t ch = 0; ch < PPM_CHANNELS; ch++) {
            if( ch == 0) {
                CFG->source[ch] = CHANNEL_THROTTLE;
            } else if( ch == 1) {
                CFG->source[ch] = CHANNEL_AILERON;
            } else if( ch == 2) {
                CFG->source[ch] = CHANNEL_ELEVATOR;
            } else if( ch == 3) {
                CFG->source[ch] = CHANNEL_RUDDER;
            } else if( ch == 4) {
                CFG->source[ch] = CHANNEL_AILERON2;
            } else if( ch == 5) {
                CFG->source[ch] = CHANNEL_FLAP;
            } else if( ch == 6) {
                CFG->source[ch] = CHANNEL_GEAR;
            } else if( ch == 7) {
                CFG->source[ch] = CHANNEL_8;
            } else {
                CFG->source[ch] = CHANNEL_9;
            }
        }
    )
}

/* From TableEditable */

uint8_t ServoRemap::getRowCount() {

    return PPM_CHANNELS;
}

const char *ServoRemap::getRowName( uint8_t row) {

    return OutputChannelNames[row];
}

uint8_t ServoRemap::getColCount( uint8_t row) {

    return 2;
}

void ServoRemap::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 0) {
        cell->setLabel( TEXT_OUT_length +1, TEXT_ARROW_LEFT, 1);
    } else if( col == 1) {
        cell->setList( TEXT_OUT_length +3, LogicalChannelNames, LOGICAL_CHANNELS, CFG->source[row]);
    }
}

void ServoRemap::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 1) {
        CFG->source[row] = cell->getList();
    }
}
