
#include "Output.h"
#include "OutputBox.h"

extern OutputBox *outputBox;

Output::Output() {

}

void Output::setChannels( channelSet_t &channels) {

    for( int ch=0; ch<CHANNELS; ch++) {
        outputBox->SetChannelValue( ch, channels.analogChannel[ch]);
    }
}