
#include "ServoSubtrim.h"

ServoSubtrim::ServoSubtrim() : Module( MODULE_SERVO_SUBTRIM_TYPE) {

    setDefaults();
}

void ServoSubtrim::run( channelSet_t &channels) {

    for( channel_t ch = 0; ch < CHANNELS; ch++) {
        
        channels.analogChannel[ch] += PCT_TO_CHANNEL( cfg.trim_pct[ch]);

        if( channels.analogChannel[ch] > CHANNELVALUE_MAX) {
            channels.analogChannel[ch] = CHANNELVALUE_MAX;
        }

        if( channels.analogChannel[ch] < CHANNELVALUE_MIN) {
            channels.analogChannel[ch] = CHANNELVALUE_MIN;
        }
    }
}

void ServoSubtrim::setDefaults() {

    for( channel_t ch=0; ch < CHANNELS; ch++) {
        cfg.trim_pct[ch] = 0;
    }
}

moduleSize_t ServoSubtrim::getConfigSize() {

    return (moduleSize_t)sizeof( cfg);
}

uint8_t *ServoSubtrim::getConfig() {

    return (uint8_t*)&cfg;
}

void ServoSubtrim::setConfig( uint8_t *config, moduleSize_t size) {


}
