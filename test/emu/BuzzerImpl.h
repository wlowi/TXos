
#ifndef _BuzzerImpl_h_
#define _BuzzerImpl_h_

#include "Arduino.h"
#include "Buzzer.h"

class BuzzerImpl {

    public:
        BuzzerImpl();

        void init( Ports &p);
        
        void off();
        void play( const buzzerCmd_t sound[]);
};

#endif