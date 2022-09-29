/*
    TXos. A remote control transmitter OS.

    Copyright (C) 2022 Wolfgang Lohwasser

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#ifndef _Buzzer_h_
#define _Buzzer_h_

#include "TXos.h"
#include "Ports.h"

/* 
 * Play a sound sequence.
 * 
 * How to use:
 * 
 * 1) Define a sequence
 * 
 *    const buzzerCmd_t SoundWelcome[] = {
 *      BUZZER_PLAY( 2),
 *      BUZZER_PAUSE( 2),
 *      BUZZER_REPEAT( 0, 3),
 *      BUZZER_STOP()
 *    };
 *  
 *    This means:
 *      [0] enable buzzer for 200 msec
 *      [1] pause for 200 msec
 *      [2] repeat from step 0 three times
 *      [3] turn buzzer off and stop
 * 
 * 2) Play in background
 * 
 *    buzzer.play( SoundWelcome);
 *    
 * 3) Cancel sound
 * 
 *    buzzer.off();
 *    
 * Note: Last command MUST be BUZZER_STOP()
 *       Maximum 8 steps (BUZZER_SOUND_LEN) including mandatory STOP.
 * 
 * 
 * Commands:
 * =========
 * 
 * BUZZER_STOP()                   00 000000
 * BUZZER_PLAY( time in 100ms )    01 tttttt
 * BUZZER_PAUSE( time in 100ms )   10 tttttt
 * BUZZER_REPEAT( step, count )    11 sssccc 
 * 
 * time  0 - 40  (4.0 sec)
 * step  0 - 7
 * count 0 - 7   (A count of 0 repeats forever)
 */

typedef uint8_t buzzerCmd_t;

#define BUZZER_STOP_CMD        0
#define BUZZER_STOP()          ((buzzerCmd_t)BUZZER_STOP_CMD)

#define BUZZER_PLAY_CMD        0x40
#define BUZZER_PLAY( t)        ((buzzerCmd_t)((BUZZER_PLAY_CMD) | ((t) & 0x3f)))

#define BUZZER_PAUSE_CMD       0x80
#define BUZZER_PAUSE( t)       ((buzzerCmd_t)((BUZZER_PAUSE_CMD) | ((t) & 0x3f)))

#define BUZZER_REPEAT_CMD      0xc0
#define BUZZER_REPEAT( s, c)   ((buzzerCmd_t)((BUZZER_REPEAT_CMD) | (((s) & 0x07) << 3) | ((c) & 0x07)))

/* Extract from buzzer command */
#define BUZZER_CMD( v)         ((v) & 0xc0)
#define BUZZER_TIME( v)        ((v) & 0x3f)
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
