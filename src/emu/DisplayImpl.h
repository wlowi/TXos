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
        uint8_t count = 0;

        void markProcessed() { key = KEY_NONE; }
        bool pending() const { return key != KEY_NONE; }
};

class DisplayImpl : public wxStaticBoxSizer {

    private:
        LcdWidget *lcd;
        uint8_t key = KEY_NONE;
        uint8_t count = 0;
        Event event;

    public:
        DisplayImpl( wxWindow *parent);
        void OnButton( wxCommandEvent &event);
        
        LcdWidget *getLCD( void);
        Event *getEvent();
};

#endif