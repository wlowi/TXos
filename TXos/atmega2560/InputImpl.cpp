
#include <util/atomic.h>

#include "InputImpl.h"

extern InputImpl *inputImpl;

/* 
 * ADC conversion complete interrupt. 
 */
ISR(ADC_vect) {

    /* MUST read ADCL first */
    channelValue_t v = ADCL;
    v |= (ADCH << 8);

    /* Disable ADC */
    ADCSRA &= ~_BV(ADEN);
    
    if( inputImpl->mux < inputImpl->adcInputs -1) {

      inputImpl->adcValues[inputImpl->mux] = v;
      
      inputImpl->mux++;
      inputImpl->setMux();
      
      ADCSRA |= _BV(ADEN) | _BV(ADSC) | _BV(ADIE);
      
    } else {
      /* done */
    }
}

InputImpl::InputImpl( channel_t adcInputs, const uint8_t analogPins[],
                      switch_t switches, const uint8_t switchPins[])
{
    this->adcInputs = adcInputs;
    this->switches = switches;

    this->analogPins = analogPins;
    this->switchPins = switchPins;

    swValues = new int[switches];
    adcValues = new channelValue_t[adcInputs];

    for( int i=0; i<switches; i++) {
        swValues[i] = 0;
    }

    for( int i=0; i<adcInputs; i++) {
        adcValues[i] = 0;
    }
}

void InputImpl::init( switchSetConf_t conf) {

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
    }
}

void InputImpl::start() {

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {
      
       mux = 0;

       setMux();

       ADCSRA |= _BV(ADEN) | _BV(ADSC) | _BV(ADIE);
    }
}

void InputImpl::setMux() {

    uint8_t adc = analogPins[mux];

    ADMUX &= ~(_BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0));
        
    if( adc < A8) {
        ADMUX |= (adc - A0);
        ADCSRB &= ~_BV(MUX5);
    } else {
        ADMUX |= (adc - A8);
        ADCSRB |= _BV(MUX5);
    }
}

channel_t InputImpl::GetChannels() {

    return adcInputs;
}

switch_t InputImpl::GetSwitches() {

    return switches;
}

channelValue_t InputImpl::GetChannelValue( channel_t ch) {

    channelValue_t v;
    
    if( ch < adcInputs) {
      ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {
          v = adcValues[ch];
      }
      return v;
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
