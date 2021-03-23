/*
 * ModelConfig.h
 */
 
#ifndef _ModelConfig_h
#define _ModelConfig_h

#include "Arduino.h"
#include "Configurable.h"

class ModelConfig : public Configurable {

public:
    ModelConfig( ConfigBlock *srvc);

};

#endif
