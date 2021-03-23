/*
 * SystemConfig.h
 */
 
#ifndef _SystemConfig_h
#define _SystemConfig_h

#include "Arduino.h"
#include "Configurable.h"

class SystemConfig : public Configurable {

public:
    SystemConfig( ConfigBlock *srvc);    
};

#endif
