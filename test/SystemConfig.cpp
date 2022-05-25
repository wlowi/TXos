
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

void SystemConfig::setModelID( configBlockID_t model) {

    cfg.modelID = model;
    save();
}

/* private */

void SystemConfig::parseBlock() {

    if( sizeof( cfg) <= CONFIG_PAYLOAD_SIZE) {
        blockService->memcpy( (uint8_t*)&cfg, blockService->getPayload(), sizeof(cfg));
        verify();
    } else {
        LOG( "** SystemConfig::parseBlock(): Config size > CONFIG_PAYLOAD_SIZE\n", NULL);
    }
}

void SystemConfig::generateBlock() {

    verify();

    blockService->formatBlock( SYSTEMCONFIG_BLOCKID);

    if( sizeof( cfg) <= CONFIG_PAYLOAD_SIZE) {
        blockService->memcpy( blockService->getPayload(), (uint8_t*)&cfg, sizeof(cfg));
    } else {
        LOG( "** SystemConfig::generateBlock(): Config size > CONFIG_PAYLOAD_SIZE\n", NULL);
    }
}

void SystemConfig::setDefaults() {

    cfg.modelID = (configBlockID_t)1;
}

void SystemConfig::verify() {

    if( cfg.modelID < 1 || cfg.modelID > CONFIG_MODEL_COUNT) {
        LOG( "** SystemConfig::verify(): Failed. invalid modelID=%d\n", cfg.modelID);
        cfg.modelID = 1;
    }
}
