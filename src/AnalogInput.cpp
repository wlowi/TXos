/*
 * AnalogInput.cpp
 * 
 * History:
 * 
 *   wolfix   31.Dec.2019  Creation
 */

#include <util/atomic.h>

#include "AnalogInput.h"

AnalogInput analogInput;

/* ADC Conversion complete interrupt.
 */
ISR(ADC_vect)
{
    uint8_t mux = analogInput.channelMux;
    
    /* MUST!!! read ADCL first, then ADCH */
    analog_t v = ADCL;
    v |= (ADCH << 8);

    /* Disable ADC */
    ADCSRA &= ~_BV(ADEN);

    if( mux < ANALOG_CHANNELS) {
        analogInput.analogChannels.channel[ mux++] = v;
    }

    if( mux < ANALOG_CHANNELS) {
        analogInput.startADCconversion( mux);
    } else {
        analogInput.adcState = ANALOG_DONE;
        Dispatcher::adcComplete();
    }
}

AnalogInput::AnalogInput() {

    adcState = ANALOG_IDLE;
    channelMux = 0;
    
    for( channel_t ch = 0; ch < ANALOG_CHANNELS; ch++) {
        analogChannels.calibrateMid[ch] = 511;
        analogChannels.calibrateMin[ch] = 0;
        analogChannels.calibrateMax[ch] = 1023;
    }

    init();
}

void AnalogInput::init()
{
    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {
        
        /* Disable power reduction for ADC */
        PRR &= ~_BV(PRADC);

        /* REFS1 = 0, REFS0 = 1   ==>   VCC with ext. cap. on AREF */
        ADMUX = _BV(REFS0);

        /* Prescaler /128   ==>   16MHz / 128 = 125KHz */
        ADCSRA = _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
        ADCSRB = 0;

        /* Disable digital IO on ADC pins 0 and 1 */
        DIDR0 = _BV(ADC1D) | _BV(ADC0D);
    }
}

void AnalogInput::startADCconversion( uint8_t mux)
{
    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {
        
        channelMux = mux;

        ADMUX &= 0xf0;
        ADMUX |= mux & 0x0f;

        adcState = ANALOG_WORKING;

        ADCSRA |= _BV(ADEN) | _BV(ADSC) | _BV(ADIE);
    }
}

value_t AnalogInput::channelValue( channel_t channel) {
    
    analog_t a;
    analog_t da;
    value_t v;
    value_t dv;
    
    /* Convert:
     * 
     * analog_t
     * ========
     * analogChannels.calibrateMid[ channel ]   typically  511
     * analogChannels.calibrateMin[ channel ]   typically  0
     * analogChannels.calibrateMax[ channel ]   typically  1023
     * 
     * to:
     * 
     * value_t
     * =======
     * CHANNELVALUE_MID   0
     * CHANNELVALUE_MIN   -1000
     * CHANNELVALUE_MAX   1000
     */


    /* DO NOT WRAP THIS INTO AN ATOMIC BLOCK.
     * THIS DISTURBS PPM GENERATION TOO MUCH.
     */
    if( channel < ANALOG_CHANNELS) {

            int32_t r_analog;
            int32_t r_value;

            a = analogChannels.channel[ channel ];

            if( a == analogChannels.calibrateMid[ channel ]) {
                dv = 0;
            } else if( a > analogChannels.calibrateMid[ channel ]) {
                da = a - analogChannels.calibrateMid[ channel ];
                r_analog = analogChannels.calibrateMax[ channel ] - analogChannels.calibrateMid[ channel ];
                r_value = CHANNELVALUE_MAX - CHANNELVALUE_MID;
                dv = (value_t)(( r_value * da ) / r_analog);
            } else /* a <  analogChannels.calibrateMid[ channel ] */ {
                da = analogChannels.calibrateMid[ channel ] - a;
                r_analog = analogChannels.calibrateMid[ channel ] - analogChannels.calibrateMin[ channel ];
                r_value = CHANNELVALUE_MID - CHANNELVALUE_MIN;
                dv = -(value_t)(( r_value * da ) / r_analog);
            }

            v = CHANNELVALUE_MID + dv;

    } else  {
        v = CHANNELVALUE_MID;
    }

    return v;
}

void AnalogInput::start() {

    analogInput.startADCconversion( 0);
}

uint8_t AnalogInput::state() {
    
    return analogInput.adcState;
}

value_t AnalogInput::getChannelValue( channel_t channel) {
 
    return analogInput.channelValue( channel); 
}

void AnalogInput::calibrateMid() {
    
}

void AnalogInput::calibrateStart() {
    
}

void AnalogInput::calibrateEnd() {
    
}

