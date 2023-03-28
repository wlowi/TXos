/*
  TXos. A remote control transmitter OS.

  MIT License

  Copyright (c) 2023 wlowi

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
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
