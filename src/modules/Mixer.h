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

#ifndef _Mixer_h_
#define _Mixer_h_

#include "Module.h"

typedef struct mixer_t {

    switch_t mixSw[MIXER];

    /* source and target are mix channel numbers 
     * To get the real channel number they need to be 
     * mapped via real_channel = mixChannelMap[ source ]
     */
    channel_t source[MIXER];
    channel_t target[MIXER];

    percent_t mixPct[MIXER];
    percent_t mixOffset[MIXER];

} mixer_t;

class Mixer : public Module {

    NON_PHASED_CONFIG( mixer_t)

    private:
        char mixerName[5];

        channelValue_t mixValue( channelValue_t v, uint8_t mix);
        void limitChannels( Controls &controls);

    public:
        Mixer();

        /* From Module */
        void run( Controls &controls) final;
        void setDefaults() final;
        void exportConfig( Exporter *exporter, uint8_t *config, moduleSize_t configSz) const;

        /* From TableEditable */
        uint8_t getRowCount() final;
        const char *getRowName( uint8_t row) final;
        uint8_t getColCount( uint8_t row) final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;
};
#endif