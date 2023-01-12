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

        void unittestSetStickValue( channel_t ch, channelValue_t v);
        void unittestSetTrimValue( channel_t ch, channelValue_t v);
        void unittestSetSwitchValues( switchState_t a, switchState_t b, switchState_t c, switchState_t d);
};

#endif
