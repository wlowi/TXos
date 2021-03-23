/*
 * Configurable.h
 */
 
#ifndef _Configurable_h
#define _Configurable_h

#include "Arduino.h"
#include "ConfigBlock.h"
#include "ProcessChain.h"

/* Function return codes */
#define CONFIG_RC_OK             ((uint8_t)0)
#define CONFIG_RC_INVID          ((uint8_t)1)
#define CONFIG_RC_CSUM           ((uint8_t)2)



/* The lower 5 bits identify the module, so there are 
 * 31 possible modules [1-31]
 */
#define CONFIG_MODULE_INVALID            ((uint8_t)0)

#define CONFIG_MODULE_MODELSELECT        ((uint8_t)1)
#define CONFIG_MODULE_MODESELECT         ((uint8_t)2)
#define CONFIG_MODULE_SERVOREVERSE       ((uint8_t)3)
#define CONFIG_MODULE_SERVOSUBTRIM       ((uint8_t)4)
#define CONFIG_MODULE_SERVOLIMIT         ((uint8_t)5)

/* The upper 3 bits identify the module subtype
 * There are 8 different subtypes.
 */
#define CONFIG_MODULE_SUBTYPE_0          ((uint8_t)0)
#define CONFIG_MODULE_SUBTYPE_1          ((uint8_t)1)
#define CONFIG_MODULE_SUBTYPE_2          ((uint8_t)2)
#define CONFIG_MODULE_SUBTYPE_3          ((uint8_t)3)
#define CONFIG_MODULE_SUBTYPE_4          ((uint8_t)4)
#define CONFIG_MODULE_SUBTYPE_5          ((uint8_t)5)
#define CONFIG_MODULE_SUBTYPE_6          ((uint8_t)6)
#define CONFIG_MODULE_SUBTYPE_7          ((uint8_t)7)

/* uint8_t MAKE_MODULE_CONFIG_ID( uint8_t type, uint8_t subtype)
 */
#define MAKE_MODULE_CONFIG_ID( t, s)   (t | (s << 5))

#define GET_MODULE_TYPE( i)            (i & 0b00011111)
#define GET_MODULE_SUBTYPE( i)         ((i & 0b11100000) >> 5)


class Configurable {
private:
    ConfigBlock *blockService;
    ProcessChain modules;

public:
    Configurable( ConfigBlock *srvc);

    void addModule( ProcessModule *module);
    void process( channelSet_t *channels);

    uint8_t read( uint8_t id);
    uint8_t write( uint8_t id);
    
    /* Private */
    uint8_t configureAllModules();
    uint8_t getAllModuleConfigurations();
};

#endif
