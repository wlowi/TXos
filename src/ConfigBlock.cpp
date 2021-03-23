/*
 * ConfigBlock.cpp
 */

#include "ConfigBlock.h"

ConfigBlock::ConfigBlock() {
    
    blockId = CONFIG_INVALID_BLOCK;
}

uint8_t ConfigBlock::readBlock( uint8_t id) {

    void *configStart;

    if( id < CONFIG_INVALID_BLOCK) {
        
        blockId = id;
        configStart = (void*)(EEPROM_START_LOC + (blockId * sizeof(configBlock_t)));
        eeprom_read_block( &block, configStart, sizeof(configBlock_t));

        if( !isBlockValid()) {
            return CONFIGBLOCK_RC_CSUM;
        }

        return CONFIGBLOCK_RC_OK;
    }
    
    return CONFIGBLOCK_RC_INVID;
}

uint8_t ConfigBlock::formatBlock( uint8_t id) {

    if( id < CONFIG_INVALID_BLOCK) {

        blockId = id;
        for( uint8_t i = 0; i < CONFIG_BLOCK_PAYLOAD_SIZE; i++) {
            block.mem[i] = 0;
        }
        
        return CONFIGBLOCK_RC_OK;        
    }
    
    return CONFIGBLOCK_RC_INVID;  
}

uint8_t ConfigBlock::writeBlock() {
    
    void *configStart;

    if( blockId < CONFIG_INVALID_BLOCK) {

        configStart = (void*)(EEPROM_START_LOC + (blockId * sizeof(configBlock_t)));
        block.checksum = computeChecksum();
        eeprom_update_block( &block, configStart, sizeof(configBlock_t));
        
        return CONFIGBLOCK_RC_OK;
    }
    
    return CONFIGBLOCK_RC_INVID;
}

uint8_t *ConfigBlock::getMemoryBlock() {
    
    return block.mem;
}

/* ***** PRIVATE ***** */

/* Compute and return config checksum.
 */
checksum_t ConfigBlock::computeChecksum()
{
    checksum_t checksum = 0;
    byte *p = (byte*)&block;

    for( uint16_t i = 0; i < CONFIG_BLOCK_PAYLOAD_SIZE; i++)
    {
        checksum = rotate(checksum);
        checksum ^= (i ^ ((byte)*(p+i)));
    }

    return checksum;
}

/* Left rotate unsigned int value.
 */
checksum_t ConfigBlock::rotate( checksum_t v)
{
    return ((v >> 15) & 1) | (v << 1);
}

boolean ConfigBlock::isBlockValid()
{
    return block.checksum == computeChecksum();
}
