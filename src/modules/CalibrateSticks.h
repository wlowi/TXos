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

#ifndef _CalibrateSticks_h_
#define _CalibrateSticks_h_

#include "Module.h"

/* Calibrate analog inputs.
 */

typedef struct calibrateSticks_t {

    channelValue_t minPos[PORT_ANALOG_INPUT_COUNT];
    channelValue_t midPos[PORT_ANALOG_INPUT_COUNT];
    channelValue_t maxPos[PORT_ANALOG_INPUT_COUNT];

} calibrateSticks_t;

class CalibrateSticks : public Module {

    NON_PHASED_CONFIG( calibrateSticks_t)

    private:
        uint8_t calibrationStep;
#define CALIBRATION_STEP_NONE     0
#define CALIBRATION_STEP_CENTER   1
#define CALIBRATION_STEP_MINMAX   2

    public:
        CalibrateSticks();

        /* From Module */
        void run( Controls &controls) final;
        void setDefaults() final;

        /* From TableEditable */
        uint8_t getRowCount() final;
        const char *getRowName( uint8_t row) final;
        uint8_t getColCount( uint8_t row) final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;

        bool isRowEditable( uint8_t row) final { return false; }
        bool isRowExecutable( uint8_t row) final;
        void rowExecute( uint8_t row ) final;

        /* Force UI update during calibration. */
        bool hasChanged( uint8_t row, uint8_t col) { return true; }
};

#endif