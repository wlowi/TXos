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
