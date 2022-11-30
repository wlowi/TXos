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

#ifndef _CalibrateTrim_h_
#define _CalibrateTrim_h_

#include "TXos.h"
#include "Module.h"

typedef struct calibrateTrim_t {

    channelValue_t minPos[PORT_TRIM_INPUT_COUNT];
    channelValue_t midPos[PORT_TRIM_INPUT_COUNT];
    channelValue_t maxPos[PORT_TRIM_INPUT_COUNT];

} calibrateTrim_t;

class CalibrateTrim : public Module {

    private:
        calibrateTrim_t cfg;

        uint8_t calibrationStep;
#define CALIBRATION_STEP_NONE     0
#define CALIBRATION_STEP_CENTER   1
#define CALIBRATION_STEP_MINMAX   2

        char stepName[10];

    public:
        CalibrateTrim();

        /* From Module */
        void run( Controls &controls) final;
        void setDefaults() final;
        moduleSize_t getConfigSize() final;
        uint8_t *getConfig() final;

        /* From TableEditable */
        uint8_t getRowCount() final;
        const char *getRowName( uint8_t row) final;
        uint8_t getColCount( uint8_t row) final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;

        bool isRowEditable( uint8_t row) { return false; }
        bool isRowExecutable( uint8_t row) final;
        void rowExecute( uint8_t row ) final;
        bool hasChanged( uint8_t row, uint8_t col) { return true; }
};

#endif