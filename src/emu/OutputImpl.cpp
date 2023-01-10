
#include "OutputImpl.h"

OutputImpl::OutputImpl( wxWindow *parent, int channels)
    : wxStaticBoxSizer(wxHORIZONTAL, parent, "Servos")
{
    channelIDs = new wxWindowID[channels];
    gauges = new wxGauge*[channels];
    values = new wxStaticText*[channels];

    AddSpacer(20);

    for( int channel=0; channel<channels; channel++) {
        channelIDs[channel] = wxWindow::NewControlId();
        wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
        wxGauge *gauge = new wxGauge(parent, channelIDs[channel], 3000, wxDefaultPosition, wxSize(-1,300), wxSL_VERTICAL);
        gauges[channel] = gauge;
        vbox->AddSpacer(10);
        vbox->Add( gauge);
        wxString str;
        str.Printf(wxT(" CH%d  "), channel+1);
        values[channel] = new wxStaticText(parent, wxID_ANY, str);
        vbox->Add( values[channel]);
        vbox->AddSpacer(10);
        vbox->Add( new wxStaticText(parent, wxID_ANY, str));
        Add( vbox);

        gauge->SetValue(1500);
    }
}

bool OutputImpl::acceptChannels() {

    long now = millis();

    if( now >= lastFrameMs + PPM_FRAME_TIME_MSEC) {
        lastFrameMs = now;
        return true;
    } 

    return false;
}

void OutputImpl::SetChannelValue(int channel, int value) {

    wxString str;
    str.Printf(wxT("%d"), value);
    values[channel]->SetLabel( str);
    gauges[channel]->SetValue( value+1500);
}

uint16_t OutputImpl::getOverrunCounter() {

    return 0;
}

timingUsec_t OutputImpl::getMaxFrameTime() {

    return 0;
}

OutputImpl::~OutputImpl( void) {

    if( channelIDs != NULL) {
        delete [] channelIDs;
        delete [] gauges;
        delete [] values;
        channelIDs = NULL;
    }
}