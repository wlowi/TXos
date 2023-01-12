/*
    TXos. A remote control transmitter OS.

    Copyright (C) 2022 Wolfgang Lohwasser

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

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
