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

#include "ConfigBlock.h"

ConfigBlock::ConfigBlock() = default;

/*
 * Verify config block id and read a block from EEPROM.
 */
configBlock_rc ConfigBlock::readBlock( configBlockID_t id) {

    int configStart;

    LOGV("ConfigBlock::readBlock( %d )\n", id);
    if( id > CONFIG_BLOCKID_INVALID && id < CONFIG_BLOCKS) {
        
        blockID = id;
        configStart = (blockID * CONFIG_BLOCK_SIZE);
        EEPROM.get( configStart, block);

        if( !isBlockValid()) {
            LOGV("** ConfigBlock::readBlock(): invalid csum for ID=%d\n", blockID);
            return CONFIGBLOCK_RC_CSUM;
        }

        return CONFIGBLOCK_RC_OK;
    }

    LOGV("** ConfigBlock::readBlock(): invalid ID=%d\n", id);
    return CONFIGBLOCK_RC_INVID;
}

/*
 * Verify config block id and format a block.
 * Formatting a block means filling it with 0xff.
 */
configBlock_rc ConfigBlock::formatBlock( configBlockID_t id) {

    LOGV("ConfigBlock::formatBlock( %d )\n", id);
    if( id > CONFIG_BLOCKID_INVALID && id < CONFIG_BLOCKS) {

        blockID = id;
        for( uint8_t i = 0; i < CONFIG_PAYLOAD_SIZE; i++) {
            block.payload[i] = 0xff;
        }
        
        return CONFIGBLOCK_RC_OK;        
    }

    LOGV("** ConfigBlock::formatBlock(): invalid ID=%d\n", id);
    return CONFIGBLOCK_RC_INVID;  
}

/*
 * Verify current block id, update block checksum and write block to EEPROM.
 */
configBlock_rc ConfigBlock::writeBlock() {
    
    int configStart;

    LOGV("ConfigBlock::writeBlock() blockID=%d\n", blockID);
    if( blockID > CONFIG_BLOCKID_INVALID && blockID < CONFIG_BLOCKS) {

        configStart = (blockID * CONFIG_BLOCK_SIZE);
        block.checksum = computeChecksum();
        EEPROM.put( configStart, block);
        
        return CONFIGBLOCK_RC_OK;
    }

    LOGV("** ConfigBlock::writeBlock(): invalid ID=%d\n", blockID);
    return CONFIGBLOCK_RC_INVID;
}

/*
 * Get a pointer to the block payload.
 */
uint8_t *ConfigBlock::getPayload() {

    return block.payload;
}

/*
 * Recompute block checksum and compare against stored block checksum.
 * Returns false if the computed checksum differs.
 */
bool ConfigBlock::isBlockValid()
{
    return block.checksum == computeChecksum();
}

void ConfigBlock::memcpy( uint8_t *dest, const uint8_t *src, size_t sz) const {

    while( sz--) {
        (*dest) = (*src);
        dest++;
        src++;
    }
}

/* private */

/*
 * Recompute and return block checksum.
 * NOTE: The blocks checksum is NOT updated.
 */
checksum_t ConfigBlock::computeChecksum()
{
    checksum_t checksum = 0;
    const uint8_t *p = (uint8_t*)&block;

    for( uint16_t i = 0; i < CONFIG_PAYLOAD_SIZE; i++)
    {
        checksum = rotate(checksum);
        checksum ^= (i ^ *(p+i));
    }

    return checksum;
}

/*
 * Left rotate unsigned int value.
 */
checksum_t ConfigBlock::rotate( checksum_t v) const
{
    return ((v >> 15) & 1) | (v << 1);
}
