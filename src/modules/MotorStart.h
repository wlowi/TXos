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

#ifndef _MotorStart_h_
#define _MotorStart_h_

#include "Module.h"


/*
 * Leading time before the motor starts.
 * Leading time is triggered by moving the gas stick above trigger_pct.
 * After leading time expires the motor starts to turn and full
 * throttle is reached after delay time.
 */
#define LEADING_MAX_SEC   (10)
/*
 * Delay is the number of seconds from 0 to 100%.
 * The resolution is 1/10 of a second.
 */
#define DELAY_MAX_SEC     (10)

typedef enum {

    STATE_INACTIVE = 0,
    STATE_ACTIVATED,
    STATE_TRIGGERED,
    STATE_START,
    STATE_DEACTIVATING

} motorStartState_t;

typedef struct motorStart_t {

    phase_t phase;
    switch_t activateSw;
    percent_t trigger_pct;
    float1 leadingTime;
    float1 delayTime;

} motorStart_t;

class MotorStart : public Module {

    NON_PHASED_CONFIG( motorStart_t)

    private:
        motorStartState_t state;
        unsigned long endTime_msec;
        // Last value of motor channel (times 10)
        int16_t lastChannelValue10;
        bool inModule;

    public:
        MotorStart();
        bool isActive();

        /* From module */
        void run( Controls &controls) final;
        void setDefaults() final;
        COMM_RC_t exportConfig( ImportExport *exporter, uint8_t *config) const;
        COMM_RC_t importConfig( ImportExport *importer, uint8_t *config) const;
        void moduleEnter() { inModule = true; }
        void moduleExit() { inModule = false; }

        /* From TableEditable */
        uint8_t getRowCount() final;
        const char *getRowName( uint8_t row) final;
        uint8_t getColCount( uint8_t row) final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;
};

#endif
