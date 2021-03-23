/*
 * ConfigBlock.h
 */

#ifndef _ConfigBlock_h
#define _ConfigBlock_h

#include "Arduino.h"


/* Function return codes */
#define CONFIGBLOCK_RC_OK             ((uint8_t)0)
#define CONFIGBLOCK_RC_INVID         ((uint8_t)1)
#define CONFIGBLOCK_RC_CSUM           ((uint8_t)2)


typedef uint16_t checksum_t;

#define EEPROM_START_LOC           ((uint8_t*)0)

#define CONFIG_EEPROM_SIZE         ((size_t)1024)
#define CONFIG_BLOCK_PAYLOAD_SIZE  ((size_t)128 - sizeof(checksum_t)) 

/* Number of supported config blocks */
#define CONFIG_BLOCKS  (CONFIG_EEPROM_SIZE / CONFIG_BLOCK_PAYLOAD_SIZE)

/* Well known block IDs */
#define CONFIG_SYSTEM_BLOCK    0
#define CONFIG_INVALID_BLOCK   CONFIG_BLOCKS


typedef struct configBlock_t {
    
    uint8_t mem[CONFIG_BLOCK_PAYLOAD_SIZE];
    checksum_t checksum;

} configBlock_t;


class ConfigBlock {
private:
    uint8_t blockId;
    configBlock_t block;

public:
    ConfigBlock();
    
    /* Read a block from EEPROM.
     * Returns
     *   CONFIGBLOCK_OK        on success
     *   CONFIGBLOCK_CSUM      if the blocks checksum is invalid
     *   CONFIGBLOCK_INV_ID    if the passed id is invalid
     */
    uint8_t readBlock( uint8_t id);
    uint8_t formatBlock( uint8_t id);
    uint8_t writeBlock();

    uint8_t *getMemoryBlock();

    checksum_t computeChecksum();
    checksum_t rotate( checksum_t v);
    boolean isBlockValid();
};

#endif
