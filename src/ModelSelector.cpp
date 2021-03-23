/*
 * ModelSelector.cpp
 */
 
#include "ModelSelector.h"
#include "Configurable.h"

ModelSelector::ModelSelector() : ProcessModule( CONFIG_MODULE_MODELSELECT) {

    init();
}

void ModelSelector::init() {

    model = 1;
}

void ModelSelector::setConfiguration( uint8_t type, uint8_t length, uint8_t *mem) {
    
    if( GET_MODULE_SUBTYPE(type) == 0 && length == sizeof(model)) {
        memcpy( (void*)&model, (const void*)mem, (size_t)length);
    }
}

uint8_t ModelSelector::getConfiguration( uint8_t *mem, uint8_t ptr) {
    
    uint8_t bytesWritten = 0;
    uint8_t type = MAKE_MODULE_CONFIG_ID( getModuleType(), CONFIG_MODULE_SUBTYPE_0);
    uint8_t length = (uint8_t)sizeof(model);

    mem[ptr++] = type;
    bytesWritten++;
    
    mem[ptr++] = length;
    bytesWritten++;
    
    memcpy( (void*)&mem[ptr], (const void*)&model, (size_t)length);
    // ptr += length;
    bytesWritten += length;
    
    return bytesWritten;
}

void ModelSelector::process( channelSet_t *channels) {
    
    /* Nothing to do */
}

uint8_t ModelSelector::getModel() {
    
    return model;
}

void ModelSelector::setModel( uint8_t m) {
    
    model = m;
}
