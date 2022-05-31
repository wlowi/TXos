
#include "InputImpl.h"

InputImpl::InputImpl( int channels, int switches)
{
    this->channels = channels;
    this->switches = switches;

    chValues = new int[channels];

    for( int i=0; i<channels; i++) {
        chValues[i] = 0;
    }

    swValues = new int[switches];

    for( int i=0; i<switches; i++) {
        swValues[i] = 0;
    }
}

int InputImpl::GetChannels() {

    return channels;
}

int InputImpl::GetSwitches() {

    return switches;
}

int InputImpl::GetChannelValue( int ch) {

    return chValues[ch];
}

int InputImpl::GetSwitchValue( int sw) {

    return swValues[sw];
}
