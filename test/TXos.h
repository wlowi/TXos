/*
 * TXos.h
 */

#ifndef _TXos_h
#define _TXos_h

#include "Arduino.h"
#include "Text.h"

#define TXOS_VERSION "0.1.8"

#if defined( ARDUINO )
    #define LOG( f )
    #define LOGV( f, ... )
#else
    #include "stdio.h"
    #define LOG( f ) printf( f )
    #define LOGV( f, ... ) printf( f, __VA_ARGS__)
#endif

/* Holds small float values with 2 fractional digits.
 * This is currently only used to display battery voltage.
 *
 * Note that you can add and subtract float16 values
 * but you cannot simply multiply or devide float16 values.
 */
typedef int16_t float16;

/* This identifies a channel number.
 */
typedef uint8_t channel_t;

/* Numeric switch identifier.
 */
typedef uint8_t switch_t;

#include "TXosConfig.h"

#define MODEL_NAME_LEN          ((uint8_t)8)
#define MODEL_NAME_DEFAULT      CC("--------")

#define BDEBUG_LEN 40

#define BDEBUG_ADD( c )          \
do {                             \
  if( bdebugi < BDEBUG_LEN-1) {  \
    bdebug[bdebugi++] = c;       \
    bdebug[bdebugi] = '\0';      \
  }                              \
} while( false)

#define BDEBUG_CLEAR()           \
do {                             \
  bdebugi = 0;                   \
  bdebug[0] = '\0';              \
} while( false)

extern char bdebug[];
extern uint8_t bdebugi;

#endif
