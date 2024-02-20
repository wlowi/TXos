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

#include "ConfigBlock.h"
#include "EEPROM.h"

ConfigBlock::ConfigBlock() {

    storageSize = EEPROM.length();
    modelBlockCount = (storageSize - SYSTEMCONFIG_BLOCK_SIZE) / MODELCONFIG_BLOCK_SIZE;

    LOGV("ConfigBlock::ConfigBlock(): Storage size is %ld. Model configurations: %d\n", storageSize, modelBlockCount);
}

/*
 * Verify config block id and read a block from EEPROM.
 */
configBlock_rc ConfigBlock::readBlock( configBlockID_t id) {

    size_t configStart;

    LOGV("ConfigBlock::readBlock( %d )\n", id);

    if( setBlockID( id)) {

        configStart = getBlockStart();

        LOGV("ConfigBlock::readBlock(): reading %ld bytes from addr %ld\n", configBlockSize, configStart);
        for( uint16_t i=0; i < configBlockSize; i++) {
            block.payload[i] = EEPROM.read( configStart + i);
        }

        if( !isBlockValid()) {
            LOGV("** ConfigBlock::readBlock(): invalid csum for block ID=%d\n", id);
            return CONFIGBLOCK_RC_CSUM;
        }

        if( block.payload[0] == 0xff) {
            LOGV("** ConfigBlock::readBlock(): uninitialized block ID=%d\n", id);
            return CONFIGBLOCK_RC_INVID;
        }

        return CONFIGBLOCK_RC_OK;
    }

    LOGV("** ConfigBlock::readBlock(): invalid block ID=%d\n", id);
    return CONFIGBLOCK_RC_INVID;
}

/* Format member variable "block".
 * Verify config block id and format a block.
 * Formatting a block means filling it with 0xff.
 */
configBlock_rc ConfigBlock::formatBlock( configBlockID_t id) {

    LOGV("ConfigBlock::formatBlock( %d )\n", id);

    if( setBlockID( id)) {

        for( uint16_t i = 0; i < configPayloadSize; i++) {
            block.payload[i] = 0xff;
        }
        
        return CONFIGBLOCK_RC_OK;        
    }

    LOGV("** ConfigBlock::formatBlock(): invalid ID=%d\n", id);
    return CONFIGBLOCK_RC_INVID;  
}

/* Write content of member variable "block" to EEPROM.
 * Verify current block id, update block checksum and write block to EEPROM.
 */
configBlock_rc ConfigBlock::writeBlock() {
    
    size_t configStart;

    LOGV("ConfigBlock::writeBlock() blockID=%d\n", blockID);

    if( setBlockID( blockID)) {

        configStart = getBlockStart();
        *block.checksum = computeChecksum();

        LOGV("ConfigBlock::writeBlock(): writing %ld bytes to addr %ld\n", configBlockSize, configStart);
        for( uint16_t i=0; i < configBlockSize; i++) {
            EEPROM.update( configStart + i, block.payload[i]);
        }

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
    return *block.checksum == computeChecksum();
}

void ConfigBlock::memcpy( uint8_t *dest, const uint8_t *src, size_t sz) const {

    while( sz--) {
        (*dest) = (*src);
        dest++;
        src++;
    }
}

/* private */

bool ConfigBlock::setBlockID( configBlockID_t blkID) {

    if( blkID > CONFIG_BLOCKID_INVALID && blkID <= modelBlockCount) {

        blockID = blkID;

        if( blockID == SYSTEMCONFIG_BLOCKID) {
            configBlockSize = SYSTEMCONFIG_BLOCK_SIZE;
        } else {
            configBlockSize = MODELCONFIG_BLOCK_SIZE;
        }

        configPayloadSize = configBlockSize - sizeof(checksum_t);

        block.checksum = (checksum_t*)(&(block.payload[0]) + configPayloadSize);

        return true;
    }

    return false;
}

size_t ConfigBlock::getBlockStart() const {

    return (blockID == SYSTEMCONFIG_BLOCKID) 
            ? 0 
            : SYSTEMCONFIG_BLOCK_SIZE + (blockID-1) * configBlockSize;
}

/*
 * Recompute and return block checksum.
 * NOTE: The blocks checksum is NOT updated.
 */
checksum_t ConfigBlock::computeChecksum()
{
    checksum_t checksum = 0;
    const uint8_t *p = (uint8_t*)&block;

    for( uint16_t i = 0; i < configPayloadSize; i++)
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
