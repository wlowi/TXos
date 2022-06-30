/*
    TXos. A remote control transmitter OS.

    Copyright (C) 2022 Wolfgang Lohwasser

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#include "SystemConfig.h"

SystemConfig::SystemConfig( ConfigBlock &svc) : blockService( &svc){

}

/*
 * Load and parse system configuration.
 */
void SystemConfig::load() {

    blockService->readBlock( SYSTEMCONFIG_BLOCKID);

    if( blockService->isBlockValid()) {
        parseBlock();
    } else {
        setDefaults();
        save();
    }
}

/*
 * Generate and store system configuration.
 */
void SystemConfig::save() {

    generateBlock();
    blockService->writeBlock();
}

configBlockID_t SystemConfig::getModelID() const {

    return cfg.modelID;
}

void SystemConfig::setModelID( configBlockID_t model) {

    cfg.modelID = model;
    save();
}

/* private */

/*
 * Copy system configuration from block and verify the data.
 *
 * This should be called after blockService->readBlock()
 */
void SystemConfig::parseBlock() {

    if( sizeof( cfg) <= CONFIG_PAYLOAD_SIZE) {
        blockService->memcpy( (uint8_t*)&cfg, blockService->getPayload(), sizeof(cfg));
        verify();
    } else {
        LOG( "** SystemConfig::parseBlock(): Config size > CONFIG_PAYLOAD_SIZE\n", NULL);
    }
}

/*
 * Verify system configuration then format the system configuration block and 
 * copy the system configuration into the block.
 * 
 * This should be called before blockService->writeBlock()
 */
void SystemConfig::generateBlock() {

    verify();

    blockService->formatBlock( SYSTEMCONFIG_BLOCKID);

    if( sizeof( cfg) <= CONFIG_PAYLOAD_SIZE) {
        blockService->memcpy( blockService->getPayload(), (uint8_t*)&cfg, sizeof(cfg));
    } else {
        LOG( "** SystemConfig::generateBlock(): Config size > CONFIG_PAYLOAD_SIZE\n", NULL);
    }
}

/*
 * Set all system configuration data to its default.
 */
void SystemConfig::setDefaults() {

    cfg.modelID = (configBlockID_t)1;
}

/*
 * Verify system configuration. 
 * If invalid data is found, set a default value.
 */
void SystemConfig::verify() {

    if( cfg.modelID < 1 || cfg.modelID > CONFIG_MODEL_COUNT) {
        LOG( "** SystemConfig::verify(): Failed. invalid modelID=%d\n", cfg.modelID);
        cfg.modelID = 1;
    }
}
