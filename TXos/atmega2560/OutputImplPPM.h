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

#ifndef _OutputImplPPM_h_
#define _OutputImplPPM_h_

#include "TXos.h"

#if HF_MODULE == HF_SPEKTRUM_PPM || HF_MODULE == HF_JETI_TU2

#include "OutputImpl.h"

#define PPM_PORT            5

/*
 *      0  400     1000  1500  2000             22000
 *  ____     _______             _________ ... ____
 *      |   |       |     |     |
 *      |___|       |     |     |
 *
 *      |   |
 *    PPM_SPACE
 *
 *      |     PPM_MID     |
 *
 *
 *  100%  ==   1.1  -  1.9 mSec
 *  125%  ==   1.0  -  2.0 mSec
 *  150%  ==   0.9  -  2.1 mSec
 *
 *  9 ch * 2.1 mSec = 18.9 mSec    22.0 - 18.9 = 3.1 mSec
 *
 */


#define PPM_FRAME_usec      ((timingUsec_t) PPM_FRAME_TIME_usec)
#define PPM_SPACE_usec      ((timingUsec_t)  400)
#define PPM_MID_usec        ((timingUsec_t) 1500)

/* 100 %  1500 +/- 400 = [1100,1900] uSec */
#define PPM_RANGE100_usec   ((timingUsec_t)  400)
/* 150 %  1500 +/- 600 = [900,2100] uSec*/
#define PPM_RANGEMAX_usec   ((timingUsec_t)  600)

#define PPM_MIN_usec        (PPM_MID_usec - PPM_RANGEMAX_usec)
#define PPM_MAX_usec        (PPM_MID_usec + PPM_RANGEMAX_usec)

/* Initial high for sync */
#define PPM_INIT_usec       ((timingUsec_t) 8000)

#define OTHER_PPMSET( s)    (((s) +1) % 2)

typedef struct ppmSet_t {

    timingUsec_t channel[ PPM_CHANNELS ];

} ppmSet_t;

class OutputImplPPM : public OutputImpl
{
    public:
        ppmSet_t ppmSet[2];
        uint8_t currentSet;

    private:
        /* Count how may times the user mode code was unable to compute
         * and set all channels within one PPM frame.
         * This should always be 0.
         */
        uint16_t ppmOverrun;
        bool channelSetDone;
        uint8_t modelID;

    public:
        OutputImplPPM();

        void switchSet();

        void isrDisable() {}
        void isrEnable() {}

        bool acceptChannels();
        void SetChannelValue( int channel, int value);

        bool isBindSupported() const;
        bool isRangeTestSupported() const;

        uint8_t getBindModeCount() const;
        bindmode_t* getBindModes() const;

        void bindActivate( bindmode_t bm);
        void bindDeactivate();

        void rangeTestActivate();
        void rangeTestDeactivate();

        void setModelID( uint8_t mID);
        uint8_t getModelID();

        void setBindMode( bindmode_t bm);
        bindmode_t getBindMode();

        void HFoff();
        void HFon();

        timingUsec_t getInFrameTime();
        timingUsec_t getMaxFrameTime();
        uint16_t getOverrunCounter();

    private:
        void init();
};

#endif

#endif
