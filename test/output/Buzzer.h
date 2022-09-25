
#ifndef _Buzzer_h_
#define _Buzzer_h_

#include "TXos.h"
#include "Ports.h"

/* STOP()                   00 000000
 * PLAY( time in 100ms )    01 00tttt
 * PAUSE( time in 100ms )   10 00tttt
 * REPEAT( step, count )    11 sssccc 
 * 
 * time  0 - 15  (1.5 sec)
 * step  0 - 7
 * count 0 - 7
 */

typedef uint8_t buzzerCmd_t;

#define BUZZER_STOP_CMD        0
#define BUZZER_STOP()          ((buzzerCmd_t)BUZZER_STOP_CMD)

#define BUZZER_PLAY_CMD        0x40
#define BUZZER_PLAY( t)        ((buzzerCmd_t)((BUZZER_PLAY_CMD) | ((t) & 0x0f)))

#define BUZZER_PAUSE_CMD       0x80
#define BUZZER_PAUSE( t)       ((buzzerCmd_t)((BUZZER_PAUSE_CMD) | ((t) & 0x0f)))

#define BUZZER_REPEAT_CMD      0xc0
#define BUZZER_REPEAT( s, c)   ((buzzerCmd_t)((BUZZER_REPEAT_CMD) | (((s) & 0x07) << 3) | ((c) & 0x07)))

/* Extract from buzzer command */
#define BUZZER_CMD( v)         ((v) & 0xc0)
#define BUZZER_TIME( v)        ((v) & 0x0f)
#define BUZZER_STEP( v)        (((v) >> 3) & 0x07)
#define BUZZER_COUNT( v)       ((v) & 0x07)

#define BUZZER_SOUND_LEN       8



class Buzzer {

    Ports &ports;

    public:
        explicit Buzzer( Ports &p);

        void init();
        
        void off();
        void play( const buzzerCmd_t sound[]);
};

#endif
