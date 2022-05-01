
#include "DisplayBox.h"

DisplayBox::DisplayBox( wxWindow *parent)
    : wxBoxSizer(wxVERTICAL)
{
    lcd = new LcdWidget(parent, wxID_ANY, 160, 128, 2);
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
            printf("OnButton %d UP\n", event.GetId());
            break;
        case wxID_OK:
            printf("OnButton %d OK\n", event.GetId());
            break;
        case wxID_DOWN:
            printf("OnButton %d DOWN\n", event.GetId());
            break;
    }
}

void DisplayBox::paint( void) 
{
    for( int x=1; x<100; x++) {
        lcd->setPixel( x, 100);
        lcd->setPixel( x, 102);
        lcd->setPixel( x, 104);
    }

    lcd->setBg( 255,0,0);
    lcd->println( "Hallo Welt!");
    lcd->setFontSize(2);
    lcd->println( "Hallo Welt!");
    lcd->setFontSize(3);
    lcd->print( "Hallo Welt!");
}