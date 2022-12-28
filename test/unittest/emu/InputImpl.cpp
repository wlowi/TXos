
#include "InputImpl.h"

#define SLIDER_MIN   300
#define SLIDER_MAX   700
#define SLIDER_INIT  500

InputImpl::InputImpl( channel_t stickCnt, channel_t trimCnt, channel_t auxCnt,
                      uint8_t switches, const switchConf_t *conf)
{
    this->stickCount = stickCnt;
    this->trimCount = trimCnt;
    this->auxCount = auxCnt;

    this->channels = stickCnt + trimCnt + auxCnt;
    this->switches = switches;
    this->switchConf = conf;

    chValues = new channelValue_t[channels];

    for( int i=0; i<channels; i++) {
        chValues[i] = 0;
    }

    swValues = new switchState_t[switches];

    for( int i=0; i<switches; i++) {
        swValues[i] = SW_STATE_0;
    }

    for( int channel=0; channel<channels; channel++) {
        chValues[channel] = SLIDER_INIT;
    }
}

void InputImpl::init() {

    /* This is a no-op as all the initialization
     * has already be done in the constructor.
     */
}

uint8_t InputImpl::GetSwitches() {

    return switches;
}


channelValue_t InputImpl::GetStickValue( int ch) {

    return chValues[ch];
}

channelValue_t InputImpl::GetTrimValue( int ch) {

    return chValues[ch + stickCount];
}

channelValue_t InputImpl::GetAuxValue( int ch) {

    return chValues[ch + stickCount + trimCount];
}

switchState_t InputImpl::GetSwitchValue( int sw) {

    return swValues[sw];
}

switchConf_t InputImpl::GetSwitchConf( int sw) {

    return switchConf[sw];
}

void InputImpl::unittestSetStickValue( channel_t ch, channelValue_t v) {

    chValues[ch] = v;
}

void InputImpl::unittestSetTrimValue( channel_t ch, channelValue_t v) {

    chValues[ch + stickCount] = v;
}

void InputImpl::unittestSetSwitchValues( switchState_t a, switchState_t b, switchState_t c, switchState_t d) {

    swValues[0] = a;
    swValues[1] = a;
    swValues[2] = a;
    swValues[3] = a;
}
