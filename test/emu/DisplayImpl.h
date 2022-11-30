
#ifndef _DisplayImpl_h_
#define _DisplayImpl_h_

#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "Arduino.h"
#include "LcdWidget.h"

#define KEY_NONE        0
#define KEY_UP          1
#define KEY_ENTER       2
#define KEY_CLEAR       3
#define KEY_DOWN        4

class Event {

    public:
        uint8_t key = KEY_NONE;
        uint8_t count = 1;

        void markProcessed() { key = KEY_NONE; }
        bool pending() const { return key != KEY_NONE; }
};

class DisplayImpl : public wxStaticBoxSizer {

    private:
        LcdWidget *lcd;
        uint8_t key;
        uint8_t count;
        Event event;

    public:
        DisplayImpl( wxWindow *parent);
        void OnButton( wxCommandEvent &event);
        
        LcdWidget *getLCD( void);
        Event *getEvent();
};

#endif