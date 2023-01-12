/*
    TXos. A remote control transmitter OS.

    Copyright (C) 2022 Wolfgang Lohwasser

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

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
    swValues[1] = b;
    swValues[2] = c;
    swValues[3] = d;
}
