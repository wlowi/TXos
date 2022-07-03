
#ifndef _InputImpl_h_
#define _InputImpl_h_

#include "TXos.h"
#include "Controls.h"

class InputImpl
{
    private:
        channel_t channels;
        switch_t switches;

        const uint8_t *analogPins;
        const uint8_t *switchPins;

        int *swValues = NULL;

    public:
        InputImpl( channel_t channels, const uint8_t analogPins[],
                   switch_t switches, const uint8_t switchPins[]);

        void init( switchSetConf_t conf);

        channel_t GetChannels();
        switch_t GetSwitches();
        channelValue_t GetChannelValue( channel_t ch);
        switchState_t GetSwitchValue( switch_t sw);
};

#endif
