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

#ifndef _HomeScreen_h_
#define _HomeScreen_h_

#include "TXos.h"
#include "TextUI.h"

#include "VccMonitor.h"
#include "EngineCut.h"
#include "MotorStart.h"
#include "Phases.h"
#include "Timer.h"

class HomeScreen : public TextUIScreen {

private:
    TextUILcd* lcd = nullptr;
    VccMonitor* vccMonitor;
    Phases* phases;
    Timer* timer;
    EngineCut* engineCut;
    MotorStart *motorStart;

    uint8_t refresh = REFRESH_FULL;

    uint8_t screenWidth;

    uint8_t message1; /* displayed messages */
    uint8_t message2;
    uint8_t post1;    /* posted messages */
    uint8_t post2;
    uint8_t arg1;     /* message argument */
    uint8_t arg2;

    phase_t lastPhase;
    float2 lastVcc;
    uint16_t lastTime;
    bool engineSave;
    bool startActive;

public:
    HomeScreen();

    void printDebug(uint16_t t);

    /**
     * @brief Print a message into one of the two bottom lines.
     *
     * line == 0 print to second to last line
     * line == 1 print to last line
     * msg == message number (see Text_xx.h)
     * arg == 0 is no argument
     *
     * @param line which line
     * @param msg message number
     * @param arg single digit argument
     */
    void postMessage(uint8_t line, uint8_t msg, uint8_t arg);
    void postMessage(uint8_t line, uint8_t msg);

    /* From TextUIScreen */
    virtual void handleEvent(TextUI* ui, Event* e) final;

    void activate(TextUI* ui) final;

    /* This screen does its own painting in handleEvent().
     * We don't use the TextUI row/column handling.
     */
    bool goBackItem() { return false; }
    uint8_t getRowCount() { return 0; };
    const char* getRowName(uint8_t row) { return ""; }
    uint8_t getColCount(uint8_t row) { return 0; }
    void getValue(uint8_t row, uint8_t col, Cell* cell) {}
    const char* getMenuName() { return ""; };
    const char* getHeader() { return getMenuName(); }
};

#endif
