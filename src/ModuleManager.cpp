/*
  TXos. A remote control transmitter OS.

  MIT License

  Copyright (c) 2023 wlowi

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include "ModuleManager.h"
#include "TextUI.h"
#include "HomeScreen.h"

extern HomeScreen *homeScreen;

ModuleManager::ModuleManager( ConfigBlock &svc) : blockService( &svc) {

    modelMenu->addScreen( systemMenu);
}

/*
 * Add a Screen.
 */
void ModuleManager::addToSystemMenu( TextUIScreen *scr) {

    systemMenu->addScreen( scr);
}

void ModuleManager::addToModelMenu( TextUIScreen *scr) {

    modelMenu->addScreen( scr);
}

TextUIMenu *ModuleManager::getSystemMenu() {

    return systemMenu;
}

TextUIMenu *ModuleManager::getModelMenu() {

    return modelMenu;
}

void ModuleManager::addToSystemSetAndMenu( Module *modulePtr) {

    modulePtr->setNext = nullptr;

    if( systemSetFirst == nullptr) {
        systemSetFirst = systemSetLast = modulePtr;
    } else {
        systemSetLast->setNext = modulePtr;
        systemSetLast = modulePtr;
    }

    addToSystemMenu( modulePtr);
}

void ModuleManager::addToModelSetAndMenu( Module *modulePtr) {

    modulePtr->setNext = nullptr;

    if( modelSetFirst == nullptr) {
        modelSetFirst = modelSetLast = modulePtr;
    } else {
        modelSetLast->setNext = modulePtr;
        modelSetLast = modulePtr;
    }

    addToModelMenu( modulePtr);
}

bool ModuleManager::inSystemSet( Module *modulePtr)
{
    Module *m = systemSetFirst;

    while( m != nullptr)
    {
        if( m == modulePtr) return true;
        m = m->setNext;
    }

    return false;
}

bool ModuleManager::inModelSet( Module *modulePtr)
{
    Module *m = modelSetFirst;

    while( m != nullptr)
    {
        if( m == modulePtr) return true;
        m = m->setNext;
    }

    return false;
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

Module *ModuleManager::getModuleByType( uint8_t setType, moduleType_t type) {

    Module *current = nullptr;
    
    if( setType == MODULE_SET_MODEL) {
        current = modelSetFirst;
    } else if( setType == MODULE_SET_SYSTEM) {
        current = systemSetFirst;
    }

    while( current != nullptr) {
        if( current->getConfigType() == type) { 
            break;
        }
        current = current->setNext;
    }

    return current;
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

    Module *current = modelSetFirst;

    while( current != nullptr) {
        current->switchPhase( phase);
        current = current->setNext;
    }
}

void ModuleManager::setModelDefaults() {

    LOG("\nModuleManager::setModelDefaults():\n");

    Module *current = modelSetFirst;

    while( current != nullptr) {
        current->setDefaults();
        current = current->setNext;
    }
}

void ModuleManager::setSystemDefaults() {

    LOG("\nModuleManager::setSystemDefaults():\n");

    Module *current = systemSetFirst;

    while( current != nullptr) {
        current->setDefaults();
        current = current->setNext;
    }
}

void ModuleManager::initModel() {

    LOG("\nModuleManager::initModel():\n");

    Module *current = modelSetFirst;

    while( current != nullptr) {
        current->init();
        current = current->setNext;
    }
}

uint8_t ModuleManager::getModelCount() const {

    return blockService->getModelBlockCount();
}
/*
 * Read configuration block from EEPROM and distribute
 * configuration data to each module.
 */
void ModuleManager::loadModel( configBlockID_t modelID) {

    LOGV("\nModuleManager::loadModel(): loading model %d\n", modelID);

    setModelDefaults();

    blockService->readBlock( modelID);

    if( blockService->isBlockValid()) {
        parseBlock( MODULE_SET_MODEL);
    } else {
        LOGV("** ModuleManager::loadModel(): Block %d is invalid.\n", modelID);
        homeScreen->postMessage( 1, MSG_MODEL_LOAD_FAILED);
        saveModel( modelID);
    }

    initModel();
}

/*
 * Generate configuration block for all modules and write to EEPROM.
 */
void ModuleManager::saveModel( configBlockID_t modelID) {

    LOGV("\nModuleManager::saveModel(): saving model %d\n", modelID);

    generateBlock( modelID, MODULE_SET_MODEL);
    blockService->writeBlock();
}


/*
 * Load and parse system configuration.
 */
void ModuleManager::loadSystemConfig(configBlockID_t blockID) {

    LOG("\nModuleManager::loadSystemConfig(): loading system config\n");

    blockService->readBlock( blockID);

    if( blockService->isBlockValid()) {
        parseBlock( MODULE_SET_SYSTEM);
    } else {
        LOG("** ModuleManager::loadSystemConfig(): system config is invalid, saving defaults\n");
        homeScreen->postMessage( 1, MSG_BAD_SYSCONFIG);
        setSystemDefaults();
        saveSystemConfig( blockID);
    }
}

/*
 * Generate and store system configuration.
 */
void ModuleManager::saveSystemConfig(configBlockID_t blockID) {

    LOG("\nModuleManager::saveSystemConfig(): saving system config\n");

    generateBlock( blockID, MODULE_SET_SYSTEM);
    blockService->writeBlock();
}
/*
 * Parse data in configuration block and distribute data to 
 * each module.
 */
void ModuleManager::parseBlock( uint8_t setType) {

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

        current = getModuleByType( setType, type);

        if( current == nullptr) {
            LOGV("** ModuleManager::parseBlock(): No module of type=%d\n", type);
            payload += size;
            totalSize += size;

        } else if( current->getConfigSize() != size) {
            LOGV("** ModuleManager::parseBlock(): Config size mismatch of module type=%d %d != %d\n",
                type, current->getConfigSize(), size);
            homeScreen->postMessage( 1, MSG_CONFIG_SIZE);
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


void ModuleManager::generateBlock( configBlockID_t modelID, uint8_t setType) {

    uint8_t *payload;
    moduleType_t type;
    moduleSize_t size;
    uint16_t totalSize = 0;
    size_t payloadSize;
    Module *current = nullptr;

    if( setType == MODULE_SET_MODEL) {
        current = modelSetFirst;
    } else if( setType == MODULE_SET_SYSTEM) {
        current = systemSetFirst;
    }

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

        current = current->setNext;
    }

    // Terminating invalid module type
    type = MODULE_INVALID_TYPE;
    PUT( (uint8_t*)&type, sizeof(moduleType_t));

    LOGV("ModuleManager::generateBlock(): Payload %d bytes\n", totalSize);
}
