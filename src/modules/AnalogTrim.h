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

#ifndef _AnalogTrim_h_
#define _AnalogTrim_h_

#include "Module.h"

/* Adjust servo subtrim to current trim values */

typedef struct analogTrim_t {

    /* Trim values of analog input channels! */
    percent_t storedTrim_pct[PORT_TRIM_INPUT_COUNT];

} analogTrim_t;

class AnalogTrim : public Module {

    NON_PHASED_CONFIG( analogTrim_t)

    private:
        Controls *current;

        /* Trim values of analog input channels! */
        percent_t trim_pct[PORT_TRIM_INPUT_COUNT];

        uint8_t execStep;
#define ANALOGTRIM_STEP_NONE     0
#define ANALOGTRIM_STEP_SAVE     1
#define ANALOGTRIM_STEP_ADJUST   2

    public:
        AnalogTrim();

        /* From Module */
        void run( Controls &controls) final;
        void setDefaults() final;
        COMM_RC_t exportConfig( ImportExport *exporter, uint8_t *config) const;
        COMM_RC_t importConfig( ImportExport *importer, uint8_t *config) const;

        void moduleEnter() final;
        void moduleExit() final;

        /* From TableEditable */
        uint8_t getRowCount() final;
        const char *getRowName( uint8_t row) final;
        uint8_t getColCount( uint8_t row) final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;

        bool isColEditable( uint8_t row, uint8_t col) final;

        /* Force UI update during calibration. */
        bool hasChanged( uint8_t row, uint8_t col) final { return true; }
};

#endif
