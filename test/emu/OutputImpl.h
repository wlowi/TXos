
#ifndef _OutputImpl_h_
#define _OutputImpl_h_

#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "Controls.h"

class OutputImpl : public wxStaticBoxSizer
{
    private:
        wxWindowID *channelIDs = NULL;
        wxGauge **gauges = NULL;
        wxStaticText **values = NULL;

        long lastFrameMs = 0;
        
    public:
        OutputImpl( wxWindow *parent, int channels);
        ~OutputImpl( void);

        bool acceptChannels();
        void SetChannelValue( int channel, int value);
        uint16_t getOverrunCounter();
        timingUsec_t getMaxFrameTime();
};

#endif