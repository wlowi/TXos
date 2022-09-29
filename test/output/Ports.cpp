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
