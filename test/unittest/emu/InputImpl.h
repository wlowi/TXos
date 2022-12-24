
#ifndef _InputImpl_h_
#define _InputImpl_h_

#include "Arduino.h"
#include "Controls.h"
#include "stdlib.h"
#include "cstring"

class InputImpl
{
    private:
        channel_t channels;
        uint8_t switches;
        
        const switchConf_t *switchConf;

        channel_t stickCount;
        channel_t trimCount;
        channel_t auxCount;

        channelValue_t *chValues = nullptr;
        switchState_t *swValues = nullptr;

    public:
        explicit InputImpl( channel_t stickCnt, channel_t trimCnt, channel_t auxCnt,
                            uint8_t switches, const switchConf_t *conf);

        void init();

        uint8_t GetSwitches();

        channelValue_t GetStickValue( int ch);
        channelValue_t GetTrimValue( int ch);
        channelValue_t GetAuxValue( int ch);
        switchState_t GetSwitchValue( int sw);
        switchConf_t GetSwitchConf( int sw);
};

#endif
