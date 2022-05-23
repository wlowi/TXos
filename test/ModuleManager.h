
#ifndef _ModuleManager_h_
#define _ModuleManager_h_

#include "TXos.h"
#include "Module.h"
#include "ConfigBlock.h"

class ModuleManager : public TableEditable {
    
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
        uint8_t getModuleCount();
        Module *getModule( uint8_t idx);
        Module *getModuleByType( moduleType_t type);

        void RunModules( channelSet_t &channels);

        void load( configBlockID_t modelID);
        void save( configBlockID_t modelID);

        /* From Interface TableEditable */
        const char *getName();
        uint8_t getItemCount();
        const char *getItemName( uint8_t row);
        uint8_t getValueCount();
        void getValue( uint8_t row, uint8_t col, Cell *cell);
        void setValue( uint8_t row, uint8_t col, Cell *cell);
};

#endif
