
#ifndef _OutputImpl_h_
#define _OutputImpl_h_

#include "Arduino.h"

class OutputImpl
{
    private:

    public:
        OutputImpl( int channels);

        void SetChannelValue( int channel, int value);
};

#endif
