
#ifndef _DisplayBox_h_
#define _DisplayBox_h_

#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "Arduino.h"
#include "LcdWidget.h"

#define KEY_NONE        0
#define KEY_UP          1
#define KEY_ENTER       2
#define KEY_DOWN        3


class DisplayBox : public wxBoxSizer
{
    private:
        LcdWidget *lcd;
        uint8_t key;

    public:
        DisplayBox( wxWindow *parent);
        void OnButton( wxCommandEvent &event);
        
        LcdWidget *getLCD( void);
        uint8_t getKey();
};

#endif