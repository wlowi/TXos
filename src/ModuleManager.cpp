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
#include "ImportExport.h"
#include "Comm.h"
#include "TextUI.h"
#include "HomeScreen.h"

extern HomeScreen* homeScreen;

ModuleManager::ModuleManager(ConfigBlock& svc) : blockService(&svc) {

    modelMenu->addScreen(systemMenu);
}

/*
 * Add a Screen.
 */
void ModuleManager::addToSystemMenu(TextUIScreen* scr) {

    systemMenu->addScreen(scr);
}

void ModuleManager::addToModelMenu(TextUIScreen* scr) {

    modelMenu->addScreen(scr);
}

TextUIMenu* ModuleManager::getSystemMenu() {

    return systemMenu;
}

TextUIMenu* ModuleManager::getModelMenu() {

    return modelMenu;
}

void ModuleManager::addToSystemSetAndMenu(Module* modulePtr) {

    modulePtr->setNext = nullptr;

    if (systemSetFirst == nullptr) {
        systemSetFirst = systemSetLast = modulePtr;
    }
    else {
        systemSetLast->setNext = modulePtr;
        systemSetLast = modulePtr;
    }

    addToSystemMenu(modulePtr);
}

void ModuleManager::addToModelSetAndMenu(Module* modulePtr) {

    modulePtr->setNext = nullptr;

    if (modelSetFirst == nullptr) {
        modelSetFirst = modelSetLast = modulePtr;
    }
    else {
        modelSetLast->setNext = modulePtr;
        modelSetLast = modulePtr;
    }

    addToModelMenu(modulePtr);
}

bool ModuleManager::inSystemSet(Module* modulePtr) {
    Module* m = systemSetFirst;

    while (m != nullptr) {
        if (m == modulePtr) return true;
        m = m->setNext;
    }

    return false;
}

bool ModuleManager::inModelSet(Module* modulePtr) {
    Module* m = modelSetFirst;

    while (m != nullptr) {
        if (m == modulePtr) return true;
        m = m->setNext;
    }

    return false;
}

void ModuleManager::addToRunList(Module* modulePtr) {

    modulePtr->runlistNext = nullptr;

    if (runlistFirst == nullptr) {
        runlistFirst = runlistLast = modulePtr;
    }
    else {
        runlistLast->runlistNext = modulePtr;
        runlistLast = modulePtr;
    }
}

Module* ModuleManager::getModuleByType(uint8_t setType, moduleType_t type) {

    Module* current = nullptr;

    if (setType == MODULE_SET_MODEL) {
        current = modelSetFirst;
    }
    else if (setType == MODULE_SET_SYSTEM) {
        current = systemSetFirst;
    }

    while (current != nullptr) {
        if (current->getConfigType() == type) {
            break;
        }
        current = current->setNext;
    }

    return current;
}

Module* ModuleManager::getModuleByCommType(uint8_t setType, nameType_t type) {

    Module* current = nullptr;

    if (setType == MODULE_SET_MODEL) {
        current = modelSetFirst;
    }
    else if (setType == MODULE_SET_SYSTEM) {
        current = systemSetFirst;
    }

    while (current != nullptr) {
        if (current->getCommType() == type) {
            break;
        }
        current = current->setNext;
    }

    return current;
}

/* Copy memory form "payload" into "p".
 * "p" must be a pointer to memory.
 * The block is of size "s".
 * "payload" is advanced and totalsize increased by "s"
 */
#define GET( p, s)                              \
    do {                                        \
        blockService->memcpy( p, payload, s);   \
        payload += s;                           \
        totalSize += s;                         \
    } while( 0 )

/* Copy memory from "p" to "payload".
 */
#define PUT( p, s)                              \
    do {                                        \
        blockService->memcpy( payload, p, s);   \
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
uint8_t ModuleManager::parseModule(configBlockID_t modelID, Module& moduleRef) {

    uint8_t rc;
    const uint8_t* payload;
    moduleType_t type;
    moduleSize_t size;
    uint16_t totalSize = 0;

    rc = blockService->readBlock(modelID);
    if (rc != CONFIGBLOCK_RC_OK) {
        return rc;
    }

    rc = CONFIGBLOCK_RC_INVID;
    payload = blockService->getPayload();
    GET((uint8_t*)&type, sizeof(moduleType_t));

    while (type != MODULE_INVALID_TYPE) {

        GET((uint8_t*)&size, sizeof(moduleSize_t));

        LOGV("ModuleManager::parseModule(): GET type=%d size=%d\n", type, size);

        if (type == moduleRef.getConfigType()) {
            GET(moduleRef.getConfig(), size);
            rc = CONFIGBLOCK_RC_OK;
            break;
        }
        else {
            payload += size;
            totalSize += size;
        }

        GET((uint8_t*)&type, sizeof(moduleType_t));
    }

    return rc;
}

/*
 * Call the run() method of each module in the list
 * and pass the reference to the channel set.
 */
void ModuleManager::runModules(Controls& controls) {

    Module* current = runlistFirst;

    while (current != nullptr) {
        current->run(controls);
        current = current->runlistNext;
    }
}

/* Notify all modules about a phase switch.
 */
void ModuleManager::switchPhase(phase_t phase) {

    LOGV("\nModuleManager::switchPhase(): new phase %d\n", phase);
    /* System modules do not have phases. */

    Module* current = modelSetFirst;

    while (current != nullptr) {
        current->switchPhase(phase);
        current = current->setNext;
    }
}

/* Set default values for all modules.
 */
void ModuleManager::setModelDefaults() {

    LOG("\nModuleManager::setModelDefaults():\n");

    Module* current = modelSetFirst;

    while (current != nullptr) {
        current->setDefaults();
        current = current->setNext;
    }
}

/* Set defaults for system config block.
 */
void ModuleManager::setSystemDefaults() {

    LOG("\nModuleManager::setSystemDefaults():\n");

    Module* current = systemSetFirst;

    while (current != nullptr) {
        current->setDefaults();
        current = current->setNext;
    }
}

/* Initialize all modules.
 * This is called after model load.
 */
void ModuleManager::initModel() {

    LOG("\nModuleManager::initModel():\n");

    Module* current = modelSetFirst;

    while (current != nullptr) {
        current->init();
        current = current->setNext;
    }
}

uint8_t ModuleManager::getModelCount() const {

    return blockService->getModelBlockCount();
}

void ModuleManager::removeModel( configBlockID_t modelID) {

    LOGV("ModuleManager::removeModel(): modelID=%d\n", modelID);

    if (modelID > 0 && modelID <= blockService->getModelBlockCount()) {
        blockService->formatBlock(modelID);
        blockService->writeBlock();
    }
}

void ModuleManager::copyModel( configBlockID_t fromModelID, configBlockID_t toModelID) {

    LOGV("ModuleManager::copyModel(): fromModelID=%d toModelID=%d\n", fromModelID, toModelID);

    if (fromModelID > 0 && fromModelID <= blockService->getModelBlockCount()
        && toModelID > 0 && toModelID <= blockService->getModelBlockCount())
    {
        blockService->readBlock(fromModelID);

        if (blockService->isBlockValid() && blockService->setBlockID(toModelID)) {
            blockService->writeBlock();
        }
    }
}

/* Load a model from EEPROM.
 *
 * Read configuration block from EEPROM and distribute
 * configuration data to each module.
 */
void ModuleManager::loadModel(configBlockID_t modelID) {

    LOGV("\nModuleManager::loadModel(): loading model %d\n", modelID);

    setModelDefaults();

    blockService->readBlock(modelID);

    if (blockService->isBlockValid()) {
        parseBlock(MODULE_SET_MODEL);
    }
    else {
        LOGV("** ModuleManager::loadModel(): Block %d is invalid.\n", modelID);
        homeScreen->postMessage(1, MSG_MODEL_LOAD_FAILED);
        saveModel(modelID);
    }

    initModel();
}

/*
 * Generate configuration block for all modules and write to EEPROM.
 */
void ModuleManager::saveModel(configBlockID_t modelID) {

    LOGV("\nModuleManager::saveModel(): saving model %d\n", modelID);

    generateBlock(modelID, MODULE_SET_MODEL);
    blockService->writeBlock();
}


/*
 * Load and parse system configuration.
 */
void ModuleManager::loadSystemConfig(configBlockID_t blockID) {

    LOG("\nModuleManager::loadSystemConfig(): loading system config\n");

    blockService->readBlock(blockID);

    if (blockService->isBlockValid()) {
        parseBlock(MODULE_SET_SYSTEM);
    }
    else {
        LOG("** ModuleManager::loadSystemConfig(): system config is invalid, saving defaults\n");
        homeScreen->postMessage(1, MSG_BAD_SYSCONFIG);
        setSystemDefaults();
        saveSystemConfig(blockID);
    }
}

/*
 * Generate and store system configuration.
 */
void ModuleManager::saveSystemConfig(configBlockID_t blockID) {

    LOG("\nModuleManager::saveSystemConfig(): saving system config\n");

    generateBlock(blockID, MODULE_SET_SYSTEM);
    blockService->writeBlock();
}

void ModuleManager::exportSystemConfig(ImportExport* exporter) {

    LOG("ModuleManager::exportSystemConfig(): called\n");

    uint8_t rc;
    const uint8_t* payload;
    moduleType_t type;
    moduleSize_t size;
    uint16_t totalSize;
    uint8_t buffer[64];
    const Module* module;
    Comm& comm = exporter->getComm();

    /* Block 0 is system config. Model blocks start at 1 */
    rc = blockService->readBlock(0);
    if (rc != CONFIGBLOCK_RC_OK) {
        /* This may be an uninitialized block. */
        LOG("** ModuleManager::exportSystemConfig(): Failed to read system config block\n");
        return;
    }
    else {
        LOG("ModuleManager::exportSystemConfig(): Read system config block \n");
    }

    payload = blockService->getPayload();
    totalSize = 0;

    GET((uint8_t*)&type, sizeof(moduleType_t));

    comm.open(COMM_PACKET_SYSCONFIG);

    while (type != MODULE_INVALID_TYPE) {
        GET((uint8_t*)&size, sizeof(moduleSize_t));

        LOGV("ModuleManager::exportSystemConfig(): GET type=%d size=%d\n", type, size);

        GET(buffer, size);

        module = getModuleByType(MODULE_SET_SYSTEM, type);
        if (module == nullptr) {
            LOGV("** ModuleManager::exportSystemConfig(): Failed to get module of type=%d\n", type);
        }
        else {
            module->exportConfig(exporter, buffer);
            comm.writePart();
            yieldLoop();
        }

        GET((uint8_t*)&type, sizeof(moduleType_t));
    }

    comm.close();
    comm.write();
}

void ModuleManager::importSystemConfig(ImportExport* importer) {

    LOG("ModuleManager::importSystemConfig(): called\n");
}

/* Export all models. */
void ModuleManager::exportModels(ImportExport* exporter) {

    LOG("ModuleManager::exportModels(): called\n");

    uint8_t modelCount = getModelCount();

    /* Block 0 is system config. Model blocks start at 1 */
    for (configBlockID_t modelID = 1; modelID <= modelCount; modelID++) {
        exportModel(exporter, modelID);
    }
}

void ModuleManager::exportModel(ImportExport* exporter, configBlockID_t modelID) {

    LOGV("ModuleManager::exportModel( modelID=%d): called\n", modelID);

    uint8_t rc;
    const uint8_t* payload;
    moduleType_t type;
    moduleSize_t size;
    uint16_t totalSize;
    uint8_t buffer[64];
    const Module* module;
    Comm& comm = exporter->getComm();

    rc = blockService->readBlock(modelID);
    if (rc != CONFIGBLOCK_RC_OK) {
        /* This may be an uninitialized block. */
        LOGV("** ModuleManager::exportModel(): Failed to read block %d\n", modelID);
        return;
    }
    else {
        LOGV("ModuleManager::exportModel(): Read model block %d\n", modelID);
    }

    payload = blockService->getPayload();
    totalSize = 0;

    GET((uint8_t*)&type, sizeof(moduleType_t));

    comm.open(COMM_PACKET_MODELCONFIG);
    comm.addUInt8(COMM_FIELD_ID, modelID);

    while (type != MODULE_INVALID_TYPE) {
        GET((uint8_t*)&size, sizeof(moduleSize_t));

        LOGV("ModuleManager::exportModels(): GET type=%d size=%d\n", type, size);

        GET(buffer, size);

        module = getModuleByType(MODULE_SET_MODEL, type);
        if (module == nullptr) {
            LOGV("** ModuleManager::exportModels(): Failed to get module of type=%d\n", type);
        }
        else {
            module->exportConfig(exporter, buffer);
            comm.writePart();
            yieldLoop();
        }

        GET((uint8_t*)&type, sizeof(moduleType_t));
    }

    comm.close();
    comm.write();
}

void ModuleManager::importModel(ImportExport* importer) {

    nameType_t cmd;
    char dType;
    uint8_t width;
    uint16_t count;
    COMM_RC_t rc;
    Module* module;
    uint8_t* payload;
    size_t payloadSize;
    uint16_t totalSize = 0;
    moduleType_t type;
    moduleSize_t size;
    uint8_t errCode = 0;
    Comm& comm = importer->getComm();

    configBlockID_t modelID = CONFIG_BLOCKID_INVALID;

    LOG("ModuleManager::importModel(): called\n");

    /* First read model ID */
    if ((rc = comm.nextField(&cmd, &dType, &width, &count)) == COMM_RC_OK) {
        if (cmd == COMM_FIELD_ID) {
            comm.nextData(&modelID, dType, width, count);
        }
        else {
            LOG("ModuleManager::importModel(): ERROR: COMM_FIELD_ID expected\n");
            errCode = 1;
        }
    }
    else {
        LOG("ModuleManager::importModel(): ERROR: Field expected\n");
        errCode = 2;
    }

    LOGV("ModuleManager::importModel(): modelID=%d\n", modelID);

    if (modelID > 0 && modelID <= blockService->getModelBlockCount()) {

        blockService->formatBlock(modelID);

        payload = blockService->getPayload();
        payloadSize = blockService->getPayloadSize();

        while ((rc = comm.nextField(&cmd, &dType, &width, &count)) == COMM_RC_SUBSTART) {
            if ((rc = comm.nextPacket(&cmd)) == COMM_RC_OK) {
                LOGV("ModuleManager::importModel(): substart name=%c%c\n", PACKET_NAME(cmd));
                module = getModuleByCommType(MODULE_SET_MODEL, cmd);
                if (module == nullptr) {
                    LOG("** ModuleManager::importModel(): ERROR: no module found\n");
                    errCode = 3;
                }
                else {
                    size = module->getConfigSize();

                    if (size > 0) { // Do not store module config of size 0

                        type = module->getConfigType();

                        // +1 is for terminating invalid module type
                        if (totalSize + sizeof(moduleType_t) + sizeof(moduleSize_t) + size + 1 <= payloadSize) {
                            LOGV("ModuleManager::importModel(): Put type=%d size=%d\n", type, size);

                            PUT((uint8_t*)&type, sizeof(moduleType_t));
                            PUT((uint8_t*)&size, sizeof(moduleSize_t));

                            rc = module->importConfig(importer, payload);

                            if (rc != COMM_RC_END) {
                                errCode = 4;
                                break; // Error condition
                            }

                            payload += size;
                            totalSize += size;

                        }
                        else {
                            LOGV("** ModuleManager::importModel(): ERROR: Payload to large. %u > %u\n",
                                totalSize + sizeof(moduleType_t) + sizeof(moduleSize_t) + size,
                                payloadSize);
                            errCode = 5;
                        }
                    }
                }
            }
        }

        // Terminating invalid module type
        type = MODULE_INVALID_TYPE;
        PUT((uint8_t*)&type, sizeof(moduleType_t));

        if (!errCode) {
            blockService->writeBlock();
            LOGV("** ModuleManager::importModel(): model config saved. %d bytes\n", totalSize);
        }
    }
    else {
        LOG("ModuleManager::importModel(): ERROR: Invalid modelID\n");
        errCode = 6;
    }

    if (errCode) {
        homeScreen->postMessage(1, MSG_MODEL_IMP_FAILED, errCode);
    }
}

/*
 * Parse data in configuration block and distribute data to
 * each module.
 */
void ModuleManager::parseBlock(uint8_t setType) {

    const uint8_t* payload;
    moduleType_t type;
    moduleSize_t size;
    Module* current;
    uint16_t totalSize = 0;

    payload = blockService->getPayload();

    GET((uint8_t*)&type, sizeof(moduleType_t));

    while (type != MODULE_INVALID_TYPE) {

        GET((uint8_t*)&size, sizeof(moduleSize_t));

        if( type == 0xff) {
            LOG("ModuleManager::parseBlock(): ERROR: uninitialized memory\n");
            break;
        }

        if( totalSize + size > blockService->getPayloadSize()) {
            LOGV("ModuleManager::parseBlock(): ERROR: size > payloadSize %u > %u\n", totalSize + size, blockService->getPayloadSize());
            break;
        }

        LOGV("ModuleManager::parseBlock(): GET type=%d size=%d\n", type, size);

        current = getModuleByType(setType, type);

        if (current == nullptr) {
            LOGV("** ModuleManager::parseBlock(): ERROR: No module of type=%d\n", type);
            payload += size;
            totalSize += size;

        }
        else if (current->getConfigSize() != size) {
            LOGV("** ModuleManager::parseBlock(): ERROR: Config size mismatch of module type=%d %d != %d\n",
                type, current->getConfigSize(), size);
            homeScreen->postMessage(1, MSG_CONFIG_SIZE);
            payload += size;
            totalSize += size;

        }
        else {
            GET(current->getConfig(), size);
        }

        GET((uint8_t*)&type, sizeof(moduleType_t));
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
 *
 * Generate a system or model config block from active modules.
 *
 * The setType must be MODULE_SET_MODEL or MODULE_SET_SYSTEM and
 * defines which module set is used to generate the block.
 *
 * The modelID must be SYSTEMCONFIG_BLOCKID for the system config block
 * or avalid model is ( 1 .. modelBlockCount )
 */
void ModuleManager::generateBlock(configBlockID_t modelID, uint8_t setType) {

    uint8_t* payload;
    moduleType_t type;
    moduleSize_t size;
    uint16_t totalSize = 0;
    size_t payloadSize;
    Module* module = nullptr;

    if (setType == MODULE_SET_MODEL) {
        module = modelSetFirst;
    }
    else if (setType == MODULE_SET_SYSTEM) {
        module = systemSetFirst;
    }

    blockService->formatBlock(modelID);

    payload = blockService->getPayload();
    payloadSize = blockService->getPayloadSize();

    while (module != nullptr) {

        size = module->getConfigSize();

        if (size > 0) { // Do not store module config of size 0

            type = module->getConfigType();

            // +1 is for terminating invalid module type
            if (totalSize + sizeof(moduleType_t) + sizeof(moduleSize_t) + size + 1 <= payloadSize) {
                LOGV("ModuleManager::generateBlock(): Put type=%d size=%d\n", type, size);

                PUT((uint8_t*)&type, sizeof(moduleType_t));
                PUT((uint8_t*)&size, sizeof(moduleSize_t));
                PUT(module->getConfig(), size);

            }
            else {
                LOGV("** ModuleManager::generateBlock(): Payload to large. %u > %u\n",
                    totalSize + sizeof(moduleType_t) + sizeof(moduleSize_t) + size,
                    payloadSize);
#ifdef __linux__
                printf("** ModuleManager::generateBlock(): Payload to large. %lu > %lu\n",
                    totalSize + sizeof(moduleType_t) + sizeof(moduleSize_t) + size,
                    payloadSize);
#endif
            }
        }

        module = module->setNext;
    }

    // Terminating invalid module type
    type = MODULE_INVALID_TYPE;
    PUT((uint8_t*)&type, sizeof(moduleType_t));

    LOGV("ModuleManager::generateBlock(): Payload %d bytes\n", totalSize);
#ifdef __linux__
    printf("ModuleManager::generateBlock(): Payload %d bytes\n", totalSize);
#endif
}
