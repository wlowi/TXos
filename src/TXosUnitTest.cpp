/*
    TXos. A remote control transmitter OS.

    Copyright (C) 2022 Wolfgang Lohwasser

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

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

long millis() {

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