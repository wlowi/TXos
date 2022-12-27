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

#include "ModuleManager.h"
#include "UserInterface.h"

extern UserInterface userInterface;

ModuleManager::ModuleManager( ConfigBlock &svc) : blockService( &svc) {

}

/*
 * Add a module.
 */
void ModuleManager::addToSystemMenu( Module *modulePtr) {

    systemMenu->addModule( modulePtr);
}

void ModuleManager::addToModelMenu( Module *modulePtr) {

    modelMenu->addModule( modulePtr);
}

Menu *ModuleManager::getSystemMenu() {

    return systemMenu;
}

Menu *ModuleManager::getModelMenu() {

    return modelMenu;
}

void ModuleManager::addToRunList( Module *modulePtr) {

    modulePtr->runlistNext = nullptr;

    if( runlistFirst == nullptr) {
        runlistFirst = runlistLast = modulePtr;
    } else {
        runlistLast->runlistNext = modulePtr;
        runlistLast = modulePtr;
    }
}


#define GET( p, s)                              \
    do {                                        \
        blockService->memcpy( p, payload, s);   \
        payload += s;                           \
        totalSize += s;                         \
    } while( 0 )

/*
 * Called from ModelSelect.getValue() to diplay a list of model names
 * for all configuration blocks stored in the EEPROM.
 * 
 * It reads a configuration block and extracts the configuration data for
 * the module "modelRef". This is of class "Model" for the call from 
 * ModelSelect.getValue().
 */
uint8_t ModuleManager::parseModule( configBlockID_t modelID, Module &moduleRef) {

    uint8_t rc;
    const uint8_t *payload;
    moduleType_t type;
    moduleSize_t size;
    uint16_t totalSize = 0;

    rc = blockService->readBlock( modelID);
    if( rc != CONFIGBLOCK_RC_OK) {
        return rc;
    }

    rc = CONFIGBLOCK_RC_INVID;
    payload = blockService->getPayload();
    GET( (uint8_t*)&type, sizeof(moduleType_t));

    while( type != MODULE_INVALID_TYPE) {

        GET( (uint8_t*)&size, sizeof(moduleSize_t));

        LOGV("ModuleManager::parseModule(): GET type=%d size=%d\n", type, size);

        if( type == moduleRef.getConfigType()) {
            GET( moduleRef.getConfig(), size);
            rc = CONFIGBLOCK_RC_OK;
            break;
        } else {
            payload += size;
            totalSize += size;
        }

        GET( (uint8_t*)&type, sizeof(moduleType_t));
    }

    return rc;
}

/*
 * Call the run() method of each module in the list
 * and pass the reference to the channel set.
 */
void ModuleManager::runModules( Controls &controls) {

    Module *current = runlistFirst;

    while( current != nullptr) {
        current->run( controls);
        current = current->runlistNext;
    }
}

void ModuleManager::switchPhase( phase_t phase) {

    LOGV("\nModuleManager::switchPhase(): new phase %d\n", phase);
    /* System modules do not have phases. */
    modelMenu->switchPhase( phase);
}

/*
 * Read configuration block from EEPROM and distribute
 * configuration data to each module.
 */
void ModuleManager::loadModel( configBlockID_t modelID) {

    LOGV("\nModuleManager::loadModel(): loading model %d\n", modelID);

    modelMenu->setDefaults();

    blockService->readBlock( modelID);

    if( blockService->isBlockValid()) {
        parseBlock( modelMenu);
    } else {
        LOGV("** ModuleManager::loadModel(): Block %d is invalid.\n", modelID);
        userInterface.postMessage( 1, MSG_MODEL_LOAD_FAILED);
        saveModel( modelID);
    }

    modelMenu->init();
}

/*
 * Generate configuration block for all modules and write to EEPROM.
 */
void ModuleManager::saveModel( configBlockID_t modelID) {

    LOGV("\nModuleManager::saveModel(): saving model %d\n", modelID);

    generateBlock( modelID, modelMenu);
    blockService->writeBlock();
}


/*
 * Load and parse system configuration.
 */
void ModuleManager::loadSystemConfig(configBlockID_t blockID) {

    LOG("\nModuleManager::loadSystemConfig(): loading system config\n");

    blockService->readBlock( blockID);

    if( blockService->isBlockValid()) {
        parseBlock( systemMenu);
    } else {
        LOG("** ModuleManager::loadSystemConfig(): system config is invalid, saving defaults\n");
        userInterface.postMessage( 1, MSG_BAD_SYSCONFIG);
        systemMenu->setDefaults();
        saveSystemConfig( blockID);
    }
}

/*
 * Generate and store system configuration.
 */
void ModuleManager::saveSystemConfig(configBlockID_t blockID) {

    LOG("\nModuleManager::saveSystemConfig(): saving system config\n");

    generateBlock( blockID, systemMenu);
    blockService->writeBlock();
}
/*
 * Parse data in configuration block and distribute data to 
 * each module.
 */
void ModuleManager::parseBlock( Menu *menu) {

    const uint8_t *payload;
    moduleType_t type;
    moduleSize_t size;
    Module *current;
    uint16_t totalSize = 0;

    payload = blockService->getPayload();

    GET( (uint8_t*)&type, sizeof(moduleType_t));

    while( type != MODULE_INVALID_TYPE) {

        GET( (uint8_t*)&size, sizeof(moduleSize_t));

        LOGV("ModuleManager::parseBlock(): GET type=%d size=%d\n", type, size);

        current = menu->getModuleByType( type);

        if( current == nullptr) {
            LOGV("** ModuleManager::parseBlock(): No module of type=%d\n", type);
            payload += size;
            totalSize += size;

        } else if( current->getConfigSize() != size) {
            LOGV("** ModuleManager::parseBlock(): Config size mismatch of module type=%d %d != %d\n",
                type, current->getConfigSize(), size);
            userInterface.postMessage( 1, MSG_CONFIG_SIZE);
            payload += size;
            totalSize += size;
            
        } else {
            GET( current->getConfig(), size);
        }

        GET( (uint8_t*)&type, sizeof(moduleType_t));
    }

    LOGV("ModuleManager::parseBlock(): Total bytes read=%d\n", totalSize);
}

/*
 * Walk through all modules and write configuration data of
 * all modules to the configuration block.
 * The block gets formatted first.
 * 
 * -----------
 * moduleType1
 * -----------
 * modulesSize1
 * -----------
 * conigData1 + Checksum
 * ...
 * -----------
 * moduleType2
 * -----------
 * modulesSize3
 * -----------
 * conigData3 + Checksum
 * ...
 * -----------
 * moduleType3
 * -----------
 * modulesSize3
 * -----------
 * conigData4 + Checksum
 * ...
 * -----------
 * moduleTypeInvalid << End Marker
 * -----------
 */

#define PUT( p, s)                              \
    do {                                        \
        blockService->memcpy( payload, p, s);   \
        payload += s;                           \
        totalSize += s;                         \
    } while( 0 )


void ModuleManager::generateBlock( configBlockID_t modelID, Menu *menu) {

    uint8_t *payload;
    moduleType_t type;
    moduleSize_t size;
    uint16_t totalSize = 0;
    size_t payloadSize;
    Module *current = menu->getFirstModule();

    blockService->formatBlock( modelID);

    payload = blockService->getPayload();
    payloadSize = blockService->getPayloadSize();

    while( current != nullptr) {
        
        size = current->getConfigSize();

        if( size > 0) { // Do not store module config of size 0

            type = current->getConfigType();

            // +1 is for terminating invalid module type
            if( totalSize + sizeof(moduleType_t) + sizeof(moduleSize_t) + size +1 <= payloadSize) {
                LOGV("ModuleManager::generateBlock(): Put type=%d size=%d\n", type, size);

                PUT( (uint8_t*)&type, sizeof(moduleType_t));
                PUT( (uint8_t*)&size, sizeof(moduleSize_t));
                PUT( current->getConfig(), size);

            } else {
                LOGV("** ModuleManager::generateBlock(): Payload to large. %ld > %ld\n",
                    totalSize + sizeof(moduleType_t) + sizeof(moduleSize_t) + size,
                    payloadSize);
            }
        }

        current = current->menuNext;
    }

    // Terminating invalid module type
    type = MODULE_INVALID_TYPE;
    PUT( (uint8_t*)&type, sizeof(moduleType_t));

    LOGV("ModuleManager::generateBlock(): Payload %d bytes\n", totalSize);
}
