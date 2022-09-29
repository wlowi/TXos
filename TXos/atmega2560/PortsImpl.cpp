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

#include "PortsImpl.h"

/*
 * System specific implementation to control IO ports.
 */

PortsImpl::PortsImpl() = default;

void PortsImpl::portInit( uint8_t p, uint8_t t) const {

  pinMode( p, t);
}

void PortsImpl::portSet( uint8_t p, uint8_t s) const {

  digitalWrite( p, s);  
}

uint8_t PortsImpl::portGet( uint8_t p) const {

  return digitalRead( p);
}
