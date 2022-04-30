
#include "DisplayBox.h"

DisplayBox::DisplayBox( wxWindow *parent)
    : wxBoxSizer(wxHORIZONTAL)
{
    lcd = new LcdWidget(parent, wxID_ANY, 160, 128, 2);
    Add( lcd);
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