
#include "ServoReverse.h"

ServoReverse::ServoReverse() : Module() {

    revBits = 0;
}

void ServoReverse::run( channelSet_t &channels) {

    for( int ch = 0; ch < CHANNELS; ch++) {
        if( IS_BIT_SET( revBits, ch)) {
            channels.analogChannel[ch] = -channels.analogChannel[ch];
        }
    }
}