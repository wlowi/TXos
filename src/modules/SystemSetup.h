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

#ifndef _SystemSetup_h_
#define _SystemSetup_h_

#include "Module.h"

class SystemSetup : public Module {

    NO_CONFIG()
    
    public:
        explicit SystemSetup();
        
        /* From MOdule */
        void run( Controls &controls) final;
        void setDefaults() final;

        /* From TableEditable */
        bool isRowEditable( uint8_t row) final { return false; }
        bool isModuleExecutable() final { return true; }

        void moduleExecute() final;

        uint8_t getRowCount() final;
        const char *getRowName( uint8_t row) final;
        uint8_t getColCount( uint8_t row) final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;
};

#endif