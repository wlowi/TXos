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
