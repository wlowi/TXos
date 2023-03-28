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

#include "CalibrateTrim.h"

CalibrateTrim::CalibrateTrim() : Module( MODULE_CAL_TRIM_TYPE, TEXT_MODULE_CAL_TRIM) {

    setDefaults();
}

/* From Module */

void CalibrateTrim::run( Controls &controls) {

    long v;

    /* Usually the first 4 channels */
    for( channel_t ch = 0; ch < PORT_TRIM_INPUT_COUNT; ch++) {

        v = controls.trimADCGet( ch); // This is the raw ADC output

        if( calibrationStep == CALIBRATION_STEP_CENTER) {

            CFG->midPos[ch] = CFG->maxPos[ch] = CFG->minPos[ch] = (channelValue_t)v;

        } else if( calibrationStep == CALIBRATION_STEP_MINMAX) {

            if( v < CFG->minPos[ch]) {
                CFG->minPos[ch] = (channelValue_t)v;
            } else if( v > CFG->maxPos[ch]) {
                CFG->maxPos[ch] = (channelValue_t)v;
            }

        } else {

            if( v > CFG->midPos[ch]) {
                v = ((v - CFG->midPos[ch]) * (TRIMVALUE_MAX_LIMIT - TRIMVALUE_MID)) / (CFG->maxPos[ch] - CFG->midPos[ch]);
            } else if( v < CFG->midPos[ch]) {
                v = ((v - CFG->midPos[ch]) * (TRIMVALUE_MID - TRIMVALUE_MIN_LIMIT)) / (CFG->midPos[ch] - CFG->minPos[ch]);
            } else {
                v = TRIMVALUE_MID;
            }

            controls.trimSet( ch, (channelValue_t)v);
        }
    }
}

void CalibrateTrim::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        for( channel_t ch = 0; ch < PORT_TRIM_INPUT_COUNT; ch++) {        
            CFG->minPos[ch] = 0;
            CFG->maxPos[ch] = 1023;
            CFG->midPos[ch] = CFG->maxPos[ch] / 2;
        }
    )

    calibrationStep = CALIBRATION_STEP_NONE;
}

/* From TableEditable */

bool CalibrateTrim::isRowExecutable( uint8_t row) {

    return (row == 0);
}

void CalibrateTrim::rowExecute( uint8_t row ) {

    switch( calibrationStep) {
    case CALIBRATION_STEP_NONE:
        calibrationStep = CALIBRATION_STEP_CENTER;
        break;

    case CALIBRATION_STEP_CENTER:
        calibrationStep = CALIBRATION_STEP_MINMAX;
        break;

    case CALIBRATION_STEP_MINMAX:
        [[fallthrough]];

    default:
        calibrationStep = CALIBRATION_STEP_NONE;
        break;
    }
}

uint8_t CalibrateTrim::getRowCount() {

    return PORT_TRIM_INPUT_COUNT +1;
}

const char *CalibrateTrim::getRowName( uint8_t row) {

    return (row == 0) ? TEXT_START : TEXT_MSG_NONE;
}

uint8_t CalibrateTrim::getColCount( uint8_t row) {

    return (row == 0) ? 1 : 2;
}

void CalibrateTrim::getValue( uint8_t row, uint8_t col, Cell *cell) {

    const char *stepName;

    if( row == 0) {

        switch( calibrationStep) {
        case CALIBRATION_STEP_CENTER:
            stepName = TEXT_CALIB_CENTER;
            break;

        case CALIBRATION_STEP_MINMAX:
            stepName = TEXT_CALIB_MINMAX;
            break;

        case CALIBRATION_STEP_NONE:
            [[fallthrough]];

        default:
            stepName = TEXT_CALIB_NONE;
        }

        cell->setLabel( 6, stepName, TEXT_CALIB_length);

    } else {

        if( col == 0) {
            cell->setInt16( 1, CFG->minPos[row-1], 0, TRIMVALUE_MIN_LIMIT, TRIMVALUE_MAX_LIMIT);
        } else if( col == 1) {
            cell->setInt16( 7, CFG->maxPos[row-1], 0, TRIMVALUE_MIN_LIMIT, TRIMVALUE_MAX_LIMIT);
        }
    }
}

void CalibrateTrim::setValue( uint8_t row, uint8_t col, Cell *cell) {

    /* noop */
}
