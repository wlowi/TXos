/*
 * Arduino.h
 *
 * Some Arduino compatibe defines.
 */

#ifndef Arduino_h
#define Arduino_h

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <inttypes.h>
#include <avr/sleep.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "Dispatcher.h"

#undef ENABLE_DEBUG
#define ENABLE_MEMDEBUG

#ifdef ENABLE_DEBUG
    void setDebugValue( unsigned int v);
#endif

#define HIGH_BYTE( v ) (((v) >> 8) & 0xff)
#define LOW_BYTE( v ) ((v) & 0xff)

typedef unsigned int word;
typedef uint8_t byte;

#ifdef __cplusplus
typedef bool boolean;
#else
typedef uint8_t boolean;
#endif

void panic();

inline void * operator new (size_t size) { return malloc (size); }

#endif
