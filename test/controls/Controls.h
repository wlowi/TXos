
#ifndef _Controls_h_
#define _Controls_h_

#include "TXos.h"

class Controls {

    public:
        Controls();
        void init();
        void GetControlValues( channelSet_t &channels );
};

#endif