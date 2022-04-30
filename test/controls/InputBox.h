
#ifndef _InputBox_h_
#define _InputBox_h_

#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "Arduino.h"

class InputBox : public wxBoxSizer
{
    private:
        int channels;
        int switches;

        wxWindowID *sliderIDs = NULL;
        int *chValues = NULL;
        wxWindowID *switchIDs = NULL;
        int *swValues = NULL;

    public:
        InputBox( wxWindow *parent, int channels, int switches);
        ~InputBox( void);

        int GetChannels();
        int GetSwitches();
        int GetChannelValue( int ch);
        int GetSwitchValue( int sw);

        void OnScroll( wxScrollEvent& event);
        void OnSwitch( wxCommandEvent& event);
};

#endif
