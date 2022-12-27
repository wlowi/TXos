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

#ifndef _AssignInput_h_
#define _AssignInput_h_

#include "Module.h"

/* Assign input channels (analog) to logical channels */

typedef struct assignInput_t {

    /* source is an analog channel */
    channel_t source[MIX_CHANNELS];

} assignInput_t;

class AssignInput : public Module {

    NON_PHASED_CONFIG( assignInput_t)

    public:
        AssignInput();

        /* From Module */
        void run( Controls &controls) final;
        void setDefaults() final;

        /* From TableEditable */
        uint8_t getRowCount() final;
        const char *getRowName( uint8_t row) final;
        uint8_t getColCount( uint8_t row) final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;
};

#endif