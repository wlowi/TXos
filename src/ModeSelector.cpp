/*
 * ModeSelector.cpp
 */
 
#include "ModeSelector.h"
#include "Configurable.h"

ModeSelector::ModeSelector() : ProcessModule( CONFIG_MODULE_MODESELECT) {

    init();
}

void ModeSelector::init() {

    mode = 1;
}

void ModeSelector::setConfiguration( uint8_t type, uint8_t length, uint8_t *mem) {
    
    if( GET_MODULE_SUBTYPE(type) == 0 && length == sizeof(mode)) {
        memcpy( (void*)&mode, (const void*)mem, (size_t)length);
    }
}

uint8_t ModeSelector::getConfiguration( uint8_t *mem, uint8_t ptr) {
    
    uint8_t bytesWritten = 0;
    uint8_t type = MAKE_MODULE_CONFIG_ID( getModuleType(), CONFIG_MODULE_SUBTYPE_0);
    uint8_t length = (uint8_t)sizeof(mode);

    mem[ptr++] = type;
    bytesWritten++;
    
    mem[ptr++] = length;
    bytesWritten++;
    
    memcpy( (void*)&mem[ptr], (const void*)&mode, (size_t)length);
    // ptr += length;
    bytesWritten += length;
    
    return bytesWritten;
}

void ModeSelector::process( channelSet_t *channels) {
    
    value_t tmp;
    
    switch( mode) {

    case 1:
        break;

    case 2: // Switch channel 1 and 3 (GAS <> ELEVATOR)
        tmp = channels->analogChannel[CHANNEL_GAS];
        channels->analogChannel[CHANNEL_GAS] = channels->analogChannel[CHANNEL_ELEVATOR];
        channels->analogChannel[CHANNEL_ELEVATOR] = tmp;
        break;

    case 3: // Switch channel 2 and 4 (RUDDER <> AILERON)
        tmp = channels->analogChannel[CHANNEL_RUDDER];
        channels->analogChannel[CHANNEL_RUDDER] = channels->analogChannel[CHANNEL_AILERON];
        channels->analogChannel[CHANNEL_AILERON] = tmp;
        break;
        
    case 4: // Switch channel 1 and 3 (GAS <> ELEVATOR)
            // and channel 2 and 4 (RUDDEL <> AILERON)
        tmp = channels->analogChannel[CHANNEL_GAS];
        channels->analogChannel[CHANNEL_GAS] = channels->analogChannel[CHANNEL_ELEVATOR];
        channels->analogChannel[CHANNEL_ELEVATOR] = tmp;

        tmp = channels->analogChannel[CHANNEL_RUDDER];
        channels->analogChannel[CHANNEL_RUDDER] = channels->analogChannel[CHANNEL_AILERON];
        channels->analogChannel[CHANNEL_AILERON] = tmp;
        break;
    }
}
