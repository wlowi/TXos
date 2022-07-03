
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

void InputImpl::init( switchSetConf_t conf) {
  
}

channel_t InputImpl::GetChannels() {

    return channels;
}

switch_t InputImpl::GetSwitches() {

    return switches;
}

channelValue_t InputImpl::GetChannelValue( channel_t ch) {

    if( ch < channels) {
      return analogRead( analogPins[ch]);
    }

    LOGV("InputImpl::GetChannelValue: Illegal channel no. %s", ch);
    
    return 0;
}

switchState_t InputImpl::GetSwitchValue( switch_t sw) {

    if( sw < switches) {
      return SW_STATE_0;
    }

    LOGV("InputImpl::GetSwitchValue: Illegal switch no. %s", sw);

    return SW_STATE_DONTCARE;
}
