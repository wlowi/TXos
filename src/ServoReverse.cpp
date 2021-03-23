/*
 * ServoReverse.cpp
 */

#include "ServoReverse.h"
#include "Configurable.h"
ServoReverse::ServoReverse() : ProcessModule( CONFIG_MODULE_SERVOREVERSE) {

    init();
}

void ServoReverse::init() {

    reverseBits = 0;
}

void ServoReverse::setConfiguration( uint8_t type, uint8_t length, uint8_t *mem) {
    
    if( GET_MODULE_SUBTYPE(type) == 0 && length == sizeof(reverseBits)) {
        memcpy( (void*)&reverseBits, (const void*)mem, (size_t)length);
    }
}

uint8_t ServoReverse::getConfiguration( uint8_t *mem, uint8_t ptr) {
    
    uint8_t bytesWritten = 0;
    uint8_t type = MAKE_MODULE_CONFIG_ID( getModuleType(), CONFIG_MODULE_SUBTYPE_0);
    uint8_t length = (uint8_t)sizeof(reverseBits);

    mem[ptr++] = type;
    bytesWritten++;
    
    mem[ptr++] = length;
    bytesWritten++;
    
    memcpy( (void*)&mem[ptr], (const void*)&reverseBits, (size_t)length);
    // ptr += length;
    bytesWritten += length;
    
    return bytesWritten;
}

void ServoReverse::process( channelSet_t *channels) {
    
    for( channel_t ch = 0; ch < CHANNELS; ch++) {
        if( reverseBits & _BV(ch)) {
            channels->analogChannel[ch] = -channels->analogChannel[ch];
        }
    }
}
