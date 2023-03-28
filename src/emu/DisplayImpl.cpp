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

#include "DisplayImpl.h"

DisplayImpl::DisplayImpl( wxWindow *parent) : wxStaticBoxSizer(wxVERTICAL, parent, "UI") {

    lcd = new LcdWidget(parent, wxID_ANY, 160, 128, 1);
    AddSpacer(20);
    Add( lcd);
    AddSpacer(20);

    wxButton *b = new wxButton(parent, wxID_FORWARD, wxT("Up 10"));
    Add( b);
    b->Bind( wxEVT_BUTTON, &DisplayImpl::OnButton, this, wxID_FORWARD);
    
    b = new wxButton(parent, wxID_UP, wxT("Up"));
    Add( b);
    b->Bind( wxEVT_BUTTON, &DisplayImpl::OnButton, this, wxID_UP);

    b = new wxButton(parent, wxID_OK, wxT("Enter"));
    Add( b);
    b->Bind( wxEVT_BUTTON, &DisplayImpl::OnButton, this, wxID_OK);

    b = new wxButton(parent, wxID_CLEAR, wxT("Clear"));
    Add( b);
    b->Bind( wxEVT_BUTTON, &DisplayImpl::OnButton, this, wxID_CLEAR);

    b = new wxButton(parent, wxID_DOWN, wxT("Down"));
    Add( b);
    b->Bind( wxEVT_BUTTON, &DisplayImpl::OnButton, this, wxID_DOWN);

    b = new wxButton(parent, wxID_BACKWARD, wxT("Down 10"));
    Add( b);
    b->Bind( wxEVT_BUTTON, &DisplayImpl::OnButton, this, wxID_BACKWARD);

    AddSpacer(20);
}

void DisplayImpl::OnButton( wxCommandEvent &btnEvent) {

    switch( btnEvent.GetId()) {
        case wxID_FORWARD:
            key = KEY_UP;
            count = 10;
            break;
        case wxID_UP:
            key = KEY_UP;
            count = 1;
            break;
        case wxID_OK:
            key = KEY_ENTER;
            count = 1;
            break;
        case wxID_CLEAR:
            key = KEY_CLEAR;
            count = 1;
            break;
        case wxID_DOWN:
            key = KEY_DOWN;
            count = 1;
            break;
        case wxID_BACKWARD:
            key = KEY_DOWN;
            count = 10;
            break;

    }
}

LcdWidget *DisplayImpl::getLCD( void) {

    return lcd;
}

Event *DisplayImpl::getEvent() {

    event.key = key;
    event.count = count;

    key = KEY_NONE;
    count = 0;

    return &event;
}
