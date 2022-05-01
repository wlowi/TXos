
#ifndef _ServoReverse_h_
#define _ServoReverse_h_

#include "TXos.h"
#include "Module.h"

class ServoReverse : public Module {

    private:
        channelBits_t revBits;

    public:
        ServoReverse();
        void run( channelSet_t &channels);
};

#endif
