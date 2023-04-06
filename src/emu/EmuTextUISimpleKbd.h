
#ifndef _EmuTextUISimpleKbd_h_
#define _EmuTextUISimpleKbd_h_

#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "TextUI.h"

class EmuTextUISimpleKbd : public TextUIInput, public wxStaticBoxSizer {

    private:
        uint8_t key = KEY_NONE;
        uint8_t count = 0;
        Event event;

    public:
        EmuTextUISimpleKbd( wxWindow *parent);
        void OnButton( wxCommandEvent &event);

        /* TextUIInput */
        bool pending();
        void setEvent( Event *e);
};

#endif