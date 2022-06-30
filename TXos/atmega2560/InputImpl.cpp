
#include "InputImpl.h"

InputImpl::InputImpl( channel_t channels, const uint8_t analogPins[],
                      switch_t switches, const uint8_t switchPins[])
{
    this->channels = channels;
    this->switches = switches;

    this->analogPins = analogPins;
    this->switchPins = switchPins;

    swValues = new int[switches];

    for( int i=0; i<switches; i++) {
        swValues[i] = 0;
    }
}

channel_t InputImpl::GetChannels() {

    return channels;
}

switch_t InputImpl::GetSwitches() {

    return switches;
}

int InputImpl::GetChannelValue( channel_t ch) {

    if( ch < channels) {
      return analogRead( analogPins[ch]);
    }

    LOG("InputImpl::GetChannelValue: Illegal channel no. %s", ch);
    
    return 0;
}

int InputImpl::GetSwitchValue( switch_t sw) {

    if( sw < switches) {
      return 0;
    }

    LOG("InputImpl::GetSwitchValue: Illegal switch no. %s", sw);

    return 0;
}
