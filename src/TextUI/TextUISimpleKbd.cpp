/*
  TextUI. A simple text based UI.

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

#include "TextUISimpleKbd.h"

#define STATE_BIT_PRESSED       0
#define STATE_BIT_SHORT         1
#define STATE_BIT_LONG          2
#define STATE_BIT_REPEAT        3

#define BTN_PRESSED( s )        (s == LOW)
#define BTN_RELEASED( s )       (s == HIGH)

#define BTN_CLEAR_STATE( b)     buttonState[b] = 0

#define BTN_WAS_PRESSED( b )    (buttonState[b] & 1)
#define BTN_SHORT_PRESSED( b )  (buttonState[b] & 2)
#define BTN_LONG_PRESSED( b )   (buttonState[b] & 4)
#define BTN_SHORT_REPEAT( b )   (buttonState[b] & 8)

TextUISimpleKbd::TextUISimpleKbd( uint8_t count, uint8_t ports[], uint8_t skey[], uint8_t lkey[]) {

  buttonCount = count;
  
  buttonPort = new uint8_t[count];
  memcpy( buttonPort, ports, count);

  shortKey = new uint8_t[count];
  memcpy( shortKey, skey, count);
  longKey = new uint8_t[count];
  memcpy( longKey, lkey, count);

/* bitmap
 *  bit 0    => button pressed
 *  bit 1    => short
 *  bit 2    => long
 */
  buttonState = new uint8_t[count];

  for( uint8_t b = 0; b < buttonCount; b++) {
    pinMode( buttonPort[b], INPUT_PULLUP);
    BTN_CLEAR_STATE( b);
  }
}

bool TextUISimpleKbd::pending() {

  bool pend = false;
  bool state;

  long now = millis();
  
  for( uint8_t b = 0; b < buttonCount; b++) {
    state = digitalRead( buttonPort[b]);
    
    if( BTN_PRESSED( state) && buttonState[b] == 0) {
      
      bitSet( buttonState[b], STATE_BIT_PRESSED);
      pressedTimeStamp = repeatTimeStamp = now;
      
    } else if( BTN_PRESSED( state) && BTN_WAS_PRESSED( b)) {

      if( now > pressedTimeStamp + BUTTON_REPEAT_TIMEOUT_msec
          && now > repeatTimeStamp + BUTTON_REPEAT_SPEED_msec)
      {
        repeatTimeStamp = now;
        bitSet( buttonState[b], STATE_BIT_REPEAT);
        pend = true;
      }
       
    } else if( BTN_RELEASED( state) && BTN_WAS_PRESSED( b)) {

      if( BTN_SHORT_REPEAT( b)) {
         BTN_CLEAR_STATE( b);
         
      } else {
        bitSet( buttonState[b],
              (now > pressedTimeStamp + BUTTON_LONG_TIMEOUT_msec) 
              ? STATE_BIT_LONG
              : STATE_BIT_SHORT);
      
        pend = true;
      }
    }
  }
  
  return pend;
}

void TextUISimpleKbd::setEvent( Event *e) {
  
  for( uint8_t b = 0; b < buttonCount; b++) {
    
    if( BTN_SHORT_PRESSED( b)) {
      e->setKeyEvent( shortKey[b], 1);
      BTN_CLEAR_STATE( b);
      break;
      
    } else if( BTN_LONG_PRESSED( b)) {
      e->setKeyEvent( longKey[b], 1);
      BTN_CLEAR_STATE( b);
      break;
      
    } else if( BTN_SHORT_REPEAT( b)) {
      e->setKeyEvent( shortKey[b], 1);
      break;
    }
  }
}
