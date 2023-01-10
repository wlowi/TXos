
#ifndef _DisplayImpl_h_
#define _DisplayImpl_h_

#include "Arduino.h"
#include "LcdWidget.h"
#include "stdlib.h"
#include "cstring"

#define KEY_NONE        0
#define KEY_UP          1
#define KEY_ENTER       2
#define KEY_CLEAR       3
#define KEY_DOWN        4

class Event {

    public:
        uint8_t key = KEY_NONE;
        uint8_t count = 0;

        void markProcessed() { key = KEY_NONE; }
        bool pending() const { return key != KEY_NONE; }
};

class DisplayImpl {

    private:
        LcdWidget *lcd;
        uint8_t key = KEY_NONE;
        uint8_t count = 0;
        Event event;

    public:
        DisplayImpl();
        
        LcdWidget *getLCD( void);
        Event *getEvent();
};

#endif