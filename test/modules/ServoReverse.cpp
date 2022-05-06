
#include "ServoReverse.h"

ServoReverse::ServoReverse() : Module( MODULE_SERVO_REVERSE_TYPE) {

    setDefaults();
}

void ServoReverse::run( channelSet_t &channels) {

    for( channel_t ch = 0; ch < CHANNELS; ch++) {
        if( IS_BIT_SET( cfg.revBits, ch)) {
            channels.analogChannel[ch] = -channels.analogChannel[ch];
        }
    }
}

void ServoReverse::setDefaults() {

    cfg.revBits = 0;
}

moduleSize_t ServoReverse::getConfigSize() {

    return (moduleSize_t)sizeof( cfg);
}

uint8_t *ServoReverse::getConfig() {

    return (uint8_t*)&cfg;
}

void ServoReverse::setConfig( uint8_t *config, moduleSize_t size) {


}
