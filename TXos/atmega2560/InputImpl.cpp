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

#include <util/atomic.h>

#include "InputImpl.h"
#include "PortsImpl.h"

extern InputImpl *inputImpl;
extern PortsImpl *portsImpl;

/* 
 * ADC conversion complete interrupt. 
 */
ISR(ADC_vect) {

    /* MUST read ADCL first */
    channelValue_t v = ADCL;
    v |= (ADCH << 8);

    /* Disable ADC */
    ADCSRA &= ~_BV(ADEN);
    
    if( inputImpl->mux < inputImpl->adcInputs) {

#ifdef INVERT_CH1
      if( inputImpl->mux == 0) v = ADC_RESOLUTION -v;
#endif
#ifdef INVERT_CH2
      if( inputImpl->mux == 1) v = ADC_RESOLUTION -v;
#endif
#ifdef INVERT_CH3
      if( inputImpl->mux == 2) v = ADC_RESOLUTION -v;
#endif
#ifdef INVERT_CH4
      if( inputImpl->mux == 3) v = ADC_RESOLUTION -v;
#endif

      inputImpl->adcValues[inputImpl->mux] = v;
      
      inputImpl->mux++;
      inputImpl->setMux();      
    } else {
      /* done */
    }
}

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
}

void InputImpl::init() {

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {
    
        /* Disable power reduction for ADC */
        PRR0 &= ~_BV(PRADC);
         
        /* REFS1 = 0, REFS0 = 1   ==>   VCC with ext. cap. on AREF */
        ADMUX = _BV(REFS0);
        
        /* Prescaler /128   ==>   16MHz / 128 = 125KHz */
        ADCSRA = _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
        ADCSRB = 0;

        for( uint8_t i=0; i<adcInputs; i++) {
          
            uint8_t adc = analogPins[i];
             
            /* Disable Digital input on ADC pins */
            if( adc < A8) {
                DIDR0 |= (1 << (adc - A0));
            } else {
                DIDR2 |= (1 << (adc - A8));
            }
        }

        for( uint8_t i=0; i<switches; i++) {
          portsImpl->portInit( switchPins[i], INPUT);
        }
    }
}

void InputImpl::start() {

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {
      
       mux = 0;
       setMux();

    }
}

void InputImpl::setMux() {

    uint8_t adc;

    if( mux < adcInputs) {

        adc = analogPins[mux];

        ADMUX &= ~(_BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0));
        
        if( adc < A8) {
          ADMUX |= (adc - A0);
          ADCSRB &= ~_BV(MUX5);
        } else {
          ADMUX |= (adc - A8);
          ADCSRB |= _BV(MUX5);
        }
          
        ADCSRA |= _BV(ADEN) | _BV(ADSC) | _BV(ADIE);
    }
}

switch_t InputImpl::GetSwitches() {

    return switches;
}
        
channelValue_t InputImpl::GetStickValue( channel_t ch) {

    if( ch < stickCount) {
      return GetAnalogValue( ch);
    }

    LOGV("InputImpl::GetStickValue: Illegal channel no. %s", ch);
    
    return 0;
}

channelValue_t InputImpl::GetTrimValue( channel_t ch) {

  if( ch < trimCount) {
      return GetAnalogValue( ch + stickCount);
  }

  LOGV("InputImpl::GetTrimValue: Illegal channel no. %s", ch);

  return 0;
}

channelValue_t InputImpl::GetAuxValue( channel_t ch) {

  if( ch < auxCount) {
      return GetAnalogValue( ch + stickCount + trimCount);
  }

  LOGV("InputImpl::GetAuxValue: Illegal channel no. %s", ch);

  return 0;
}

channelValue_t InputImpl::GetAnalogValue( channel_t ch) {

    channelValue_t v;

    if( ch < adcInputs) {
      ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {
          v = adcValues[ch];
      }
      return v;
    }

    LOGV("InputImpl::GetStickValue: Illegal channel no. %s", ch);

    return 0;
}

switchState_t InputImpl::GetSwitchValue( switch_t sw) {

    bool s1, s2;
    switchConf_t swConf;
    switchState_t state = SW_STATE_DONTCARE;

    if( sw >= switches) {
      LOGV("InputImpl::GetSwitchValue: Illegal switch no. %s", sw);
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
