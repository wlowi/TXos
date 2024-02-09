/*
  Lights. A remote control lights controller.

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

#ifndef __Stream_h__
#define __Stream_h__

#include "stddef.h"

/* A stream class to make Arduino code work in Designer.
 * This is used by Lights/Comm class.
 *
 * It takes a Usb class and reads/writes from there.
 */
class Stream {

    public:
        virtual ~Stream() = 0;

        virtual void setTimeout(unsigned long timeout) = 0;

        virtual size_t write( const char* text) = 0;
        virtual void flush() = 0;

        virtual int read() = 0;

        virtual int available() = 0;

        virtual void close() = 0;
};

inline Stream::~Stream() {}

#endif