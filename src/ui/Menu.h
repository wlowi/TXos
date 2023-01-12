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

#ifndef _Menu_h_
#define _Menu_h_

#include "TXos.h"
#include "Module.h"

class Menu : public TableEditable {

    private:

        const char *header;
        Module *first = nullptr;
        Module *last = nullptr;

    public:
        explicit Menu( const char *hdr);

        void setDefaults();
        void init();
        void switchPhase( phase_t phase);
        
        void addModule( Module *modulePtr);

        uint8_t getModuleCount();
        Module *getFirstModule();
        Module *getModule( uint8_t idx);
        Module *getModuleByType( moduleType_t type);

        /* From Interface TableEditable */
        bool isRowEditable( uint8_t row) final { return false; }
        bool isRowExecutable( uint8_t row) final;
        void rowExecute( uint8_t row ) final;

        const char *getName() final;
        uint8_t getRowCount() final;
        const char *getRowName( uint8_t row) final;
        uint8_t getColCount( uint8_t row) final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;
};

#endif
