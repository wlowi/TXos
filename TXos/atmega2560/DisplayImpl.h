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

#ifndef _DisplayImpl_h_
#define _DisplayImpl_h_

#include "Arduino.h"
#include "LcdWidget.h"

#define KEY_NONE        0
#define KEY_UP          1
#define KEY_ENTER       2
#define KEY_CLEAR       3
#define KEY_DOWN        4

class Event {

    public:
        uint8_t key;
        uint8_t count;

        void markProcessed() { key = KEY_NONE; }
        bool pending() { return key != KEY_NONE; }
};

class DisplayImpl {

    private:
        LcdWidget *lcd;
        Event event;

    public:
        DisplayImpl();
        void init();
        
        LcdWidget *getLCD( void);
        Event *getEvent();
};

#endif
