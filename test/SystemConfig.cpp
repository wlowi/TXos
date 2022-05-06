
#include "SystemConfig.h"

SystemConfig::SystemConfig(ConfigBlock &svc) {

    blockService = &svc;
}

void SystemConfig::load() {

    blockService->readBlock( SYSTEMCONFIG_BLOCKID);

    if( blockService->isBlockValid()) {
        parseBlock();
    } else {
        setDefaults();
        save();
    }
}

void SystemConfig::save() {

    generateBlock();

    blockService->writeBlock();
}

configBlockID_t SystemConfig::getModelID() {

    return cfg.modelID;
}

/* private */

void SystemConfig::parseBlock() {

    if( sizeof( cfg) <= CONFIG_PAYLOAD_SIZE) {
        blockService->memcpy( (uint8_t*)&cfg, blockService->getPayload(), sizeof(cfg));
    } else {
        // printf("ERROR: SystemConfig::parseBlock(): Config size > CONFIG_PAYLOAD_SIZE\n");
    }
}

void SystemConfig::generateBlock() {

    blockService->formatBlock( SYSTEMCONFIG_BLOCKID);

    if( sizeof( cfg) <= CONFIG_PAYLOAD_SIZE) {
        blockService->memcpy( blockService->getPayload(), (uint8_t*)&cfg, sizeof(cfg));
    } else {
        // printf("ERROR: SystemConfig::generateBlock(): Config size > CONFIG_PAYLOAD_SIZE\n");
    }
}

void SystemConfig::setDefaults() {

    cfg.modelID = (configBlockID_t)1;
}
