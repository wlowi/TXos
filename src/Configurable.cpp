/*
 * Configurable.cpp
 */
 
#include "Configurable.h"
#include "PPM.h"

Configurable::Configurable( ConfigBlock *srvc) {
    
    blockService = srvc;
}

void Configurable::addModule( ProcessModule *module) {
    
    modules.add( module);
}

void Configurable::process( channelSet_t *channels) {

    for( channel_t ch = 0; ch < CHANNELS; ch++) {
        PPM::setChannel( ch, channels->analogChannel[ ch ]);
    }
}

uint8_t Configurable::read( uint8_t id) {
    
    uint8_t rc;
    
    rc = blockService->readBlock( id);
    
    switch( rc) {
    case CONFIGBLOCK_RC_OK:
        rc = configureAllModules();
        break;

    case CONFIGBLOCK_RC_CSUM:
        return CONFIG_RC_CSUM;
        
    case CONFIGBLOCK_RC_INVID:
        return CONFIG_RC_INVID;
    }
    
    return rc;
}

uint8_t Configurable::configureAllModules() {
    
    ProcessModule *module;
    uint8_t type;
    uint8_t length;
    
    uint8_t *mem;
    uint8_t memPtr = 0;
    
    mem = blockService->getMemoryBlock();
    
    if( !mem) panic();
    
    type = mem[memPtr++];
    while( type != CONFIG_MODULE_INVALID && memPtr < CONFIG_BLOCK_PAYLOAD_SIZE) {
        length = mem[memPtr++];

        module = modules.findModule( GET_MODULE_TYPE( type));
        if( module) {
            module->setConfiguration( type, length, mem);
        } else {
            panic();
        }

        memPtr += length;
    }
    
    return CONFIG_RC_OK;
}

uint8_t Configurable::write( uint8_t id) {
    
    uint8_t rc;
    
    rc = blockService->formatBlock( id);
    
    switch( rc) {
    case CONFIGBLOCK_RC_OK:
        rc = getAllModuleConfigurations();
        rc = blockService->writeBlock();
        break;

    case CONFIGBLOCK_RC_CSUM:
        return CONFIG_RC_CSUM;
        
    case CONFIGBLOCK_RC_INVID:
        return CONFIG_RC_INVID;
    }
    
    return rc;
}

uint8_t Configurable::getAllModuleConfigurations() {
    
    ProcessModule *module;
    
    uint8_t *mem;
    uint8_t memPtr = 0;
    
    mem = blockService->getMemoryBlock();
    
    if( !mem) panic();
    
    module = modules.firstModule();
    while( module) {
        /* @TODO not good! beware of mem overflow */
        memPtr += module->getConfiguration( mem, memPtr);

        module = module->nextModule();
    }
    return CONFIG_RC_OK;
}
