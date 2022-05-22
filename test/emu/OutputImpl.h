
#ifndef _OutputImpl_h_
#define _OutputImpl_h_

#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "Arduino.h"

class OutputImpl : public wxBoxSizer
{
    private:
        wxWindowID *channelIDs = NULL;
        wxGauge **gauges = NULL;
        wxStaticText **values = NULL;

    public:
        OutputImpl( wxWindow *parent, int channels);
        ~OutputImpl( void);

        void SetChannelValue( int channel, int value);
};

#endif