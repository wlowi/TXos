
#ifndef _OutputImpl_h_
#define _OutputImpl_h_

#include "Controls.h"

class OutputImpl {

        long lastFrameMs = 0;
        
    public:
        OutputImpl( int channels);

        bool acceptChannels();
        void SetChannelValue( int channel, int value);
        uint16_t getOverrunCounter();
        timingUsec_t getMaxFrameTime();
};

#endif