
#ifndef _BuzzerImpl_h_
#define _BuzzerImpl_h_

#include "Arduino.h"
#include "Buzzer.h"

class BuzzerImpl {

    private:
        Ports *ports;
        buzzerCmd_t sound[BUZZER_SOUND_LEN];
        
    public:
        BuzzerImpl();

        void init( Ports &p);
        
        void off();
        void play( const buzzerCmd_t s[]);

        void processNext();
        void scheduleInterrupt( uint8_t t);
};

#endif
