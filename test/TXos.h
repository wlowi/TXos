/*
 * TXos.h
 */

#ifndef _TXos_h
#define _TXos_h

#include "Arduino.h"
#include "Text.h"

#define TXOS_VERSION "0.1"

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

#define PORT_ANALOG_INPUT_COUNT   6
#define PORT_ANALOG_INPUT        A0,A1,A2,A3,A4,A5

#define PORT_TRIM_INPUT_COUNT     4
#define PORT_TRIM_INPUT          A8,A9,A10,A11

#define PORT_SWITCH_INPUT_COUNT   6
#define PORT_SWITCH_INPUT        22,23,24,25,26,27

#define PORT_HF_RELAIS            2
#define PORT_BIND_RELAIS          3
#define PORT_BUZZER              31

#endif
