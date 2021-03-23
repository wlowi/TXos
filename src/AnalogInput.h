/*
 * AnalogInput.h
 * 
 * History:
 * 
 *   wolfix   31.Dec.2019  Creation
 */

#ifndef _AnalogInput_h
#define _AnalogInput_h

#include "Arduino.h"
#include "TXos.h"

/* Numer of supported channels. */
#define ANALOG_CHANNELS      ((channel_t)6)

/* AnalogInput state */
#define ANALOG_IDLE          ((uint8_t)0)
#define ANALOG_WORKING       ((uint8_t)1)
#define ANALOG_DONE          ((uint8_t)2)

typedef uint16_t analog_t;

typedef struct analogChannels_t {
    
    analog_t channel[ANALOG_CHANNELS];

    analog_t calibrateMid[ANALOG_CHANNELS];
    analog_t calibrateMin[ANALOG_CHANNELS];
    analog_t calibrateMax[ANALOG_CHANNELS];

} analogChannels_t;


class AnalogInput {
public:
    volatile analogChannels_t analogChannels;
    volatile uint8_t channelMux;
    volatile uint8_t adcState;

public:
    AnalogInput();
    
    void init();
    void startADCconversion( uint8_t mux);
    value_t channelValue( channel_t channel);

    static void start();
    static uint8_t state();

    static value_t getChannelValue( channel_t channel);
    
    static void calibrateMid();
    static void calibrateStart();
    static void calibrateEnd();
};

#endif
