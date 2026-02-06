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

#ifndef _TextUIRotaryEncoder_h_
#define _TextUIRotaryEncoder_h_

#include "TextUI.h"

/**
 * @brief A driver for rotary encoder.
 *
 * The driver uses pin change interrupt 2 (PNINT2). This is fixed and cannot be changed unless the code in TextUIRotaryEncoder.cpp is modified.
 *
 * For a detailed description of supported pins see documentation of the constructor.
 *
 */
class TextUIRotaryEncoder : public TextUIInput
{
    private:
    	uint8_t pinClock;
    	uint8_t pinDir;
    	uint8_t pinButton;

	byte oldVal;

	unsigned long buttonDown_msec;
	boolean buttonIsDown;
	byte button;

	int enc;

    public:
        /**
         * @brief Construct a new TextUIRotaryEncoder object.
         *
         * Generated events:
         *
         * Action      | Event
         * ------------|---------
         * turn        |KEY_UP
         * turn        |KEY_DOWN
         * short press |KEY_ENTER
         * long press  |KEY_CLEAR
         *
         * Note: All 3 pins must be connected to the same pin change interrupt.
         *
         * Currently the pin change interrupt is fixed to PCINT2.
         * All pins must be connected to PCINT2.
         *
         * Arduino Nano (ATmega328):
         *
         *  Interrupt         | Chip Port     | Arduino Pin
         *  ------------------|---------------|-------------------------
         *  PCINT0 PCINT0-7   |(Port B0 - B7) |(PCINT0-5:   Pin D8 - D13)
         *  PCINT1 PCINT8-14  |(Port C0 - C6) |(PCINT8-13:  Pin A0 - A5)
         *  PCINT2 PCINT16-23 |(Port D0 - D7) |(PCINT16-23: Pin D0 - D7)
         *
         * Arduino Mega 2560:
         *
         *  Interrupt         |Chip Port      |Arduino Pin
         *  ------------------|---------------|---------------------------
         *  PCINT0 PCINT0-7   |(Port B0 - B7) |(PCINT4-7:   Pin D10 - D13)
         *  PCINT1 PCINT8     |(Port E0)      |(PCINT8:     Pin D0)
         *  PCINT1 PCINT9-15  |(Port J0 - J6) |(PCINT9-10:  Pin D15, D14)
         *  PCINT2 PCINT16-23 |(Port K0 - K7) |(PCINT16-23: Pin A8 -A15)
         *
         * Espressif ESP 32:
         *
         * Any usable GPIO pin.
         *
         * Example:
         *   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
         *   OK:   new TextUIRotaryEncoder( A8, A9, A10)
         *   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
         *
         *   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
         *   Not OK:   new TextUIRotaryEncoder( D14, A9, A10)
         *   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
         *
         * @param pinClock  Arduino pin of clock line.
         * @param pinDir    Arduino pin of direction line.
         * @param pinButton Arduino button pin.
         */
        TextUIRotaryEncoder( uint8_t pinClock, uint8_t pinDir, uint8_t pinButton);

        /**
         * @brief Pin change interrupt handler.
         *
         */
        void runISR();

        /* Returns true if there is an event pending.
         * setEvent() clears pending state.
         *
         * Note that there may be multiple events pending
         * for different keys. setEvent() clears the pending
         * state only for the key returned.
         */
        bool pending();
        void setEvent(Event *e);
};

#endif
