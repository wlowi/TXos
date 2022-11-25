
#ifndef _InputImpl_h_
#define _InputImpl_h_

#include "TXos.h"
#include "Controls.h"

class InputImpl
{
    public:
        
        const uint8_t *analogPins;
        const uint8_t *switchPins;

        int *swValues = NULL;
        channelValue_t *adcValues = NULL;

        channel_t adcInputs;
        switch_t switches;

        uint8_t mux;

        InputImpl( channel_t adcInputs, const uint8_t analogPins[],
                   switch_t switches, const uint8_t switchPins[]);

        void init( switchSetConf_t conf);

        void start();
        void setMux();
        
        channel_t GetChannels();
        switch_t GetSwitches();
        channelValue_t GetChannelValue( channel_t ch);
        switchState_t GetSwitchValue( switch_t sw);
};

#endif
