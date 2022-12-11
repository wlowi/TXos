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
#include "Menu.h"
#include "Module.h"
#include "ConfigBlock.h"

class ModuleManager {
    
    private:

        Menu *systemMenu = new Menu( TEXT_SYSTEM_SETUP);
        Menu *modelMenu = new Menu( TEXT_MODEL_SETUP);

        Module *runlistFirst = nullptr;
        Module *runlistLast = nullptr;

        ConfigBlock *blockService;

        void parseBlock( Menu *menu);
        void generateBlock( configBlockID_t modelID, Menu *menu);

    public:
        explicit ModuleManager( ConfigBlock &svc);

        void addToSystemMenu( Module *modulePtr);
        void addToModelMenu( Module *modulePtr);
        void addToRunList( Module *modulePtr);

        Menu *getSystemMenu();
        Menu *getModelMenu();
        
        uint8_t parseModule( configBlockID_t modelID, Module &moduleRef);

        void runModules( Controls &controls);

        void switchPhase( phase_t phase);

        void loadModel( configBlockID_t modelID);
        void saveModel( configBlockID_t modelID);

        void loadSystemConfig(configBlockID_t blockID);
        void saveSystemConfig(configBlockID_t blockID);
};

#endif
