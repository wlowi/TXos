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

ModuleManager::ModuleManager( ConfigBlock &svc) : blockService( &svc) {

}

/*
 * Add a module.
 */
void ModuleManager::add( Module *modulePtr) {

    modulePtr->next = nullptr;

    if( first == nullptr) {
        first = last = modulePtr;
    } else {
        last->next = modulePtr;
        last = modulePtr;
    }
}

/*
 * Return number of modules.
 * NOTE: This is stored nowhere. The method walks through the list of modules.
 *       This could be changed for efficiency.
 */
uint8_t ModuleManager::getModuleCount() {

    Module *current = first;
    uint8_t cnt = 0;

    while( current != nullptr) {
        cnt++;
        current = current->next;
    }

    return cnt;
}

/*
 * Get a module by its index in the list.
 * NOTE: This funktion returns a nullptr if idx > number of modules in the list.
 */
Module *ModuleManager::getModule( uint8_t idx) {

    Module *current = first;
    uint8_t cnt = 0;

    while( current != nullptr) {
        if( cnt == idx) {
            break;
        }
        cnt++;
        current = current->next;
    }

    return current;
}

/*
 * Return a module by type.
 * NOTE: This method returns a nullptr if there is no module of that type.
 */
Module *ModuleManager::getModuleByType( moduleType_t type) {

    Module *current = first;

    while( current != nullptr) {
        if( current->getConfigType() == type) {
            break;
        }
        current = current->next;
    }

    return current;
}

#define GET( p, s)                              \
    do {                                        \
        blockService->memcpy( p, payload, s);   \
        payload += s;                           \
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

    Module *current = first;

    while( current != nullptr) {
        current->run( controls);
        current = current->next;
    }
}

/*
 * Read configuration block from EEPROM and distribute
 * configuration data to each module.
 */
void ModuleManager::load( configBlockID_t modelID) {

    blockService->readBlock( modelID);

    if( blockService->isBlockValid()) {
        parseBlock();
    } else {
        LOGV("** ModuleManager::load(): Block %d is invalid.\n", modelID);
        setDefaults();
        save( modelID);
    }
}

/*
 * Generate configuration block for all modules and write to EEPROM.
 */
void ModuleManager::save( configBlockID_t modelID) {

    generateBlock( modelID);

    blockService->writeBlock();
}

/*
 * Parse data in configuration block and distribute data to 
 * each module.
 */
void ModuleManager::parseBlock() {

    const uint8_t *payload;
    moduleType_t type;
    moduleSize_t size;
    Module *current;

    payload = blockService->getPayload();

    GET( (uint8_t*)&type, sizeof(moduleType_t));

    while( type != MODULE_INVALID_TYPE) {

        GET( (uint8_t*)&size, sizeof(moduleSize_t));

        LOGV("ModuleManager::parseBlock(): GET type=%d size=%d\n", type, size);

        current = getModuleByType( type);
        if( current == nullptr) {
            LOGV("** ModuleManager::parseBlock(): No module of type=%d\n", type);
            break;
        }

        if( current->getConfigSize() != size) {
            LOGV("** ModuleManager::parseBlock(): Config size mismatch of module type=%d %d != %d\n",
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

void ModuleManager::generateBlock(configBlockID_t modelID) {

    uint8_t *payload;
    moduleType_t type;
    moduleSize_t size;
    moduleSize_t totalSize = 0;
    Module *current = first;

    blockService->formatBlock( modelID);

    payload = blockService->getPayload();

    while( current != nullptr) {
        
        size = current->getConfigSize();

        if( size > 0) { // Do not store module config of size 0

            type = current->getConfigType();

            // +1 is for terminating invalid module type
            if( totalSize + sizeof(moduleType_t) + sizeof(moduleSize_t) + size +1 <= CONFIG_PAYLOAD_SIZE) {
                LOGV("ModuleManager::generateBlock(): Put type=%d size=%d\n", type, size);

                PUT( (uint8_t*)&type, sizeof(moduleType_t));
                PUT( (uint8_t*)&size, sizeof(moduleSize_t));
                PUT( current->getConfig(), size);

            } else {
                LOGV("** ModuleManager::generateBlock(): Payload to large. %ld > %ld\n",
                    totalSize + sizeof(moduleType_t) + sizeof(moduleSize_t) + size,
                    CONFIG_PAYLOAD_SIZE);
            }
        }

        current = current->next;
    }

    // Terminating invalid module type
    type = MODULE_INVALID_TYPE;
    PUT( (uint8_t*)&type, sizeof(moduleType_t));

    LOGV("ModuleManager::generateBlock(): Payload %d bytes\n", totalSize);
}

/*
 * Set default configuration for all modules in the list.
 */
void ModuleManager::setDefaults() {

    Module *current = first;

    while( current != nullptr) {
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

void ModuleManager::getValue( uint8_t row, uint8_t col, Cell *cell) {
    // Nothing
}

void ModuleManager::setValue( uint8_t row, uint8_t col, Cell *cell) {
    // Nothing
}
