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

#ifndef _DualExpo_h_
#define _DualExpo_h_

#include "Module.h"

#define DUAL_EXPO_CHANNELS 3

typedef struct dualExpo_t {

    /* Dual Rate and expo for Aileron/Elevator/Rudder */
    percent_t rate[DUAL_EXPO_CHANNELS];
    percent_t expo[DUAL_EXPO_CHANNELS];

} dualExpo_t;

class DualExpo : public Module {

    PHASED_CONFIG( dualExpo_t )

    private:
        bool postRefresh;
        const char *phaseName;
        
        void applyRate( Controls &controls, channel_t ch, percent_t pct);
        void applyExpo( Controls &controls, channel_t ch, percent_t pct);

    public:
        DualExpo();

        /* From Module */
        void run( Controls &controls) final;
        void setDefaults() final;
        void exportConfig( Comm *exporter, uint8_t *config, moduleSize_t configSz) const;

        /* From TableEditable */
        bool needsRefresh() final;
        bool isRowEditable( uint8_t row) final;
        /* Return true if the value of this cell is editable */
        bool isColEditable( uint8_t row, uint8_t col) final;

        uint8_t getRowCount() final;
        const char *getRowName( uint8_t row) final;
        uint8_t getColCount( uint8_t row) final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;
};

#endif
