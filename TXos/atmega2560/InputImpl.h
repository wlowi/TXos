
#ifndef _InputImpl_h_
#define _InputImpl_h_

#include "TXos.h"
#include "Controls.h"

/* 10 bit ADC => 1023 */
#define ADC_RESOLUTION 1023

/* Invert raw channel values */
#define INVERT_CH1
#define INVERT_CH2
#undef  INVERT_CH3
#define INVERT_CH4

class InputImpl
{
    private:
        channel_t stickCount;
        channel_t trimCount;
        channel_t auxCount;

        const switchConf_t *switchConf;

        channelValue_t GetAnalogValue( channel_t ch);

    public:
        const uint8_t *analogPins;
        const uint8_t *switchPins;

        channelValue_t *adcValues = NULL;

        channel_t adcInputs;  /* Total number of ADC inputs */
        switch_t switches;

        uint8_t mux;

        InputImpl( channel_t stickCnt, channel_t trimCnt, channel_t auxCnt, const uint8_t analogPins[],
                   switch_t switches, const switchConf_t *conf, const uint8_t switchPins[]);

        void init();

        void start();
        void setMux();

        switch_t GetSwitches();

        channelValue_t GetStickValue( channel_t ch);
        channelValue_t GetTrimValue( channel_t ch);
        channelValue_t GetAuxValue( channel_t ch);
        switchState_t GetSwitchValue( switch_t sw);
        switchConf_t GetSwitchConf( switch_t sw);
};

#endif
