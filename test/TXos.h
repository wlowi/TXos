/*
 * TXos.h
 */

#ifndef _TXos_h
#define _TXos_h

#include "Arduino.h"
#include "Text.h"

#define TXOS_VERSION "0.1.4"

#if defined( ARDUINO )
    #define LOG( f )
    #define LOGV( f, ... )
#else
    #include "stdio.h"
    #define LOG( f ) printf( f )
    #define LOGV( f, ... ) printf( f, __VA_ARGS__)
#endif

#define MODEL_NAME_LEN          ((uint8_t)8)
#define MODEL_NAME_DEFAULT      CC("--------")

/* Port definitions */

/* Analog sticks and other analog channels */
#define PORT_ANALOG_INPUT_COUNT   6
#define PORT_ANALOG_INPUT        A0,A1,A2,A3,A4,A5

/* Analog trim inputs */
#define PORT_TRIM_INPUT_COUNT     4
#define PORT_TRIM_INPUT          A8,A9,A10,A11

/* Auxiliary analog input: Vcc monitor */
#define PORT_AUX_INPUT_COUNT      1
#define PORT_AUX_INPUT          A15

/* Two state or three state switches */
#define PORT_SWITCH_INPUT_COUNT   6
#define PORT_SWITCH_INPUT        22,23,24,25,26,27

#define PORT_HF_RELAIS            2
#define PORT_BIND_RELAIS          3
#define PORT_BUZZER              31



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
