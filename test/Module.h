
#ifndef _Module_h_
#define _Module_h_

#include "TXos.h"

class Module {

    private:
        Module *next;

    public:
        Module();
        friend class ModuleManager;
        
        virtual void run( channelSet_t &channels) = 0;
};

#endif
