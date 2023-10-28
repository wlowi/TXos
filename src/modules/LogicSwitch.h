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

#ifndef _LogicSwitch_h_
#define _LogicSwitch_h_

#include "Module.h"

typedef struct logicSwitch_t {

    uint8_t  type[LOGIC_SWITCHES];
    
    switch_t swStateA[LOGIC_SWITCHES];
    switch_t swStateB[LOGIC_SWITCHES];
    switch_t swStateC[LOGIC_SWITCHES];

} logicSwitch_t;

class LogicSwitch : public Module {

    NON_PHASED_CONFIG( logicSwitch_t)

    private:
        char switchName[TEXT_SW_NAME_length +1];

    public:
        LogicSwitch();

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
