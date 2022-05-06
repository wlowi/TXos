
#include "ConfigBlock.h"

ConfigBlock::ConfigBlock() {
    
    blockID = CONFIG_BLOCKID_INVALID;
}

uint8_t ConfigBlock::readBlock( configBlockID_t id) {

    int configStart;

    if( id > CONFIG_BLOCKID_INVALID && id < CONFIG_BLOCKS) {
        
        blockID = id;
        configStart = (blockID * CONFIG_BLOCK_SIZE);
        EEPROM.get( configStart, block);

        if( !isBlockValid()) {
            return CONFIGBLOCK_RC_CSUM;
        }

        return CONFIGBLOCK_RC_OK;
    }
    
    return CONFIGBLOCK_RC_INVID;
}

uint8_t ConfigBlock::formatBlock( configBlockID_t id) {

    if( id > CONFIG_BLOCKID_INVALID && id < CONFIG_BLOCKS) {

        blockID = id;
        for( uint8_t i = 0; i < CONFIG_PAYLOAD_SIZE; i++) {
            block.payload[i] = 0xff;
        }
        
        return CONFIGBLOCK_RC_OK;        
    }
    
    return CONFIGBLOCK_RC_INVID;  
}

uint8_t ConfigBlock::writeBlock() {
    
    int configStart;

    if( blockID > CONFIG_BLOCKID_INVALID && blockID < CONFIG_BLOCKS) {

        configStart = (blockID * CONFIG_BLOCK_SIZE);
        block.checksum = computeChecksum();
        EEPROM.put( configStart, block);
        
        return CONFIGBLOCK_RC_OK;
    }
    
    return CONFIGBLOCK_RC_INVID;
}

uint8_t *ConfigBlock::getPayload() {

    return block.payload;
}

bool ConfigBlock::isBlockValid()
{
    return block.checksum == computeChecksum();
}

void ConfigBlock::memcpy( uint8_t *dest, uint8_t *src, size_t sz) {

    while( sz--) {
        (*dest) = (*src);
        dest++;
        src++;
    }
}

/* private */

checksum_t ConfigBlock::computeChecksum()
{
    checksum_t checksum = 0;
    byte *p = (byte*)&block;

    for( uint16_t i = 0; i < CONFIG_PAYLOAD_SIZE; i++)
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
