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

void CalibrateTrim::run( Controls &controls) {

    long v;

    for( channel_t ch = 0; ch < PORT_TRIM_INPUT_COUNT; ch++) {

        v = controls.trimGet( ch);

        if( calibrationStep == CALIBRATION_STEP_CENTER) {

            cfg.midPos[ch] = cfg.maxPos[ch] = cfg.minPos[ch] = (channelValue_t)v;

        } else if( calibrationStep == CALIBRATION_STEP_MINMAX) {

            if( v < cfg.minPos[ch]) {
                cfg.minPos[ch] = (channelValue_t)v;
            } else if( v > cfg.maxPos[ch]) {
                cfg.maxPos[ch] = (channelValue_t)v;
            }

        } else {

            if( v > cfg.midPos[ch]) {
                v = ((long)(v - cfg.midPos[ch]) * (TRIMVALUE_MAX_LIMIT - TRIMVALUE_MID)) / (cfg.maxPos[ch] - cfg.midPos[ch]);
            } else if( v < cfg.midPos[ch]) {
                v = ((long)(v - cfg.midPos[ch]) * (TRIMVALUE_MID - TRIMVALUE_MIN_LIMIT)) / (cfg.midPos[ch] - cfg.minPos[ch]);
            } else {
                v = TRIMVALUE_MID;
            }

            controls.analogSet( ch, controls.analogGet(ch) + (channelValue_t)v);
        }
    }
}

void CalibrateTrim::setDefaults() {

    for( channel_t ch = 0; ch < PORT_TRIM_INPUT_COUNT; ch++) {        
        cfg.minPos[ch] = 0;
        cfg.midPos[ch] = 511;
        cfg.maxPos[ch] = 1023;
    }

    calibrationStep = CALIBRATION_STEP_NONE;
}

/* From Module */

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

moduleSize_t CalibrateTrim::getConfigSize() {

    return (moduleSize_t)sizeof( cfg);
}

uint8_t *CalibrateTrim::getConfig() {

    return (uint8_t*)&cfg;
}

/* From TableEditable */

uint8_t CalibrateTrim::getRowCount() {

    return PORT_TRIM_INPUT_COUNT +1;
}

const char *CalibrateTrim::getRowName( uint8_t row) {

    return (row == 0) ? "RUN" : " ";
}

uint8_t CalibrateTrim::getColCount( uint8_t row) {

    return (row == 0) ? 1 : 2;
}

void CalibrateTrim::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( row == 0) {

        switch( calibrationStep) {
        case CALIBRATION_STEP_CENTER:
            strcpy( stepName, TEXT_CALIB_CENTER);
            break;

        case CALIBRATION_STEP_MINMAX:
            strcpy( stepName, TEXT_CALIB_MINMAX);
            break;

        case CALIBRATION_STEP_NONE:
        default:
            strcpy( stepName, TEXT_CALIB_NONE);
        }

        cell->setString( 4, stepName, TEXT_CALIB_length);

    } else {

        if( col == 0) {
            cell->setInt16( 1, cfg.minPos[row-1], TRIMVALUE_MIN_LIMIT, TRIMVALUE_MAX_LIMIT);
        } else {
            cell->setInt16( 7, cfg.maxPos[row-1], TRIMVALUE_MIN_LIMIT, TRIMVALUE_MAX_LIMIT);
        }

    }
}

void CalibrateTrim::setValue( uint8_t row, uint8_t col, Cell *cell) {

    /* noop */
}
