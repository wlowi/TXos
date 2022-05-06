
#ifndef _SystemConfig_h_
#define _SystemConfig_h_

#include "TXos.h"
#include "ConfigBlock.h"

#define SYSTEMCONFIG_BLOCKID    ((configBlockID_t)0)

typedef struct systemConfig_t {

        configBlockID_t modelID;

} systemConfig_t;

class SystemConfig {

    private:
        ConfigBlock *blockService;

        systemConfig_t cfg;
        
        void parseBlock();
        void generateBlock();
        void setDefaults();

    public:
        SystemConfig( ConfigBlock &svc);
        void load();
        void save();

        configBlockID_t getModelID();
};

#endif
