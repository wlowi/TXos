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

#include "Arduino.h"

#include "InputImpl.h"
#include "DisplayImpl.h"
#include "OutputImpl.h"
#include "PortsImpl.h"
#include "BuzzerImpl.h"

#include "EEPROM.h"
#include "EmuSerial.h"
#include "SerialConsole.h"
#include "time.h"

#include <wx/utils.h>

EEPROMClass EEPROM(4096);
SerialConsole *serialConsole;
EmuSerial Serial;

extern void setup( void);
extern void loop( void);

InputImpl *inputImpl;
OutputImpl *outputImpl;
DisplayImpl *displayImpl;
PortsImpl *portsImpl;
BuzzerImpl *buzzerImpl;


static wxWindowID idConsole = wxWindow::NewControlId();


SWITCH_CONFIGURATION

unsigned long millis() {

    static wxLongLong startTime = 0;

    wxLongLong now = wxGetUTCTimeMillis();

    if( startTime == 0) {
        startTime = now;
    }

    return (now - startTime).GetLo();
}

class TXosTest : public wxApp
{
    public:
        virtual bool OnInit();
};

class MyFrame : public wxFrame
{
    public:
        MyFrame();

    private:
        void OnClose(wxCloseEvent& event);
        void OnExit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnConsole(wxCommandEvent& event);
        void handleIdle(wxIdleEvent& event);
};

wxIMPLEMENT_APP(TXosTest);

bool TXosTest::OnInit()
{
    EEPROM.loadFromFile();

    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "TXosTest")
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(idConsole, "&Console", "Open Serial Console Emulation");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar( menuBar );

    CreateStatusBar();
    SetStatusText("TXosTest");

    Bind(wxEVT_MENU, &MyFrame::OnConsole, this, idConsole);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_CLOSE_WINDOW, &MyFrame::OnClose, this);

    wxPanel *panel = new wxPanel(this, -1);

    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

    hbox->AddSpacer(10);
#if STICK_TRIM == ANALOG_TRIM
    inputImpl =  new InputImpl( panel,
                                PORT_ANALOG_INPUT_COUNT, PORT_TRIM_INPUT_COUNT, PORT_AUX_INPUT_COUNT,
                                PORT_SWITCH_INPUT_COUNT, switchConfiguration);
#else
    inputImpl =  new InputImpl( panel,
                                PORT_ANALOG_INPUT_COUNT, 0, PORT_AUX_INPUT_COUNT,
                                PORT_SWITCH_INPUT_COUNT, switchConfiguration);
#endif
    hbox->Add( inputImpl);
    hbox->AddSpacer(10);
    displayImpl =  new DisplayImpl( panel);
    hbox->Add( displayImpl);
    hbox->AddSpacer(10);
    outputImpl = new OutputImpl( panel, 9);
    hbox->Add( outputImpl);
    hbox->AddSpacer(10);

    panel->SetSizer(hbox);

    hbox->Fit( this);

    Bind(wxEVT_IDLE, &MyFrame::handleIdle, this);

    serialConsole = new SerialConsole( Serial);

    portsImpl = new PortsImpl();
    buzzerImpl = new BuzzerImpl();
}

void MyFrame::OnClose(wxCloseEvent& event)
{
    serialConsole->Show(false);
    serialConsole->Destroy();
    EEPROM.saveToFile();
    Destroy();
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close();
}

void MyFrame::OnConsole(wxCommandEvent& event)
{
    if( serialConsole->IsShown()) {
        serialConsole->Show(false);
    } else {
        serialConsole->Show(true);
    }
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    //wxMessageBox("TXos Test Helper",
    //             "TXos Test Helper", wxOK | wxICON_INFORMATION);

}

void MyFrame::handleIdle(wxIdleEvent& event)
{
    static bool setup_done = false;

    event.RequestMore();

    if( setup_done) {
        loop();
        wxMicroSleep(1000);
    } else {
        setup();
        setup_done = true;
    }
}
