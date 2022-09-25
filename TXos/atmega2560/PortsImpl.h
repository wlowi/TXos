
#ifndef _PortsImpl_h_
#define _PortsImpl_h_

#include "Arduino.h"

class PortsImpl {

    public:
        PortsImpl();

        void portInit( uint8_t p, uint8_t t) const;
        void portSet( uint8_t p, uint8_t s) const;
        uint8_t portGet( uint8_t p) const;
};

#endif
