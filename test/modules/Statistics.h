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

#ifndef _Statistics_h_
#define _Statistics_h_

#include "Module.h"

class Statistics : public Module {

    NO_CONFIG()

    private:
        uint16_t timeUI_msec;
        uint16_t timeModules_msec;
        uint16_t ppmOverrun;
        timingUsec_t maxFrameTime;

        bool timing;

    public:
        explicit Statistics();

        void updateUITime( uint16_t t);
        void updateModulesTime( uint16_t t);
        void updatePPMOverrun( uint16_t c);
        void updateFrameTime( timingUsec_t t);
        bool debugTiming();

        void run( Controls &controls) final;
        void setDefaults() final;

        /* From TableEditable */
        bool isRowEditable( uint8_t row) final;

        uint8_t getRowCount() final;
        const char *getRowName( uint8_t row) final;
        uint8_t getColCount( uint8_t row) final;
        
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;
        bool hasChanged( uint8_t row, uint8_t col) final;
};

#endif