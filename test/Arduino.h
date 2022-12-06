/*
 * Arduino.h
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

/* From AVR atomic.h */

#define ATOMIC_BLOCK( s )
#define ATOMIC_RESTORESTATE

#endif
