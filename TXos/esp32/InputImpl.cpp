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
#include "PortsImpl.h"

extern InputImpl *inputImpl;
extern PortsImpl *portsImpl;

InputImpl::InputImpl( channel_t stickCnt, channel_t trimCnt, channel_t auxCnt,
                      const uint8_t analogPins[],
                      switch_t switches, const switchConf_t *conf,
                      const uint8_t switchPins[])
{
    this->stickCount = stickCnt;
    this->trimCount = trimCnt;
    this->auxCount = auxCnt;
    this->adcInputs = stickCnt + trimCnt + auxCnt;
    this->switches = switches;
    this->switchConf = conf;
    this->analogPins = analogPins;
    this->switchPins = switchPins;

    adcValues = new channelValue_t[adcInputs];

    for( int i=0; i<adcInputs; i++) {
        adcValues[i] = 0;
    }

    init();
}

void InputImpl::init() {

    analogReadResolution( 10);
    analogSetAttenuation( ADC_11db );

    for( uint8_t i=0; i<switches; i++) {
        portsImpl->portInit( switchPins[i], INPUT);
    }
}

void InputImpl::start() {

}

switch_t InputImpl::GetSwitches() {

    return switches;
}
        
channelValue_t InputImpl::GetStickValue( channel_t ch) {

    if( ch < stickCount) {
      return GetAnalogValue( ch);
    }

    LOGV("InputImpl::GetStickValue: Illegal channel no. %d", ch);
    
    return 0;
}

channelValue_t InputImpl::GetTrimValue( channel_t ch) {

  if( ch < trimCount) {
      return GetAnalogValue( ch + stickCount);
  }

  LOGV("InputImpl::GetTrimValue: Illegal channel no. %d", ch);

  return 0;
}

channelValue_t InputImpl::GetAuxValue( channel_t ch) {

  if( ch < auxCount) {
      return GetAnalogValue( ch + stickCount + trimCount);
  }

  LOGV("InputImpl::GetAuxValue: Illegal channel no. %d", ch);

  return 0;
}

channelValue_t InputImpl::GetAnalogValue( channel_t ch) {

    channelValue_t v;

    if( ch < adcInputs) {

        v = analogRead( analogPins[ch]);
        return v;
    }

    LOGV("InputImpl::GetStickValue: Illegal channel no. %d", ch);

    return 0;
}

switchState_t InputImpl::GetSwitchValue( switch_t sw) {

    bool s1, s2;
    switchConf_t swConf;
    switchState_t state = SW_STATE_DONTCARE;

    if( sw >= switches) {
      LOGV("InputImpl::GetSwitchValue: Illegal switch no. %d", sw);
      return SW_STATE_DONTCARE;
    }
      
    swConf = switchConf[sw];
    
    switch( swConf) {

      case SW_CONF_2STATE:
        state = portsImpl->portGet(switchPins[sw]) ? SW_STATE_0 : SW_STATE_1;
        break;
        
      case SW_CONF_3STATE:
        portsImpl->portInit( switchPins[sw], INPUT);
        s1 = portsImpl->portGet(switchPins[sw]);
        portsImpl->portInit( switchPins[sw], INPUT_PULLUP);
        s2 = portsImpl->portGet(switchPins[sw]);

        if( !(s1 || s2)) state = SW_STATE_2;
        else if( s1 && s2) state = SW_STATE_0;
        else state = SW_STATE_1;
        break;
        
      case SW_CONF_UNUSED:
      case SW_CONF_CHANNEL:
      default:
        state = SW_STATE_DONTCARE;
    }

    return state;
}

switchConf_t InputImpl::GetSwitchConf( switch_t sw) {
  
  return switchConf[sw];
}
