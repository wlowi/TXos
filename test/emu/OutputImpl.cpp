
#include "OutputImpl.h"

OutputImpl::OutputImpl( wxWindow *parent, int channels)
    : wxBoxSizer(wxHORIZONTAL)
{
    channelIDs = new wxWindowID[channels];
    gauges = new wxGauge*[channels];
    values = new wxStaticText*[channels];

    AddSpacer(10);

    for( int channel=0; channel<channels; channel++) {
        channelIDs[channel] = wxWindow::NewControlId();
        wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
        wxGauge *gauge = new wxGauge(parent, channelIDs[channel], 3000, wxDefaultPosition, wxSize(-1,300), wxSL_VERTICAL);
        gauges[channel] = gauge;
        vbox->Add( gauge);
        wxString str;
        str.Printf(wxT(" CH%d  "), channel+1);
        values[channel] = new wxStaticText(parent, wxID_ANY, str);
        vbox->Add( values[channel]);
        vbox->Add( new wxStaticText(parent, wxID_ANY, str));
        Add( vbox);
        AddSpacer(10);
        gauge->SetValue(1500);
    }
}

void OutputImpl::SetChannelValue(int channel, int value) {

    wxString str;
    str.Printf(wxT("%d"), value);
    values[channel]->SetLabel( str);
    gauges[channel]->SetValue( value+1500);
}

OutputImpl::~OutputImpl( void) {

    if( channelIDs != NULL) {
        delete [] channelIDs;
        delete [] gauges;
        delete [] values;
        channelIDs = NULL;
    }
}