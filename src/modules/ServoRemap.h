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

#ifndef _ServoRemap_h_
#define _ServoRemap_h_

#include "Module.h"

/* Assign logical channels to servo channels */

typedef struct servoRemap_t {

    channel_t source[PPM_CHANNELS];

} servoRemap_t;

class ServoRemap : public Module {

    NON_PHASED_CONFIG( servoRemap_t)

    public:
        ServoRemap();

        /* Get source for servo channel. 
         * E.g. CHANNEL_THROTTLE
         * Used by servo test to skip throttle channel for safety.
         */
        channel_t getSourceForChannel( channel_t ch) { return ch < PPM_CHANNELS ? CFG->source[ch] : 0; }

        /* From Module */
        void run( Controls &controls) final;
        void setDefaults() final;
        void exportConfig( Comm *exporter, uint8_t *config, moduleSize_t configSz) const;

        /* From TableEditable */
        uint8_t getRowCount() final;
        const char *getRowName( uint8_t row) final;
        uint8_t getColCount( uint8_t row) final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;
};

#endif
