
#ifndef _Module_h_
#define _Module_h_

#include "TXos.h"

typedef uint8_t moduleType_t;
typedef uint8_t moduleSize_t;

#define MODULE_INVALID_TYPE            ((moduleType_t)0)
#define MODULE_SERVO_REVERSE_TYPE      ((moduleType_t)1)
#define MODULE_SERVO_SUBTRIM_TYPE      ((moduleType_t)2)
#define MODULE_SERVO_LIMIT_TYPE        ((moduleType_t)3)

class Module {

    private:
        moduleType_t moduleType;
        Module *next;

    public:
        Module( moduleType_t type);
        friend class ModuleManager;
        
        virtual void run( channelSet_t &channels) = 0;
        virtual void setDefaults() = 0;

        virtual moduleSize_t getConfigSize() = 0;
        moduleType_t getConfigType();
        virtual uint8_t *getConfig() = 0;
        virtual void setConfig( uint8_t *config, moduleSize_t size) = 0;
};

#endif
