/*
 * TXos.h
 */

#ifndef _TXos_h
#define _TXos_h

#include "Arduino.h"

#if defined( ARDUINO )
# include <util/atomic.h>
#endif


#define TXOS_VERSION "0.1.16"

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

/* Phase number
 */
typedef uint8_t phase_t;

/*  0 State 0
 *  1 State 1
 *  2 State 2
 *  3 Invalid position or "Dont care" for comparison
 */
typedef enum {

    SW_STATE_0 = 0,
    SW_STATE_1 = 1,
    SW_STATE_2 = 2,
    SW_STATE_DONTCARE = 3

} switchState_t;

/*  0 Switch is unused
 *  1 Switch is 2-state
 *  2 Switch is 3-state
 *  3 Switch is channel switch
 */
typedef enum {

    SW_CONF_UNUSED = 0,
    SW_CONF_2STATE = 1,
    SW_CONF_3STATE = 2,
    SW_CONF_CHANNEL = 3

} switchConf_t;

#include "TXosConfig.h"

#if UI_LANGUAGE == DE
  #include "Text_DE.h"
#elif UI_LANGUAGE == EN
  #include "Text_EN.h"
#else
  #error "Set UI_LANGUAGE in TXosConfig.h to a supported value."
#endif

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
