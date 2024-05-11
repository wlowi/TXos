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

#include "TXos.h"
#include "Controls.h"

/* 12 bit ADC => 4096 */
#define ADC_RESOLUTION 4096

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
