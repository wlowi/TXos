
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

uint8_t ModuleManager::getModuleCount() {

    Module *current = first;
    uint8_t cnt = 0;

    while( current != NULL) {
        cnt++;
        current = current->next;
    }

    return cnt;
}

Module *ModuleManager::getModule( uint8_t idx) {

    Module *current = first;
    uint8_t cnt = 0;

    while( current != NULL) {
        if( cnt == idx) {
            break;
        }
        cnt++;
        current = current->next;
    }

    return current;
}

Module *ModuleManager::getModuleByType( moduleType_t type) {

    Module *current = first;

    while( current != NULL) {
        if( current->getConfigType() == type) {
            break;
        }
        current = current->next;
    }

    return current;
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

#define GET( p, s)                              \
    do {                                        \
        blockService->memcpy( p, payload, s);   \
        payload += s;                           \
    } while( 0 )

void ModuleManager::parseBlock() {

    uint8_t *payload;
    moduleType_t type;
    moduleSize_t size;
    Module *current;

    payload = blockService->getPayload();

    GET( (uint8_t*)&type, sizeof(moduleType_t));

    while( type != MODULE_INVALID_TYPE) {

        GET( (uint8_t*)&size, sizeof(moduleSize_t));

        LOG("ModuleManager::parseBlock(): GET type=%d size=%d\n", type, size);

        current = getModuleByType( type);
        if( current == NULL) {
            LOG("ModuleManager::parseBlock(): No module of type=%d\n", type);
            break;
        }

        if( current->getConfigSize() != size) {
            LOG("ModuleManager::parseBlock(): Config size mismatch of module type=%d %d != %d\n",
                type, current->getConfigSize(), size);
            break;
        }

        GET( current->getConfig(), size);

        GET( (uint8_t*)&type, sizeof(moduleType_t));
    }
}

#define PUT( p, s)                              \
    do {                                        \
        blockService->memcpy( payload, p, s);   \
        payload += s;                           \
        totalSize += s;                         \
    } while( 0 )

void ModuleManager::generateBlock(configBlockID_t modelID) {

    uint8_t *payload;
    moduleType_t type;
    moduleSize_t size;
    moduleSize_t totalSize = 0;
    Module *current = first;

    payload = blockService->getPayload();

    while( current != NULL) {
        
        size = current->getConfigSize();

        if( size > 0) { // Do not store module config of size 0

            type = current->getConfigType();

            // +1 is for terminating invalid module type
            if( totalSize + sizeof(moduleType_t) + sizeof(moduleSize_t) + size +1 <= CONFIG_PAYLOAD_SIZE) {
                PUT( (uint8_t*)&type, sizeof(moduleType_t));
                PUT( (uint8_t*)&size, sizeof(moduleSize_t));
                PUT( current->getConfig(), size);
            } else {
                LOG("ModuleManager::generateBlock(): Payload to large. %d > %d\n",
                    totalSize + sizeof(moduleType_t) + sizeof(moduleSize_t) + size,
                    CONFIG_PAYLOAD_SIZE);
            }
        }

        current = current->next;
    }

    // Terminating invalid module type
    type = MODULE_INVALID_TYPE;
    PUT( (uint8_t*)&type, sizeof(moduleType_t));

    LOG("ModuleManager::generateBlock(): Payload %d bytes\n", totalSize);
}

void ModuleManager::setDefaults() {

    Module *current = first;

    while( current != NULL) {
        current->setDefaults();
        current = current->next;
    }
}

/* Interface TableEditable */

const char *ModuleManager::getName() {

    return TEXT_SELECT;
}

uint8_t ModuleManager::getItemCount() {

    return getModuleCount();
}

const char *ModuleManager::getItemName( uint8_t row) {

    Module *mod = getModule( row);
    return mod->getName();
}

uint8_t ModuleManager::getValueCount() {

    return 0;
}

TableEditType_t ModuleManager::getValueType( uint8_t col) {

    return INT8_T;
}

void ModuleManager::getValue( uint8_t row, uint8_t col, Cell *cell) {
    // Nothing
}

void ModuleManager::setValue( uint8_t row, uint8_t col, Cell *cell) {
    // Nothing
}

