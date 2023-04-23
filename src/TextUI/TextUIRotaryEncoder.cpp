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

#include "TextUIRotaryEncoder.h"

#include <util/atomic.h>

#define ROTARYENC_CLK    ((byte)0b00000001)
#define ROTARYENC_DIR    ((byte)0b00000010)
#define ROTARYENC_SWITCH ((byte)0b00000100)

#define ROTARYENC_ENC_MASK    (ROTARYENC_CLK | ROTARYENC_DIR)
#define ROTARYENC_SWITCH_MASK (ROTARYENC_SWITCH)

/* Button short and long pressed */
#define ROTARYENC_BUTTON_SHORT 1
#define ROTARYENC_BUTTON_LONG  2

#define ROTARYENC_BUTTON_SHORT_MSEC  10
#define ROTARYENC_BUTTON_LONG_MSEC  300


static TextUIRotaryEncoder *rotEnc = nullptr;

/*
 * The pin change interrupt is fixed to PCINT2.
 * If you want to use different pins you need to change the interrupt service routine vector.
 *
 * Arduino Nano (ATmega328):
 *
 *  Interrupt          Chip Port       Arduino Port
 *  -----------------  --------------  -------------------------
 *  PCINT0 PCINT0-7    (Port B0 - B7)  (PCINT0-5:   Pin D8 - D13)
 *  PCINT1 PCINT8-14   (Port C0 - C6)  (PCINT8-13:  Pin A0 - A5)
 *  PCINT2 PCINT16-23  (Port D0 - D7)  (PCINT16-23: Pin D0 - D7)
 *
 * Arduino Mega 2560:
 * 
 *  Interrupt          Chip Port       Arduino Port
 *  -----------------  --------------  -------------------------
 *  PCINT0 PCINT0-7    (Port B0 - B7)  (PCINT4-7:   Pin D10 - D13)
 *  PCINT1 PCINT8      (Port E0)       (PCINT8:     Pin D0)
 *  PCINT1 PCINT9-15   (Port J0 - J6)  (PCINT9-10:  Pin D15, D14)
 *  PCINT2 PCINT16-23  (Port K0 - K7)  (PCINT16-23: Pin A8 -A15)
 */
ISR( PCINT2_vect)
{
    if( rotEnc != nullptr) {
    	rotEnc->runISR();
    }
}

TextUIRotaryEncoder::TextUIRotaryEncoder( uint8_t pinClock, uint8_t pinDir, uint8_t pinButton)
{
    uint8_t pcintMask;
    
    this->pinClock = pinClock;
    this->pinDir = pinDir;
    this->pinButton = pinButton;
    
    pinMode( pinClock, INPUT);
    pinMode( pinDir, INPUT);
    pinMode( pinButton, INPUT);

    /* Enable pull-up */
    digitalWrite( pinClock, HIGH);
    digitalWrite( pinDir, HIGH);
    digitalWrite( pinButton, HIGH);
    
    oldVal  = digitalRead( pinClock) ? ROTARYENC_CLK    : 0;
    oldVal |= digitalRead( pinDir) ? ROTARYENC_DIR    : 0;
    oldVal |= digitalRead( pinButton) ? ROTARYENC_SWITCH : 0;

    enc = button = buttonDown_msec = 0;
    buttonIsDown = false;

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE ) {
      
        pcintMask = bit( digitalPinToPCMSKbit( pinClock))
		              | bit( digitalPinToPCMSKbit( pinDir))
                  | bit( digitalPinToPCMSKbit( pinButton));
    
        PCMSK2 |= pcintMask;
        PCICR |= bit(PCIE2);

        rotEnc = this;
    }
}

void TextUIRotaryEncoder::runISR() {

    byte butVal;
    byte encVal;
    unsigned long now = millis();

    encVal =  digitalRead( pinClock) ? ROTARYENC_CLK : 0;
    encVal |= digitalRead( pinDir) ? ROTARYENC_DIR : 0;
    butVal = digitalRead( pinButton) ? ROTARYENC_SWITCH : 0;

    if( (oldVal & ROTARYENC_SWITCH) && !butVal) { /* Button down */
        buttonDown_msec = now;
        buttonIsDown = true;
        
        oldVal &= ~ROTARYENC_SWITCH_MASK;
        oldVal |= butVal;
        
    } else if ( !(oldVal & ROTARYENC_SWITCH) && butVal) { /* Button up */
        unsigned int down_msec;
        buttonIsDown = false;

        down_msec = (now >= buttonDown_msec)
                  ? now - buttonDown_msec
                  : UINT16_MAX - buttonDown_msec + now;
           
        if( down_msec >= ROTARYENC_BUTTON_LONG_MSEC) {
            button = ROTARYENC_BUTTON_LONG;

        } else if( down_msec >= ROTARYENC_BUTTON_SHORT_MSEC) {
            button = ROTARYENC_BUTTON_SHORT;
        }

        oldVal &= ~ROTARYENC_SWITCH_MASK;
        oldVal |= butVal;
    }

    if( (oldVal & ROTARYENC_ENC_MASK) != encVal) {
        if( ((oldVal & ROTARYENC_ENC_MASK) ^ encVal) == ROTARYENC_CLK) {
            if( encVal == 0 || encVal == 3) enc--; else enc++;
        }

        oldVal &= ~ROTARYENC_ENC_MASK;
        oldVal |= encVal;
    }
}

bool TextUIRotaryEncoder::pending() {

    return (button != 0) || (enc >= 2) || (enc <= -2);
}

void TextUIRotaryEncoder::setEvent(Event *e) {

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE ) {

        if( button == ROTARYENC_BUTTON_SHORT) {
            e->setKeyEvent( KEY_ENTER, 1);
            button = 0;
        } else if( button == ROTARYENC_BUTTON_LONG) {
            e->setKeyEvent( KEY_CLEAR, 1);
            button = 0;
        } else if( enc >= 2) {
            e->setKeyEvent( KEY_DOWN, enc >> 1);
            enc &= 1;
        } else if( enc <= -2) {
            enc = -enc;
            e->setKeyEvent( KEY_UP, enc >> 1);
            enc &= 1;
            enc = -enc;
        } else {
            e->setNoEvent();
        }
    }
}
