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

#include "ServoDelay.h"

extern const char* const OutputChannelNames[PPM_CHANNELS];

/* The import/export dictionary.
 * See ImportExport.h
 */
DICTROWA( r1, COMM_DATATYPE_INTARR, COMM_FIELD_DELAY_ARRAY, servoDelay_t, posDelay_sec, PPM_CHANNELS)
DICTROWA( r2, COMM_DATATYPE_INTARR, COMM_FIELD_NEG_DELAY_ARRAY, servoDelay_t, negDelay_sec, PPM_CHANNELS)
DICT( ServoDelay, COMM_SUBPACKET_SERVO_DELAY, &r1, &r2)

ServoDelay::ServoDelay() : Module( MODULE_SERVO_DELAY_TYPE, TEXT_MODULE_SERVO_DELAY, COMM_SUBPACKET_SERVO_DELAY) {

    setDefaults();
}

COMM_RC_t ServoDelay::exportConfig( ImportExport *exporter, uint8_t *config) const {

    return exporter->runExport( DICT_ptr(ServoDelay), DICTROW_ptr(ServoDelay), config, sizeof(servoDelay_t));
}

COMM_RC_t ServoDelay::importConfig( ImportExport *importer, uint8_t *config) const {

    return importer->runImport( DICT_ptr(ServoDelay), DICTROW_ptr(ServoDelay), config, sizeof(servoDelay_t));
}

/* From Module */

void ServoDelay::run( Controls &controls) {

#define SCALING_F (10)

    int16_t targetPosition10;  // scaled by 10 for increased precision
    int16_t stepPosition10;    // same here
    int16_t delay_msec;

    for( uint8_t ch = 0; ch < PPM_CHANNELS; ch++) {

        /* The target position in 1/10 % scaled to 1/100 % */
        targetPosition10 = controls.outputGet( ch) * SCALING_F;

        if( (CFG->posDelay_sec[ch] > 0) && (targetPosition10 > lastChannelValue10[ch]) ) {
            // posDelay_sec is a scaled float in 1/10 sec resolution. Convert to msec.
            delay_msec = CFG->posDelay_sec[ch] * 100;
            stepPosition10 = (int16_t)((int32_t)((CHANNELVALUE_MAX - CHANNELVALUE_MIN) * SCALING_F) * PPM_FRAME_TIME_usec / 1000 / delay_msec);
            lastChannelValue10[ch] += stepPosition10;
            if( lastChannelValue10[ch] > targetPosition10) { // Do not exceed targeted value.
                lastChannelValue10[ch] = targetPosition10;
            }

        } else if( (CFG->negDelay_sec[ch] > 0) && (targetPosition10 < lastChannelValue10[ch]) ) {
            delay_msec = CFG->negDelay_sec[ch] * 100;
            stepPosition10 = (int16_t)((int32_t)((CHANNELVALUE_MAX - CHANNELVALUE_MIN) * SCALING_F) * PPM_FRAME_TIME_usec / 1000 / delay_msec);
            lastChannelValue10[ch] -= stepPosition10;
            if( lastChannelValue10[ch] < targetPosition10) {
                lastChannelValue10[ch] = targetPosition10;
            }

        } else {
            lastChannelValue10[ch] = targetPosition10;
            continue;
        }

        controls.outputSet( ch, (channelValue_t)(lastChannelValue10[ch] / SCALING_F));
    }
}

void ServoDelay::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        for( channel_t ch = 0; ch < PPM_CHANNELS; ch++) {
            CFG->posDelay_sec[ch] = 0;
            CFG->negDelay_sec[ch] = 0;
        }

    )

    for( channel_t ch = 0; ch < PPM_CHANNELS; ch++) {
        lastChannelValue10[ch] = 0;
    }
}

/* From TableEditable */

uint8_t ServoDelay::getRowCount() {

    return PPM_CHANNELS;
}

const char *ServoDelay::getRowName( uint8_t row) {

    return OutputChannelNames[row];
}

uint8_t ServoDelay::getColCount( uint8_t row) {

    return 2;
}

void ServoDelay::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 0) {
        cell->setFloat1( 4, CFG->posDelay_sec[row], 4, 0, SERVODELAY_MAX_SEC * 10 /* 10.0 sec */);
    } else if( col == 1) {
        cell->setFloat1( 9, CFG->negDelay_sec[row], 4, 0, SERVODELAY_MAX_SEC * 10 /* 10.0 sec */);
    }
}

void ServoDelay::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 0) {
        CFG->posDelay_sec[row] = cell->getFloat1();
    } else if( col == 1) {
        CFG->negDelay_sec[row] = cell->getFloat1();
    }
}
