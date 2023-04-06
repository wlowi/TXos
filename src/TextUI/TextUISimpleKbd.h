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

#ifndef _TextUISimpleKbd_h_
#define _TextUISimpleKbd_h_

#include "TextUI.h"

#define BUTTON_LONG_TIMEOUT_msec      250
#define BUTTON_REPEAT_TIMEOUT_msec   1000
#define BUTTON_REPEAT_SPEED_msec       50

class TextUISimpleKbd : public TextUIInput {

  private:
    uint8_t buttonCount;
    uint8_t *buttonPort;
    uint8_t *shortKey;
    uint8_t *longKey;

    /* A per key bitmap
     *  bit 0    => button pressed
     *  bit 1    => short
     *  bit 2    => long
     */
    uint8_t *buttonState;

    /* There should be a pressed timestamp for every key.
     * We currently do not support multiple keys pressed at the same time. 
     */
    long pressedTimeStamp = 0;
    long repeatTimeStamp = 0;
    
  public:
    TextUISimpleKbd( uint8_t count, uint8_t ports[], uint8_t skey[], uint8_t lkey[]);

    /* TextUIInput */
    bool pending();
    void setEvent( Event *e);
};

#endif
