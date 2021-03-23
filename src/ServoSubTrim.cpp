/*
 * ServoSubTrim.cpp
 */

#include "ServoSubTrim.h"
#include "Configurable.h"

ServoSubTrim::ServoSubTrim() : ProcessModule( CONFIG_MODULE_SERVOSUBTRIM) {

    init();
}

void ServoSubTrim::init() {

    for( channel_t ch = 0; ch < CHANNELS; ch++) {
        subTrimPct[ch] = 0;
    }
}

void ServoSubTrim::setConfiguration( uint8_t type, uint8_t length, uint8_t *mem) {
    
    if( GET_MODULE_SUBTYPE(type) == 0 && length == sizeof(subTrimPct)) {
        memcpy( (void*)subTrimPct, (const void*)mem, (size_t)length);
    }
}

uint8_t ServoSubTrim::getConfiguration( uint8_t *mem, uint8_t ptr) {
    
    uint8_t bytesWritten = 0;
    uint8_t type = MAKE_MODULE_CONFIG_ID( getModuleType(), CONFIG_MODULE_SUBTYPE_0);
    uint8_t length = (uint8_t)sizeof(subTrimPct);

    mem[ptr++] = type;
    bytesWritten++;
    
    mem[ptr++] = length;
    bytesWritten++;
    
    memcpy( (void*)&mem[ptr], (const void*)subTrimPct, (size_t)length);
    // ptr += length;
    bytesWritten += length;
    
    return bytesWritten;
}

void ServoSubTrim::process( channelSet_t *channels) {
    
    for( channel_t ch = 0; ch < CHANNELS; ch++) {
        if( subTrimPct[ch] != 0) {
            channels->analogChannel[ch] += PCT_TO_VALUE( subTrimPct[ch]);
        }
    }
}
