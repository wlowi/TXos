
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
