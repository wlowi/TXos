
#include "Controls.h"
#include "InputImpl.h"

extern InputImpl *inputImpl;

Controls::Controls() {

}

void Controls::init() {

}

void Controls::GetControlValues( channelSet_t &channels) {

    for( int ch=0; ch < CHANNELS; ch++) {
        channels.analogChannel[ch] = CHANNELVALUE_MID;
    }

    for( int ch=0; ch < inputImpl->GetChannels(); ch++) {
        channels.analogChannel[ch] = inputImpl->GetChannelValue( ch);
    }

    channels.switches = 0;
    for( int sw=0; sw < inputImpl->GetSwitches(); sw++) {
        channels.switches |= (inputImpl->GetSwitchValue( sw) << (sw<<1));
    }
}