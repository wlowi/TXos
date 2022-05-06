
#ifndef _ModuleManager_h_
#define _ModuleManager_h_

#include "TXos.h"
#include "Module.h"
#include "ConfigBlock.h"

class ModuleManager {
    
    private:
        Module *first;
        Module *last;
        ConfigBlock *blockService;

        void parseBlock();
        void generateBlock( configBlockID_t modelID);
        void setDefaults();

    public:
        ModuleManager( ConfigBlock &svc);
        void Add( Module *module);
        void RunModules( channelSet_t &channels);

        void load( configBlockID_t modelID);
        void save( configBlockID_t modelID);
};

#endif
