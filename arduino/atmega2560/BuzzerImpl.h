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

#ifndef _BuzzerImpl_h_
#define _BuzzerImpl_h_

#include "Arduino.h"
#include "Buzzer.h"

/* System dependent implementations of Buzzer.
 * For more documentation see Buzzer.h
 */
 
class BuzzerImpl {

    private:
        Ports *ports;
        buzzerCmd_t sound[BUZZER_SOUND_LEN];
        const buzzerCmd_t *soundAlarm;
        bool alarm;

    public:
        BuzzerImpl();

        void init( Ports &p);
        
        void off();
        void play( const buzzerCmd_t s[]);
        void playPermanent( const buzzerCmd_t sound[]);

        /* Cannot be private because it is called from ISR */
        void processNext();

    private:
        void scheduleInterrupt( uint8_t t);
        void stopSound();
};

#endif
