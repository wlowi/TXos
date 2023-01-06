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

#ifndef _AnalogTrim_h_
#define _AnalogTrim_h_

#include "Module.h"

/* Adjust servo subtrim to current trim values */

typedef struct analogTrim_t {

    percent_t storedTrim_pct[PORT_TRIM_INPUT_COUNT];

} analogTrim_t;

class AnalogTrim : public Module {

    NON_PHASED_CONFIG( analogTrim_t)

    private:
        Controls *current;

        percent_t trim_pct[PORT_TRIM_INPUT_COUNT];

        uint8_t execStep;
#define ANALOGTRIM_STEP_NONE     0
#define ANALOGTRIM_STEP_SAVE     1
#define ANALOGTRIM_STEP_ADJUST   2

    public:
        AnalogTrim();

        /* From Module */
        void run( Controls &controls) final;
        void setDefaults() final;

        void moduleEnter() final;
        void moduleExit() final;

        /* From TableEditable */
        uint8_t getRowCount() final;
        const char *getRowName( uint8_t row) final;
        uint8_t getColCount( uint8_t row) final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;

        bool isColEditable( uint8_t row, uint8_t col) final;

        /* Force UI update during calibration. */
        bool hasChanged( uint8_t row, uint8_t col) final { return true; }
};

#endif