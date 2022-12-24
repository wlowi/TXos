
#include "OutputImpl.h"

OutputImpl::OutputImpl( int channels)
{

}

bool OutputImpl::acceptChannels() {

    long now = millis();

    if( now >= lastFrameMs + 22) {
        lastFrameMs = now;
        return true;
    } 

    return false;
}

void OutputImpl::SetChannelValue(int channel, int value) {

}

uint16_t OutputImpl::getOverrunCounter() {

    return 0;
}

timingUsec_t OutputImpl::getMaxFrameTime() {

    return 0;
}