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

#ifndef _OutputImpl_h_
#define _OutputImpl_h_

#include "TXos.h"
#include "Controls.h"
#include "Bind.h"
// #include <cstdint>

class OutputImpl
{
    public:
        virtual void isrDisable() = 0;
        virtual void isrEnable() = 0;

        virtual bool acceptChannels() = 0;
        virtual void SetChannelValue( int channel, int value) = 0;

        virtual bool isBindSupported() const = 0;
        virtual bool isRangeTestSupported() const = 0;

        virtual uint8_t getBindModeCount() const = 0;
        virtual bindmode_t* getBindModes() const = 0;

        virtual void bindActivate( bindmode_t bm) = 0;
        virtual void bindDeactivate() = 0;

        virtual void rangeTestActivate() = 0;
        virtual void rangeTestDeactivate() = 0;

        virtual void setModelID( uint8_t mID) = 0;
        virtual uint8_t getModelID() = 0;

        virtual void setBindMode( bindmode_t bm) = 0;
        virtual bindmode_t getBindMode() = 0;

        virtual void HFoff() = 0;
        virtual void HFon() = 0;

        virtual timingUsec_t getMaxFrameTime() = 0;
        virtual uint16_t getOverrunCounter() = 0;
};

#endif
