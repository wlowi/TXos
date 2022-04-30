
#include "Controls.h"
#include "InputBox.h"

extern InputBox *inputBox;

Controls::Controls() {

}

void Controls::GetControlValues( channelSet_t &channels) {

    for( int ch=0; ch < inputBox->GetChannels(); ch++) {
        channels.analogChannel[ch] = inputBox->GetChannelValue( ch);
    }

    channels.switches = 0;
    for( int sw=0; sw < inputBox->GetSwitches(); sw++) {
        channels.switches |= (inputBox->GetSwitchValue( sw) << (sw<<1));
    }
}