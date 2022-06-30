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

/* This identifies a channel number.
 */
typedef uint8_t channel_t;

/* The internal representation of a channel value is a signed integer.
 * 0 represents mid position. The range is [-1000,1000]
 */
typedef int16_t channelValue_t;

/* A bitmap with boolean values for 16 channels.
 */
typedef uint16_t channelBits_t;

typedef uint16_t timingUsec_t;

#define BIT_SET( bm, ch)     bm |= ((channelBits_t)1 << ch)
#define BIT_CLEAR( bm, ch)   bm &= ~((channelBits_t)1 << ch)
#define IS_BIT_SET( bm, ch)  (bm & ((channelBits_t)1 << ch))

#define CHANNELVALUE_MID        ((channelValue_t)    0)
#define CHANNELVALUE_MIN        ((channelValue_t)-1250)
#define CHANNELVALUE_MAX        ((channelValue_t) 1250)

#define CHANNEL_THROTTLE        ((channel_t)0)
#define CHANNEL_AILERON         ((channel_t)1)
#define CHANNEL_ELEVATOR        ((channel_t)2)
#define CHANNEL_RUDDER          ((channel_t)3)
#define CHANNEL_AILERON2        ((channel_t)4)

#define H( v ) ((uint8_t)(v >> 8))
#define L( v ) ((uint8_t)(v & 0xff))

/* This represents a percentage from -125% to 125%
 */
typedef int8_t percent_t;
#define PERCENT_MAX ((percent_t)125)
#define PERCENT_MIN ((percent_t)-125)

#define PCT_TO_CHANNEL( p) ((channelValue_t)p*10)

#define CHAMMEL_TO_PCT( c) ((percent_t)(c/10))

/* Numeric switch identifier
 */
typedef uint8_t switch_t;

/* Type representing the state of 8 switches.
 * Two bits per switch allow for 4 states: (switchValue_t)
 *
 *  0 State 0
 *  1 State 1
 *  2 State 2
 *  3 Invalid position or "Dont care" for comparison
 */
typedef uint16_t switchSet_t;
typedef uint8_t switchValue_t;

/* SWITCH_SET( switchSet_t set, switch_t switchNo, switchState) */
#define SWITCH_SET( s, n, v)    ((s) |= ( ((v) & 0x03) << ((n) << 1)))
/* SWITCH_GET( switchSet_t set, switch_t switchNo) */
#define SWITCH_GET( s, n)       (((s) >> ((n) << 1)) & 0x03)

#define CHANNELS                ((channel_t)9)

extern const char *ChannelNames[CHANNELS];

/* Timings for all channels */
typedef struct channelSet_t {
    
    channelValue_t analogChannel[ CHANNELS ];
    switchSet_t switches;

} channelSet_t;


#define MODEL_NAME_LEN          ((uint8_t)8)
#define MODEL_NAME_DEFAULT      CC("--------")

#endif
