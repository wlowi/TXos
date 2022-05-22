
#ifndef _ConfigBlock_h_
#define _ConfigBlock_h_

#include "TXos.h"
#include "EEPROM.h"

typedef uint16_t checksum_t;

#define CONFIG_EEPROM_SIZE    ((size_t)4096)
#define CONFIG_BLOCK_SIZE     ((size_t)128)
#define CONFIG_PAYLOAD_SIZE   (CONFIG_BLOCK_SIZE - sizeof(checksum_t))
#define CONFIG_BLOCKS         (CONFIG_EEPROM_SIZE/CONFIG_BLOCK_SIZE)

#define CONFIG_MODELS         (CONFIG_BLOCKS -1)

typedef int8_t configBlockID_t;

#define CONFIG_BLOCKID_INVALID ((configBlockID_t)-1)

/* Return codes */
#define CONFIGBLOCK_RC_OK     ((uint8_t)0)
#define CONFIGBLOCK_RC_INVID  ((uint8_t)1)
#define CONFIGBLOCK_RC_CSUM   ((uint8_t)2)

typedef struct configBlock_t {

    uint8_t     payload[CONFIG_PAYLOAD_SIZE];
    checksum_t  checksum;
} configBlock_t;

class ConfigBlock {

    private:
        configBlockID_t blockID;
        configBlock_t block;

        checksum_t computeChecksum();
        checksum_t rotate( checksum_t v);

    public:
        ConfigBlock();

        uint8_t readBlock( configBlockID_t id);
        uint8_t formatBlock( configBlockID_t id);
        uint8_t writeBlock();

        uint8_t *getPayload();

        bool isBlockValid();
        void memcpy( uint8_t *dest, uint8_t *src, size_t sz);
};

#endif
