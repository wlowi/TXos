/*
  TXos. A remote control transmitter OS.

  MIT License

  Copyright (c) 2023 wlowi

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
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
        uint8_t sndIdx;
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
