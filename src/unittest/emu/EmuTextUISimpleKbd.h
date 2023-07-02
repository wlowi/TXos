
#ifndef _EmuTextUISimpleKbd_h_
#define _EmuTextUISimpleKbd_h_

class EmuTextUISimpleKbd : public TextUIInput {

    private:
        uint8_t key = KEY_NONE;
        uint8_t count = 0;
        Event event;

    public:
        EmuTextUISimpleKbd();

        /* TextUIInput */
        bool pending();
        void setEvent( Event *e);
};

#endif
