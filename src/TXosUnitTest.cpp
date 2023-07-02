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

#include "InputImpl.h"
#include "DisplayImpl.h"
#include "OutputImpl.h"
#include "PortsImpl.h"
#include "BuzzerImpl.h"

#include "EEPROM.h"

#include "time.h"

#include "UtModules.h"

EEPROMClass EEPROM(4096);

InputImpl *inputImpl;
OutputImpl *outputImpl;
DisplayImpl *displayImpl;
PortsImpl *portsImpl;
BuzzerImpl *buzzerImpl;

extern Controls controls;

SWITCH_CONFIGURATION

unsigned long millis() {

    return (long)clock() * 1000 / CLOCKS_PER_SEC;
}

int main() {
    
    portsImpl = new PortsImpl();
    buzzerImpl = new BuzzerImpl();
    displayImpl = new DisplayImpl();
    outputImpl = new OutputImpl( 9);
    inputImpl =  new InputImpl( PORT_ANALOG_INPUT_COUNT, PORT_TRIM_INPUT_COUNT, PORT_AUX_INPUT_COUNT,
                                PORT_SWITCH_INPUT_COUNT, switchConfiguration);

    UnitTest *modules = new UtModules();

    modules->run();

    return 0;
}
