/*
 * TXos.h
 */

#ifndef _TXos_h
#define _TXos_h

#include "Arduino.h"


/* This identifies a channel number.
 */
typedef uint8_t channel_t;

/* The internal representation of a channel value is a signed integer.
 * 0 represents mid position. The range is [-1000,1000]
 */
typedef int16_t value_t;

#define CHANNELVALUE_MID        ((value_t)    0)
#define CHANNELVALUE_MIN        ((value_t)-1000)
#define CHANNELVALUE_MAX        ((value_t) 1000)

#define CHANNEL_GAS             ((channel_t)0)
#define CHANNEL_AILERON         ((channel_t)1)
#define CHANNEL_ELEVATOR        ((channel_t)2)
#define CHANNEL_RUDDER          ((channel_t)3)

/* This represents a percentage from -100% to 100%
 * We don't need 16 bits for that.
 */
typedef int8_t percent_t;

/* Type representing the state of switches.
 */
typedef uint16_t digital_t;

#endif
