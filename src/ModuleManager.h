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
#include "Module.h"
#include "ConfigBlock.h"

#define MODULE_SET_SYSTEM     1
#define MODULE_SET_MODEL      2

class ModuleManager {
    
    private:

        TextUIMenu *systemMenu = new TextUIMenu( TEXT_SYSTEM_SETUP, true);
        TextUIMenu *modelMenu = new TextUIMenu( TEXT_MODEL_SETUP, true);

        /* A list of modules to run. This models modify stick and switch inputs */
        Module *runlistFirst = nullptr;
        Module *runlistLast = nullptr;

        /* All system models */
        Module *systemSetFirst = nullptr;
        Module *systemSetLast = nullptr;

        /* All model related models */
        Module *modelSetFirst = nullptr;
        Module *modelSetLast = nullptr;

        ConfigBlock *blockService;

        void parseBlock( uint8_t setType);
        void generateBlock( configBlockID_t modelID, uint8_t setType);

    public:
        explicit ModuleManager( ConfigBlock &svc);

        void addToSystemMenu( TextUIScreen *scr);
        void addToModelMenu( TextUIScreen *scr);

        void addToSystemSetAndMenu( Module *modulePtr);
        void addToModelSetAndMenu( Module *modulePtr);

        void addToRunList( Module *modulePtr);

        Module *getModuleByType( uint8_t setType, moduleType_t type);

        TextUIMenu *getSystemMenu();
        TextUIMenu *getModelMenu();
        
        uint8_t parseModule( configBlockID_t modelID, Module &moduleRef);

        void runModules( Controls &controls);

        void switchPhase( phase_t phase);
        void setModelDefaults();
        void setSystemDefaults();
        void initModel();

        uint8_t getModelCount() const;
        void loadModel( configBlockID_t modelID);
        void saveModel( configBlockID_t modelID);

        void loadSystemConfig( configBlockID_t blockID);
        void saveSystemConfig( configBlockID_t blockID);
};

#endif
