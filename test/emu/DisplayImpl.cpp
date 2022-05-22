
#include "DisplayImpl.h"

DisplayImpl::DisplayImpl( wxWindow *parent) : wxBoxSizer(wxVERTICAL) {

    key = KEY_NONE;
    lcd = new LcdWidget(parent, wxID_ANY, 160, 128, 1);
    Add( lcd);
    AddSpacer(20);

    wxButton *b = new wxButton(parent, wxID_UP, wxT("Up"));
    Add( b);
    b->Bind( wxEVT_BUTTON, &DisplayImpl::OnButton, this, wxID_UP);
    
    b = new wxButton(parent, wxID_OK, wxT("Enter"));
    Add( b);
    b->Bind( wxEVT_BUTTON, &DisplayImpl::OnButton, this, wxID_OK);
    
    b = new wxButton(parent, wxID_DOWN, wxT("Down"));
    Add( b);
    b->Bind( wxEVT_BUTTON, &DisplayImpl::OnButton, this, wxID_DOWN);
}

void DisplayImpl::OnButton( wxCommandEvent &event) {

    switch( event.GetId()) {
        case wxID_UP:
            key = KEY_UP;
            break;
        case wxID_OK:
            key = KEY_ENTER;
            break;
        case wxID_DOWN:
            key = KEY_DOWN;
            break;
    }
}

LcdWidget *DisplayImpl::getLCD( void) {

    return lcd;
}

Event *DisplayImpl::getEvent() {

    event.key = key;
    event.count = 1;

    key = KEY_NONE;

    return &event;
}
