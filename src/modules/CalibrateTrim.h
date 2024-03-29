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

#ifndef _CalibrateTrim_h_
#define _CalibrateTrim_h_

#include "Module.h"

/* Calibrate analog trim inputs
 * 
 * Implements the user interface to calibrate the trim inputs.
 * 
 * run() reads the ADC values and applies the calibrate range function.
 * The value stored in the control structure is between TRIMVALUE_MAX_LIMIT
 * and TRIMVALUE_MIN_LIMIT ( 250 to -250 or +/- 20% )
 */

typedef struct calibrateTrim_t {

    channelValue_t minPos[PORT_TRIM_INPUT_COUNT];
    channelValue_t midPos[PORT_TRIM_INPUT_COUNT];
    channelValue_t maxPos[PORT_TRIM_INPUT_COUNT];

} calibrateTrim_t;

class CalibrateTrim : public Module {

    NON_PHASED_CONFIG( calibrateTrim_t)

    private:
        uint8_t calibrationStep;
#define CALIBRATION_STEP_NONE     0
#define CALIBRATION_STEP_CENTER   1
#define CALIBRATION_STEP_MINMAX   2

    public:
        CalibrateTrim();

        /* From Module */
        void run( Controls &controls) final;
        void setDefaults() final;
        COMM_RC_t exportConfig( ImportExport *exporter, uint8_t *config) const;
        COMM_RC_t importConfig( ImportExport *importer, uint8_t *config) const;

        /* From TextUIScreen */
        uint8_t getRowCount() final;
        const char *getRowName( uint8_t row) final;
        uint8_t getColCount( uint8_t row) final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;

        bool isRowEditable( uint8_t row) final { return false; }
        bool isRowExecutable( uint8_t row) final;
        void rowExecute( TextUI *ui, uint8_t row ) final;

        /* Force UI update during calibration. */
        bool hasChanged( uint8_t row, uint8_t col) { return true; }
};

#endif
