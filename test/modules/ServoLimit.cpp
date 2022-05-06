
#include "ServoLimit.h"

ServoLimit::ServoLimit() : Module( MODULE_SERVO_LIMIT_TYPE) {

    setDefaults();
}

void ServoLimit::run( channelSet_t &channels) {

    channelValue_t v;

    for( channel_t ch = 0; ch < CHANNELS; ch++) {

        v = PCT_TO_CHANNEL( cfg.posLimit_pct[ch]);
        if( channels.analogChannel[ch] > v) {
            channels.analogChannel[ch] = v;
        }

        v = PCT_TO_CHANNEL( cfg.negLimit_pct[ch]);
        if( channels.analogChannel[ch] < v) {
            channels.analogChannel[ch] = v;
        }
    }
}

void ServoLimit::setDefaults() {

    for( channel_t ch=0; ch < CHANNELS; ch++) {
        cfg.posLimit_pct[ch] = PERCENT_MAX;
        cfg.negLimit_pct[ch] = PERCENT_MIN;
    }
}

moduleSize_t ServoLimit::getConfigSize() {

    return (moduleSize_t)sizeof( cfg);
}

uint8_t *ServoLimit::getConfig() {

    return (uint8_t*)&cfg;
}

void ServoLimit::setConfig( uint8_t *config, moduleSize_t size) {

}
