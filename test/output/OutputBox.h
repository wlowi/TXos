
#ifndef _OutputBox_h_
#define _OutputBox_h_

#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "Arduino.h"

class OutputBox : public wxBoxSizer
{
    private:
        wxWindowID *channelIDs = NULL;
        wxGauge **gauges = NULL;
        wxStaticText **values = NULL;

    public:
        OutputBox( wxWindow *parent, int channels);
        ~OutputBox( void);

        void SetChannelValue( int channel, int value);
};

#endif