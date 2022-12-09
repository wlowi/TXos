/*
    TXos. A remote control transmitter OS.

    Copyright (C) 2022 Wolfgang Lohwasser

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#include "CalibrateTrim.h"

CalibrateTrim::CalibrateTrim() : Module( MODULE_CAL_TRIM_TYPE, TEXT_MODULE_CAL_TRIM) {

    setDefaults();
}

/* From Module */

void CalibrateTrim::run( Controls &controls) {

    long v;

    for( channel_t ch = 0; ch < PORT_TRIM_INPUT_COUNT; ch++) {

        v = controls.trimGet( ch);

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

            controls.inputSet( ch, controls.inputGet(ch) + (channelValue_t)v);
        }
    }
}

void CalibrateTrim::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        for( channel_t ch = 0; ch < PORT_TRIM_INPUT_COUNT; ch++) {        
            CFG->minPos[ch] = 0;
            CFG->midPos[ch] = 511;
            CFG->maxPos[ch] = 1023;
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
    default:
        calibrationStep = CALIBRATION_STEP_NONE;
        break;
    }
}

uint8_t CalibrateTrim::getRowCount() {

    return PORT_TRIM_INPUT_COUNT +1;
}

const char *CalibrateTrim::getRowName( uint8_t row) {

    return (row == 0) ? TEXT_START : " ";
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
        default:
            stepName = TEXT_CALIB_NONE;
        }

        cell->setLabel( 6, stepName, TEXT_CALIB_length);

    } else {

        if( col == 0) {
            cell->setInt16( 1, CFG->minPos[row-1], 0, TRIMVALUE_MIN_LIMIT, TRIMVALUE_MAX_LIMIT);
        } else {
            cell->setInt16( 7, CFG->maxPos[row-1], 0, TRIMVALUE_MIN_LIMIT, TRIMVALUE_MAX_LIMIT);
        }

    }
}

void CalibrateTrim::setValue( uint8_t row, uint8_t col, Cell *cell) {

    /* noop */
}
