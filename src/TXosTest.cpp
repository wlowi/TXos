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

#include "Arduino.h"

#include "InputImpl.h"
#include "DisplayImpl.h"
#include "OutputImpl.h"
#include "PortsImpl.h"
#include "BuzzerImpl.h"

#include "EEPROM.h"

#include "time.h"

EEPROMClass EEPROM(4096);

extern void setup( void);
extern void loop( void);

InputImpl *inputImpl;
OutputImpl *outputImpl;
DisplayImpl *displayImpl;
PortsImpl *portsImpl;
BuzzerImpl *buzzerImpl;

SWITCH_CONFIGURATION

long millis() {

    return (long)clock() * 1000 / CLOCKS_PER_SEC;
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
        void handleIdle(wxIdleEvent& event);
};

enum
{
    ID_Hello = 1
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
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
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
 
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_CLOSE_WINDOW, &MyFrame::OnClose, this);

    wxPanel *panel = new wxPanel(this, -1);

    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

    hbox->AddSpacer(10);
    inputImpl =  new InputImpl( panel, 
                                PORT_ANALOG_INPUT_COUNT, PORT_TRIM_INPUT_COUNT, PORT_AUX_INPUT_COUNT,
                                PORT_SWITCH_INPUT_COUNT, switchConfiguration);
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

    portsImpl = new PortsImpl();
    buzzerImpl = new BuzzerImpl();
}
 
void MyFrame::OnClose(wxCloseEvent& event)
{
    EEPROM.saveToFile();
    Destroy();
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close();
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
    } else {
        setup();
        setup_done = true;
    }
}
