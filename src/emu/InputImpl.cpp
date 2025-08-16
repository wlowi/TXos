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

#include "InputImpl.h"

#define SLIDER_MIN   300
#define SLIDER_MAX   700
#define SLIDER_INIT  500

InputImpl::InputImpl( wxWindow *parent,
                      channel_t stickCnt, channel_t trimCnt, channel_t auxCnt,
                      uint8_t switches, const switchConf_t *conf)
    : wxBoxSizer(wxVERTICAL)
{
    this->stickCount = stickCnt;
    this->trimCount = trimCnt;
    this->auxCount = auxCnt;

    this->channels = stickCount + trimCount + auxCount;
    this->switches = switches;
    this->switchConf = conf;

    sliderIDs = new wxWindowID[channels];
    chValues = new channelValue_t[channels];

    for( int i=0; i<channels; i++) {
        chValues[i] = 0;
    }

    switchIDs = new wxWindowID[switches];
    swValues = new switchState_t[switches];

    for( int i=0; i<switches; i++) {
        swValues[i] = SW_STATE_0;
    }

    /* A horizontal box that contains sliders (stick, trim and aux analog inputs) */

    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer *hboxInner;

    for( int channel=0; channel<channels; channel++) {

        if( channel == 0) {
            hboxInner = new wxStaticBoxSizer( wxHORIZONTAL, parent, "Sticks");
            hbox->Add( hboxInner);
            hbox->AddSpacer(10);
#if STICK_TRIM == ANALOG_TRIM
        } else if( channel == stickCount) {
            hboxInner = new wxStaticBoxSizer(wxHORIZONTAL, parent, "Trim");
            hbox->Add( hboxInner);
            hbox->AddSpacer(10);
#endif
        } else if( channel == stickCount + trimCount) {
            hboxInner = new wxStaticBoxSizer(wxHORIZONTAL, parent, "Aux");
            hbox->Add( hboxInner);
            hbox->AddSpacer(10);
        }

        /* A vertical box that contains the slider and the slider name as static text */
        wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

        sliderIDs[channel] = wxWindow::NewControlId();
        wxSlider *slider = new wxSlider(parent, sliderIDs[channel],SLIDER_INIT,SLIDER_MIN,SLIDER_MAX, wxDefaultPosition, wxSize(-1,200), wxSL_VERTICAL | wxSL_INVERSE);
        vbox->Add( slider);
        wxString str;
        if( channel < stickCount) {
            str.Printf(wxT("CH%d"), channel+1);
#if STICK_TRIM == ANALOG_TRIM
        } else if( channel < stickCount + trimCount) {
            str.Printf(wxT("Trim%d"), channel+1-stickCount);
#endif
        } else {
            str.Printf(wxT("AUX%d"), channel+1-stickCount-trimCount);        
        }
        vbox->Add( new wxStaticText(parent, wxID_ANY, str));

        hboxInner->Add( vbox);
        hboxInner->AddSpacer(10);

        slider->Bind( wxEVT_SCROLL_THUMBTRACK, &InputImpl::OnScroll, this, sliderIDs[channel]);
        chValues[channel] = SLIDER_INIT;
    }

    Add( hbox);
    AddSpacer(10); 

    /* Another horizontal box that contains all switches */

    hbox = new wxStaticBoxSizer(wxHORIZONTAL, parent, "Switches");
    wxString choices[] = {wxT("0"), wxT("1"), wxT("2")};
    for( int sw = 0; sw < switches; sw++) {
        switchConf_t conf = switchConf[sw];

        wxRadioBox *swtch = nullptr;
        switchIDs[sw] = wxWindow::NewControlId();
        wxString str;

        switch( conf) {
            case SW_CONF_2STATE:
                str.Printf(wxT("SW%d"), sw+1);
                swtch = new wxRadioBox(parent, switchIDs[sw], str, wxDefaultPosition, wxDefaultSize, 2, choices, 1, wxRA_SPECIFY_COLS);
                break;

            case SW_CONF_3STATE:
                str.Printf(wxT("SW%d"), sw+1);
                swtch = new wxRadioBox(parent, switchIDs[sw], str, wxDefaultPosition, wxDefaultSize, 3, choices, 1, wxRA_SPECIFY_COLS);
                break;

            default:
                continue;
        }

        hbox->Add(swtch);
        hbox->AddSpacer(10);
        swtch->Bind( wxEVT_RADIOBOX, &InputImpl::OnSwitch, this, switchIDs[sw]);
        swtch->SetSelection( 0);
    }

    Add( hbox);
    AddSpacer(10);
}

InputImpl::~InputImpl( void) {

    if( sliderIDs != NULL) {
        delete [] sliderIDs;
        delete [] chValues;
        sliderIDs = NULL;
        chValues = NULL;
        delete [] switchIDs;
        delete [] swValues;
        switchIDs = NULL;
        swValues = NULL;
    }
}

void InputImpl::init() {

    /* This is a no-op as all the initialization
     * has already be done in the constructor.
     */
}

uint8_t InputImpl::GetSwitches() {

    return switches;
}


channelValue_t InputImpl::GetStickValue( int ch) {

    return chValues[ch];
}

channelValue_t InputImpl::GetTrimValue( int ch) {

#if STICK_TRIM == ANALOG_TRIM
    return chValues[ch + stickCount];
#else
    return 0;
#endif
}

channelValue_t InputImpl::GetAuxValue( int ch) {

    return chValues[ch + stickCount + trimCount];
}

switchState_t InputImpl::GetSwitchValue( int sw) {

    return swValues[sw];
}

switchConf_t InputImpl::GetSwitchConf( int sw) {

    return switchConf[sw];
}

void InputImpl::OnScroll( wxScrollEvent& event) {

    for( int i=0; i<channels; i++) {
        if( sliderIDs[i] == event.GetId()) {
            chValues[i] = event.GetPosition();
//            printf("HandleScroll %d %d\n", event.GetId(), values[i]);
            break;
        }
    }
    
}

void InputImpl::OnSwitch( wxCommandEvent& event) {

    for( int i=0; i<switches; i++) {
        if( switchIDs[i] == event.GetId()) {
            switch( event.GetSelection()) {
                case 0:
                    swValues[i] = SW_STATE_0;
                    break;

                case 1:
                    swValues[i] = SW_STATE_1;
                    break;

                case 2:
                    swValues[i] = SW_STATE_2;
                    break;

                default:
                    swValues[i] = SW_STATE_DONTCARE;
            }
            printf("InputImpl::OnSwitch(): HandleSwitch %d %d\n", i, swValues[i]);
            break;
        }
    }
}
