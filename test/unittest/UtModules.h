
#ifndef _UtModules_
#define _UtModules_

#include "UnitTest.h"

class UtModules : public UnitTest {

    private:

    public:
        void run();

        void verify( channel_t start, uint8_t count, channelValue_t in, channelValue_t expected);
        void dumpControls( Controls &controls);
};

#endif