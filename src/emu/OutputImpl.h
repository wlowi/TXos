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

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "Controls.h"

class OutputImpl : public wxStaticBoxSizer
{
    private:
        wxWindowID *channelIDs = NULL;
        wxGauge **gauges = NULL;
        wxStaticText **values = NULL;

        long lastFrameMs = 0;

    public:
        OutputImpl( wxWindow *parent, int channels);
        ~OutputImpl( void);

        bool acceptChannels();
        void SetChannelValue( int channel, int value);

        bool isBindSupported() const;
        bool isRangeTestSupported() const;

        void bindActivate();
        void bindDeactivate();

        void rangeTestActivate();
        void rangeTestDeactivate();

        uint16_t getOverrunCounter();
        timingUsec_t getMaxFrameTime();
};

#endif
