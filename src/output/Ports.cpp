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

#include "Ports.h"
#include "PortsImpl.h"

/*
 * Call system specific implementations
 * to control IO ports.
 */

extern PortsImpl *portsImpl;

void Ports::init() const {

    portsImpl->portInit( PORT_BIND_RELAIS, OUTPUT);
    portsImpl->portInit( PORT_HF_RELAIS, OUTPUT);
    portsImpl->portInit( PORT_BUZZER, OUTPUT);

    buzzerOff();
    bindOff();
    hfOn();
}

void Ports::hfOn() const {

    /* HF is on when relais is off */
    portsImpl->portSet( PORT_HF_RELAIS, LOW);
}

void Ports::hfOff() const {

    portsImpl->portSet( PORT_HF_RELAIS, HIGH);
}

void Ports::bindOn() const {

    portsImpl->portSet( PORT_BIND_RELAIS, HIGH);
}

void Ports::bindOff() const {

    portsImpl->portSet( PORT_BIND_RELAIS, LOW);
}

void Ports::buzzerOn() const {

    portsImpl->portSet( PORT_BUZZER, HIGH);
}

void Ports::buzzerOff() const {

    portsImpl->portSet( PORT_BUZZER, LOW);
}
