
#include "EmuTextUISimpleKbd.h"

EmuTextUISimpleKbd::EmuTextUISimpleKbd( wxWindow *parent) : TextUIInput(), wxStaticBoxSizer(wxVERTICAL, parent, "UI") {

    AddSpacer(20);

    wxButton *b = new wxButton(parent, wxID_FORWARD, wxT("Up 10"));
    Add( b);
    b->Bind( wxEVT_BUTTON, &EmuTextUISimpleKbd::OnButton, this, wxID_FORWARD);
    
    b = new wxButton(parent, wxID_UP, wxT("Up"));
    Add( b);
    b->Bind( wxEVT_BUTTON, &EmuTextUISimpleKbd::OnButton, this, wxID_UP);

    b = new wxButton(parent, wxID_OK, wxT("Enter"));
    Add( b);
    b->Bind( wxEVT_BUTTON, &EmuTextUISimpleKbd::OnButton, this, wxID_OK);

    b = new wxButton(parent, wxID_CLEAR, wxT("Clear"));
    Add( b);
    b->Bind( wxEVT_BUTTON, &EmuTextUISimpleKbd::OnButton, this, wxID_CLEAR);

    b = new wxButton(parent, wxID_DOWN, wxT("Down"));
    Add( b);
    b->Bind( wxEVT_BUTTON, &EmuTextUISimpleKbd::OnButton, this, wxID_DOWN);

    b = new wxButton(parent, wxID_BACKWARD, wxT("Down 10"));
    Add( b);
    b->Bind( wxEVT_BUTTON, &EmuTextUISimpleKbd::OnButton, this, wxID_BACKWARD);

    AddSpacer(20);
}

void EmuTextUISimpleKbd::OnButton( wxCommandEvent &btnEvent) {

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

bool EmuTextUISimpleKbd::pending() {

    return key != KEY_NONE;
}

void EmuTextUISimpleKbd::setEvent( Event *e) {

    e->setKeyEvent( key, count);

    key = KEY_NONE;
    count = 0;
}
