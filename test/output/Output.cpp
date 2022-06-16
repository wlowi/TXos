
#include "Output.h"
#include "OutputImpl.h"

extern OutputImpl *outputImpl;

Output::Output() {

}

void Output::init() {

}

bool Output::acceptChannels() {
  
  return outputImpl->acceptChannels();  
}

void Output::setChannels( channelSet_t &channels) {

    for( int ch=0; ch<CHANNELS; ch++) {
        outputImpl->SetChannelValue( ch, channels.analogChannel[ch]);
    }
}
