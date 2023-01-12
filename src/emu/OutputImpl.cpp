/*
    TXos. A remote control transmitter OS.

    Copyright (C) 2022 Wolfgang Lohwasser

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

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