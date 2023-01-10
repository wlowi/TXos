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

#ifndef _DualExpo_h_
#define _DualExpo_h_

#include "Module.h"

#define DUAL_EXPO_CHANNELS 3

typedef struct dualExpo_t {

    /* Dual Rate and expo for Aileron/Elevator/Rudder */
    percent_t value[2 * DUAL_EXPO_CHANNELS];

} dualExpo_t;

class DualExpo : public Module {

    PHASED_CONFIG( dualExpo_t )

    private:
        bool postRefresh;
        const char *phaseName;
        
        void applyRate( Controls &controls, channel_t ch, percent_t pct);
        void applyExpo( Controls &controls, channel_t ch, percent_t pct);

    public:
        DualExpo();

        /* From Module */
        void run( Controls &controls) final;
        void setDefaults() final;

        /* From TableEditable */
        bool needsRefresh() final;
        bool isRowEditable( uint8_t row) final;
        /* Return true if the value of this cell is editable */
        bool isColEditable( uint8_t row, uint8_t col) final;

        uint8_t getRowCount() final;
        const char *getRowName( uint8_t row) final;
        uint8_t getColCount( uint8_t row) final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;
};

#endif