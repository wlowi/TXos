
#ifndef _InputImpl_h_
#define _InputImpl_h_

#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "Arduino.h"
#include "Controls.h"

class InputImpl : public wxBoxSizer
{
    private:
        channel_t channels;
        switch_t switches;
        switchSetConf_t switchConf;

        channel_t stickCount;
        channel_t trimCount;
        channel_t auxCount;

        wxWindowID *sliderIDs = nullptr;
        channelValue_t *chValues = nullptr;
        wxWindowID *switchIDs = nullptr;
        switchState_t *swValues = nullptr;

    public:
        explicit InputImpl( wxWindow *parent,
                            channel_t stickCnt, channel_t trimCnt, channel_t auxCnt,
                            switch_t switches, switchSetConf_t conf);

        ~InputImpl( void) final;

        void init();

        switch_t GetSwitches();

        channelValue_t GetStickValue( int ch);
        channelValue_t GetTrimValue( int ch);
        channelValue_t GetAuxValue( int ch);
        switchState_t GetSwitchValue( int sw);
        switchSetConf_t GetSwitchSetConf();

        void OnScroll( wxScrollEvent& event);
        void OnSwitch( wxCommandEvent& event);
};

#endif
