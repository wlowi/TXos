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

#ifndef _VccMonitor_h_
#define _VccMonitor_h_

#include "Module.h"

typedef struct vccMonitor_t {

    float16 warnLevel;
    float16 alertLevel;
    
    /* Fine tune Vcc (battery) voltage read from ADC */
    int8_t vccAdjust;

} vccMonitor_t;

class VccMonitor : public Module {

    NON_PHASED_CONFIG( vccMonitor_t)

    private:
        float16 vcc;

    public:
        VccMonitor();

        float16 getVcc() const;
        bool belowWarn() const;
        bool belowAlert() const;

        /* From Module */
        void run( Controls &controls) final;
        void setDefaults() final;

        /* From TableEditable */
        bool isColEditable( uint8_t row, uint8_t col) final;
        uint8_t getRowCount() final;
        const char *getRowName( uint8_t row) final;
        uint8_t getColCount( uint8_t row) final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;
};

#endif