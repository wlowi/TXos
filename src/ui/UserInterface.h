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

#ifndef _UserInterface_h_
#define _UserInterface_h_

#include "TXos.h"
#include "SelectList.h"
#include "TableEditable.h"
#include "Menu.h"
#include "VccMonitor.h"
#include "EngineCut.h"
#include "Phases.h"
#include "Timer.h"

#define SCREEN_INIT     0
#define SCREEN_HOME     1
#define SCREEN_MODEL    2
#define SCREEN_SYSTEM   3
#define SCREEN_CONFIG   4

#define REFRESH_OK      0
#define REFRESH_FULL    1
#define REFRESH_UPDATE  2

class UserInterface {

    private:
        LcdWidget *lcd;
        VccMonitor *vccMonitor;
        Phases *phases;
        Timer *timer;
        EngineCut *engineCut;

#define SCREEN_STACK_SIZE   5
        uint8_t screen[SCREEN_STACK_SIZE];
        uint8_t screenPtr;
        uint8_t refresh;
        
        uint8_t screenWidth;

        uint8_t message1; /* displayed messages */
        uint8_t message2;
        uint8_t post1;    /* posted messages */
        uint8_t post2;

        phase_t lastPhase;
        float2 lastVcc;
        uint16_t lastTime;
        bool engineSave;

        Module *module;
        SelectList selectList;
        uint8_t currentSelected;

        void homeScreen(Event *event);
        void menuScreen(Event *event, Menu *menu);
        void configScreen(Event *event);

    public:
        void init();
        void handle();

        void printDebug( uint16_t t);

        void postMessage( uint8_t line, uint8_t msg);
        void cancelEdit( TableEditable *toCancel);

        void toScreen( uint8_t scr);
        void switchScreen( uint8_t scr);
        void pushScreen( uint8_t scr);
        void popScreen();
};

#endif
