

#ifndef _Ports_h_
#define _Ports_h_

#include "TXos.h"

class Ports {

    public:
        void init() const;

        void hfOn() const;
        void hfOff() const;

        void bindOn() const;
        void bindOff() const;

        void buzzerOn() const;
        void buzzerOff() const;
};

#endif
