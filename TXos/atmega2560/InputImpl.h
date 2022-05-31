
#ifndef _InputImpl_h_
#define _InputImpl_h_

#include "Arduino.h"

class InputImpl
{
    private:
        int channels;
        int switches;
        int *chValues = NULL;
        int *swValues = NULL;

    public:
        InputImpl( int channels, int switches);

        int GetChannels();
        int GetSwitches();
        int GetChannelValue( int ch);
        int GetSwitchValue( int sw);
};

#endif
