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

#include "SerialConsole.h"

static wxWindowID idOut = wxWindow::NewControlId();
static wxWindowID idIn = wxWindow::NewControlId();
static wxWindowID idSend = wxWindow::NewControlId();

SerialConsole::SerialConsole( EmuSerial &io) : wxFrame(nullptr, wxID_ANY, "Serial Console"), serial(io) {

    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

    outText = new wxTextCtrl( this, idOut, wxEmptyString, wxDefaultPosition, wxSize(640,320), wxTE_MULTILINE | wxTE_READONLY);
    vbox->Add( outText, 1, wxEXPAND | wxALL, 10);
    
    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
    inText = new wxTextCtrl( this, idIn, wxEmptyString, wxDefaultPosition, wxSize(640,120), wxTE_MULTILINE);
    hbox->Add( inText, 1, wxEXPAND);
    hbox->AddSpacer(10);

    wxButton *sendButton = new wxButton( this, idSend, "Send" );
    hbox->Add( sendButton, 0, wxEXPAND);
    
    vbox->Add( hbox, 0, wxEXPAND | wxALL, 10);

    SetSizerAndFit(vbox);

    Bind(wxEVT_CLOSE_WINDOW, &SerialConsole::OnClose, this);
    Bind(wxEVT_IDLE, &SerialConsole::handleIdle, this);
    sendButton->Bind(wxEVT_BUTTON, &SerialConsole::OnSend, this);
}

void SerialConsole::OnClose(wxCloseEvent& event)
{
    Show(false);
}

void SerialConsole::OnSend(wxCommandEvent& event) {

    wxString text = inText->GetValue();
    inText->Clear();

    serial.send( text.ToAscii());
}

void SerialConsole::handleIdle(wxIdleEvent& event)
{
    int ch;
    wxString text;

    while( (ch = serial.receive()) != -1 ) {
        text << (char)ch;
    }

    if( !text.IsEmpty()) {
        outText->AppendText( text);
    }

    event.RequestMore();
}