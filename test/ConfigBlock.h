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

/*
    Read, write and format configuration blocks.

    Configutation blocks are stored in EEPROM.
    The EEPROM space is divided into multiple blocks.
    Each block holds configuration data.

    Each block is protected by a checksum which is in the last 
    bytes of a block.

    The first configuration block is reserved for the global
    system wide configuration. Further blocks hold
    model specific configuration data.

 */

#ifndef _ConfigBlock_h_
#define _ConfigBlock_h_

#include "TXos.h"
#include "EEPROM.h"

typedef uint16_t checksum_t;
typedef int8_t configBlockID_t;

#define SYSTEMCONFIG_BLOCK_SIZE ((size_t)128)
#define MODELCONFIG_BLOCK_SIZE  ((size_t)264)

/* The maximum of
 * SYSTEMCONFIG_BLOCK_SIZE and MODELCONFIG_BLOCK_SIZE.

 * This is used to allocate a config block in memory.
 */
#define MEM_BLOCK_SIZE          ((size_t)264)

#define CONFIG_BLOCKID_INVALID  ((configBlockID_t)-1)
#define SYSTEMCONFIG_BLOCKID    ((configBlockID_t)0)

/* Return codes */
typedef uint8_t configBlock_rc;

#define CONFIGBLOCK_RC_OK       ((configBlock_rc)0)
#define CONFIGBLOCK_RC_INVID    ((configBlock_rc)1)
#define CONFIGBLOCK_RC_CSUM     ((configBlock_rc)2)


typedef struct configBlock_t {

    uint8_t     payload[MEM_BLOCK_SIZE];
    /* A pointer to the checksum in payload. */
    checksum_t  *checksum;

} configBlock_t;

class ConfigBlock {

    private:
        size_t storageSize;
        configBlockID_t modelBlockCount;

        configBlockID_t blockID = CONFIG_BLOCKID_INVALID;
        size_t configBlockSize = 0;   // Total block size
        size_t configPayloadSize = 0; // Block size without checksum

        configBlock_t block;

        /* Store block ID and compute block and payload size. */
        bool setBlockID( configBlockID_t blkID);
        size_t getBlockStart() const;
        checksum_t computeChecksum();
        checksum_t rotate( checksum_t v) const;

    public:
        ConfigBlock();

        size_t getStorageSize() const { return storageSize; }
        configBlockID_t getModelBlockCount() const { return modelBlockCount; }

        configBlock_rc readBlock( configBlockID_t id);
        configBlock_rc formatBlock( configBlockID_t id);
        configBlock_rc writeBlock();

        uint8_t *getPayload();
        size_t getPayloadSize() const { return configPayloadSize; }

        bool isBlockValid();
        void memcpy( uint8_t *dest, const uint8_t *src, size_t sz) const;
};

#endif
