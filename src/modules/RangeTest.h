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

#ifndef _RangeTest_h_
#define _RangeTest_h_

#include "Module.h"

class RangeTest : public Module {

    NO_CONFIG()

    private:

        uint8_t rangeTestStep;
#define RANGETEST_STEP_NONE     0
#define RANGETEST_STEP_ACTIVE   1

        bool changed;

        void rangeTestOn();
        void rangeTestOff();

    public:
        RangeTest();

        /* From Module */
        void run( Controls &controls) final;
        void setDefaults() final;
        void exportConfig( Comm *exporter, uint8_t *config, moduleSize_t configSz) const {};
        void moduleExit() final;

        /* From TableEditable */
        uint8_t getRowCount() final;
        const char *getRowName( uint8_t row) final;
        uint8_t getColCount( uint8_t row) final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;

        bool isRowEditable( uint8_t row) final { return false; }
        bool isRowExecutable( uint8_t row) final;
        void rowExecute( TextUI *ui, uint8_t row ) final;
        bool hasChanged( uint8_t row, uint8_t col) final;
};

#endif
