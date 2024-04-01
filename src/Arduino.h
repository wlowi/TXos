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

/*
 * Arduino.h
 * Dummy defines for simulation.
 */

#ifndef _Arduino_h_
#define _Arduino_h_

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <ctype.h>

#include "EmuSerial.h"

#ifndef NULL
#define NULL __null
#endif

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1

typedef uint8_t byte;

typedef bool boolean;

#define delay( s)

extern unsigned long millis();
extern EmuSerial Serial;

/* From AVR atomic.h */

#define ATOMIC_BLOCK( s )
#define ATOMIC_RESTORESTATE
#define ATOMIC_FORCEON

#define __FlashStringHelper char
#define F( b ) b

#define PROGMEM

#define pgm_read_byte( b ) (*(b))
#define pgm_read_word( b ) (*(b))
#define pgm_read_dword( b ) (*(b))
#define pgm_read_ptr_far( b ) (*(b))

#endif
