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

/*
    Manages a list of modules.

    The main features are:
    * Generate a block with configuration data of all modules
      and write the block to EEPROM
    * Read a block of configuration data from EEPROM and
      distribute module specific configuration to each module.
    * Walk through the list of modules and call run() on each module.


 */

#ifndef _ModuleManager_h_
#define _ModuleManager_h_

#include "TXos.h"
#include "Module.h"
#include "ConfigBlock.h"

class ModuleManager : public TableEditable {
    
    private:
        Module *first = nullptr;
        Module *last = nullptr;

        ConfigBlock *blockService;

        void parseBlock();
        void generateBlock( configBlockID_t modelID);
        void setDefaults();

    public:
        explicit ModuleManager( ConfigBlock &svc);

        void add( Module *modulePtr);
        uint8_t getModuleCount();
        Module *getModule( uint8_t idx);
        Module *getModuleByType( moduleType_t type);
        uint8_t parseModule( configBlockID_t modelID, Module &moduleRef);

        void runModules( Controls &controls);

        void load( configBlockID_t modelID);
        void save( configBlockID_t modelID);

        /* From Interface TableEditable */
        const char *getName() final;
        uint8_t getItemCount() final;
        const char *getItemName( uint8_t row) final;
        uint8_t getValueCount() final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;
};

#endif
