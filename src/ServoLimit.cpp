/*
 * ServoLimit.cpp
 */

#include "ServoLimit.h"
#include "Configurable.h"
ServoLimit::ServoLimit() : ProcessModule( CONFIG_MODULE_SERVOLIMIT) {

    init();
}

void ServoLimit::init() {

    for( channel_t ch = 0; ch < CHANNELS; ch++) {
        minLimitPct[ch] = -100;
        maxLimitPct[ch] = 100;
    }
}

void ServoLimit::setConfiguration( uint8_t type, uint8_t length, uint8_t *mem) {
    
    if( GET_MODULE_SUBTYPE(type) == 0 && length == sizeof(minLimitPct)) {
        memcpy( (void*)minLimitPct, (const void*)mem, (size_t)length);
    }

    if( GET_MODULE_SUBTYPE(type) == 1 && length == sizeof(maxLimitPct)) {
        memcpy( (void*)maxLimitPct, (const void*)mem, (size_t)length);
    }
}

uint8_t ServoLimit::getConfiguration( uint8_t *mem, uint8_t ptr) {
    
    uint8_t bytesWritten = 0;
    uint8_t type = MAKE_MODULE_CONFIG_ID( getModuleType(), CONFIG_MODULE_SUBTYPE_0);
    uint8_t length = (uint8_t)sizeof(minLimitPct);

    mem[ptr++] = type;
    bytesWritten++;
    
    mem[ptr++] = length;
    bytesWritten++;
    
    memcpy( (void*)&mem[ptr], (const void*)minLimitPct, (size_t)length);
    ptr += length;
    bytesWritten += length;


    type = MAKE_MODULE_CONFIG_ID( getModuleType(), CONFIG_MODULE_SUBTYPE_1);
    length = (uint8_t)sizeof(maxLimitPct);

    mem[ptr++] = type;
    bytesWritten++;
    
    mem[ptr++] = length;
    bytesWritten++;
    
    memcpy( (void*)&mem[ptr], (const void*)maxLimitPct, (size_t)length);
    // ptr += length;
    bytesWritten += length;
    
    return bytesWritten;
}

void ServoLimit::process( channelSet_t *channels) {
    
    value_t minT;
    value_t maxT;
    
    for( channel_t ch = 0; ch < CHANNELS; ch++) {

        minT = PCT_TO_VALUE( minLimitPct[ch]);
        maxT = PCT_TO_VALUE( maxLimitPct[ch]);

        if( channels->analogChannel[ch] > maxT) {
            channels->analogChannel[ch] = maxT;
            
        } else if( channels->analogChannel[ch] < minT) {
            channels->analogChannel[ch] = minT;
        }
    }
}
