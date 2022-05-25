
#ifndef _Module_h_
#define _Module_h_

#include "TXos.h"
#include "TableEditable.h"

typedef uint8_t moduleType_t;
typedef uint8_t moduleSize_t;

#define MODULE_INVALID_TYPE             ((moduleType_t)0)

#define MODULE_MODEL_SELECT_TYPE        ((moduleType_t)1)
#define MODULE_MODEL_TYPE               ((moduleType_t)2)
#define MODULE_ENGINE_CUT_TYPE          ((moduleType_t)10)
#define MODULE_SERVO_REVERSE_TYPE       ((moduleType_t)20)
#define MODULE_SERVO_SUBTRIM_TYPE       ((moduleType_t)21)
#define MODULE_SERVO_LIMIT_TYPE         ((moduleType_t)22)

class Module : public TableEditable {

    private:
        const char *moduleName;
        moduleType_t moduleType;
        Module *next;

    public:
        Module( moduleType_t type, const char *name);
        friend class ModuleManager;
        
        virtual void run( channelSet_t &channels) = 0;
        virtual void setDefaults() = 0;

        virtual moduleSize_t getConfigSize() = 0;
        moduleType_t getConfigType();
        virtual uint8_t *getConfig() = 0;

        /* From Interface TableEditable */
        const char *getName();
};

#endif
