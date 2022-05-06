
#include "DisplayBox.h"

DisplayBox::DisplayBox( wxWindow *parent)
    : wxBoxSizer(wxVERTICAL)
{
    key = KEY_NONE;
    lcd = new LcdWidget(parent, wxID_ANY, 160, 128, 1);
    Add( lcd);
    AddSpacer(20);

    wxButton *b = new wxButton(parent, wxID_UP, wxT("Up"));
    Add( b);
    b->Bind( wxEVT_BUTTON, &DisplayBox::OnButton, this, wxID_UP);
    
    b = new wxButton(parent, wxID_OK, wxT("Enter"));
    Add( b);
    b->Bind( wxEVT_BUTTON, &DisplayBox::OnButton, this, wxID_OK);
    
    b = new wxButton(parent, wxID_DOWN, wxT("Down"));
    Add( b);
    b->Bind( wxEVT_BUTTON, &DisplayBox::OnButton, this, wxID_DOWN);
}

void DisplayBox::OnButton( wxCommandEvent &event) {

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

LcdWidget *DisplayBox::getLCD( void) 
{
    return lcd;
}

uint8_t DisplayBox::getKey() {

    uint8_t k = key;
    key = KEY_NONE;
    return k;
}
