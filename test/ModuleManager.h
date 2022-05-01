
#ifndef _ModuleManager_h_
#define _ModuleManager_h_

#include "TXos.h"
#include "Module.h"

class ModuleManager {
    
    private:
        Module *first;
        Module *last;

    public:
        ModuleManager();
        void Add( Module *module);
        void RunModules( channelSet_t &channels);
};

#endif
