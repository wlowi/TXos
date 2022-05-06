
#include "ModuleManager.h"

ModuleManager::ModuleManager( ConfigBlock &svc) {

    first = last = NULL;
    blockService = &svc;
}

void ModuleManager::Add( Module *module) {

    module->next = NULL;

    if( first == NULL) {
        first = last = module;
    } else {
        last->next = module;
        last = module;
    }
}

void ModuleManager::RunModules( channelSet_t &channels) {

    Module *current = first;

    while( current != NULL) {
        current->run( channels);
        current = current->next;
    }
}

void ModuleManager::load( configBlockID_t modelID) {

    blockService->readBlock( modelID);

    if( blockService->isBlockValid()) {
        parseBlock();
    } else {
        LOG("ModuleManager::load(): Block %d is invalid.\n", modelID);
        setDefaults();
        save( modelID);
    }
}

void ModuleManager::save( configBlockID_t modelID) {

    generateBlock( modelID);

    blockService->writeBlock();
}

void ModuleManager::parseBlock() {

    uint8_t *payload;
    moduleType_t type;
    moduleSize_t size;
    Module *current = first;

    payload = blockService->getPayload();

    while( current != NULL) {

#define GET( p, s)                              \
    do {                                        \
        blockService->memcpy( p, payload, s);   \
        payload += s;                           \
    } while( 0 )

        GET( (uint8_t*)&type, sizeof(moduleType_t));
        GET( (uint8_t*)&size, sizeof(moduleSize_t));
        GET( current->getConfig(), size);

        LOG("ModuleManager::parseBlock(): GET type=%d size=%d\n", type, size);

        current = current->next;
    }
}

void ModuleManager::generateBlock(configBlockID_t modelID) {

    uint8_t *payload;
    moduleType_t type;
    moduleSize_t size;
    moduleSize_t totalSize = 0;
    Module *current = first;

    payload = blockService->getPayload();

    while( current != NULL) {
        
        size = current->getConfigSize();
        type = current->getConfigType();

        if( totalSize + sizeof(moduleType_t) + sizeof(moduleSize_t) + size <= CONFIG_PAYLOAD_SIZE) {

#define PUT( p, s)                              \
    do {                                        \
        blockService->memcpy( payload, p, s);   \
        payload += s;                           \
        totalSize += s;                         \
    } while( 0 )

            PUT( (uint8_t*)&type, sizeof(moduleType_t));
            PUT( (uint8_t*)&size, sizeof(moduleSize_t));
            PUT( current->getConfig(), size);
        }

        current = current->next;
    }
}

void ModuleManager::setDefaults() {

    Module *current = first;

    while( current != NULL) {
        current->setDefaults();
        current = current->next;
    }
}
