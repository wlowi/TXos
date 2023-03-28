/*
  TXos. A remote control transmitter OS.

  MIT License

  Copyright (c) 2023 wlowi

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
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

        uint8_t getModelCount() const;
        void loadModel( configBlockID_t modelID);
        void saveModel( configBlockID_t modelID);

        void loadSystemConfig(configBlockID_t blockID);
        void saveSystemConfig(configBlockID_t blockID);
};

#endif
