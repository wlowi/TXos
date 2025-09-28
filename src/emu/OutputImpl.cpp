/*
  TXos. A remote control transmitter OS.

  MIT License

  Copyright (c) 2023 wlowi

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
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

    if( now >= lastFrameMs + PPM_FRAME_TIME_usec/1000 ) {
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

bool OutputImpl::isBindSupported() const {

    return true;
}

bool OutputImpl::isRangeTestSupported() const {

    return true;
}

void OutputImpl::bindActivate() {

    printf("Bind activated\n");
}

void OutputImpl::bindDeactivate() {

    printf("Bind deactivated\n");
}

void OutputImpl::rangeTestActivate() {

    printf("Bind test activated\n");
}

void OutputImpl::rangeTestDeactivate() {

    printf("Range test deactivated\n");
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
