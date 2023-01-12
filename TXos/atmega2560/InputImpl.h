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
