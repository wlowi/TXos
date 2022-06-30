
#ifndef _InputImpl_h_
#define _InputImpl_h_

#include "TXos.h"

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

        channel_t GetChannels();
        switch_t GetSwitches();
        int GetChannelValue( channel_t ch);
        int GetSwitchValue( switch_t sw);
};

#endif
