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

/*
    Interface code to the actual implementation of PPM generation.
 */

#include "Output.h"
#include "Bind.h"
#include "OutputImpl.h"

extern OutputImpl *outputImpl;

Output::Output() = default;

/* Returns true if the PPM generator is ready to accept the next channel set */
bool Output::acceptChannels() const {

  return outputImpl->acceptChannels();
}

void Output::setChannels( Controls &controls) const {

    for( channel_t ch = 0; ch<PPM_CHANNELS; ch++) {
        outputImpl->SetChannelValue( ch, controls.outputGet(ch));
    }
}

bool Output::isBindSupported() const {

    return outputImpl->isBindSupported();
};

bool Output::isRangeTestSupported() const {

    return outputImpl->isRangeTestSupported();
}

uint8_t Output::getBindModeCount() const {

    return outputImpl->getBindModeCount();
}

bindmode_t* Output::getBindModes() const {

    return outputImpl->getBindModes();
}

void Output::bindActivate( bindmode_t bindMode) {

    outputImpl->bindActivate( bindMode);
}

void Output::bindDeactivate() {

    outputImpl->bindDeactivate();
}

void Output::rangeTestActivate() {

    outputImpl->rangeTestActivate();
}

void Output::rangeTestDeactivate() {

    outputImpl->rangeTestDeactivate();
}

void Output::setModelID( uint8_t mID) {

    outputImpl->setModelID( mID);
}

uint8_t Output::getModelID() {

    return outputImpl->getModelID();
}

void Output::setBindMode( bindmode_t bm) {

    outputImpl->setBindMode( bm);
}

bindmode_t Output::getBindMode() {

    return outputImpl->getBindMode();
}

void Output::HFoff() {

    outputImpl->HFoff();
}

void Output::HFon() {

    outputImpl->HFon();
}

/* Statistics */

uint16_t Output::getOverrunCounter() {

    return outputImpl->getOverrunCounter();
}

timingUsec_t Output::getMaxFrameTime() {

    return outputImpl->getMaxFrameTime();
}
