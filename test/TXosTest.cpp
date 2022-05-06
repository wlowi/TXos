
#include "Arduino.h"

#include "InputBox.h"
#include "DisplayBox.h"
#include "OutputBox.h"

#include "EEPROM.h"

EEPROMClass EEPROM(4096);

extern void setup( void);
extern void loop( void);

InputBox *inputBox;
OutputBox *outputBox;
DisplayBox *displayBox;

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
    inputBox =  new InputBox( panel, 4, 4);
    hbox->Add( inputBox);
    hbox->AddSpacer(10);
    displayBox =  new DisplayBox( panel);
    hbox->Add( displayBox);
    hbox->AddSpacer(10);
    outputBox = new OutputBox( panel, 9);
    hbox->Add( outputBox);
    hbox->AddSpacer(10);

    panel->SetSizer(hbox);

    hbox->Fit( this);

    Bind(wxEVT_IDLE, &MyFrame::handleIdle, this);
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
