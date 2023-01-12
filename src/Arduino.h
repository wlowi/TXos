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

/*
 * Arduino.h
 * Dummy defines for simulation.
 */

#ifndef _Arduino_h_
#define _Arduino_h_

#include <stdint.h>
#include <stdlib.h>

#ifndef NULL
#define NULL __null
#endif

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1

typedef uint8_t byte;

#define delay( s)

extern long millis();

/* From AVR atomic.h */

#define ATOMIC_BLOCK( s )
#define ATOMIC_RESTORESTATE

#endif
