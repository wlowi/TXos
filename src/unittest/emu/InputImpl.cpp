/*
  TXos. A remote control transmitter OS.

  MIT License

  Copyright (c) 2023 wlowi

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
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
