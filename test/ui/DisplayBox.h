
#ifndef _DisplayBox_h_
#define _DisplayBox_h_

#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "Arduino.h"
#include "LcdWidget.h"

class DisplayBox : public wxBoxSizer
{
    private:
        LcdWidget *lcd;

    public:
        DisplayBox( wxWindow *parent);
        void OnButton( wxCommandEvent &event);
        
        void paint( void);
};

#endif