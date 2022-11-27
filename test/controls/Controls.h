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
    Interface code to the actual implementation of input controls.
 */

#ifndef _Controls_h_
#define _Controls_h_

#include "TXos.h"

/* The number of transmitter channels.
 * This is the number of channels that will be used to when
 * generating the PPM signal.
 */
#define CHANNELS                ((channel_t)9)

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

#define CHANNELVALUE_MIN        ((channelValue_t)-1000)
#define CHANNELVALUE_MAX        ((channelValue_t) 1000)

#define CHANNELVALUE_MIN_LIMIT  ((channelValue_t)-1250)
#define CHANNELVALUE_MAX_LIMIT  ((channelValue_t) 1250)

#define CHANNEL_THROTTLE        ((channel_t)0)
#define CHANNEL_AILERON         ((channel_t)1)
#define CHANNEL_ELEVATOR        ((channel_t)2)
#define CHANNEL_RUDDER          ((channel_t)3)
#define CHANNEL_AILERON2        ((channel_t)4)

#define TRIMVALUE_MID           ((channelValue_t)   0)
#define TRIMVALUE_MIN_LIMIT     ((channelValue_t)-250)
#define TRIMVALUE_MAX_LIMIT     ((channelValue_t) 250)

#define H( v ) ((uint8_t)(v >> 8))
#define L( v ) ((uint8_t)(v & 0xff))

/* This represents a percentage from -125% to 125%
 */
typedef int8_t percent_t;
#define PERCENT_MAX_LIMIT       ((percent_t)125)
#define PERCENT_MIN_LIMIT       ((percent_t)-125)

#define PCT_TO_CHANNEL( p) ((channelValue_t)p*10)

#define CHANNEL_TO_PCT( c) ((percent_t)(c/10))


/* Numeric switch identifier
 */
typedef uint8_t switch_t;

#define SWITCHES        ((switch_t)8)


/* Type representing the state of 8 switches.
 * Two bits per switch allow for 4 states: (switchState_t)
 */
typedef uint16_t switchSet_t;

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

#define SW_STATE_ALL_DONTCARE  ((switchSet_t)0xffff)


/* Switch configuration
 * Two bits per switch
 */
typedef uint16_t switchSetConf_t;

/*  0 Switch is unused
 *  1 Switch is 2-state
 *  2 Switch is 3-state
 *  3 Switch is control switch
 */
typedef enum {

    SW_CONF_UNUSED = 0,
    SW_CONF_2STATE = 1,
    SW_CONF_3STATE = 2,
    SW_CONF_CONTROL = 3

} switchConf_t;


/* Switch configuration
 * In this case we have:
 *   4 tri state switches
 *   2 bi state switches
 *   2 control switches
 */
#define SWITCH_CONFIGURATION        \
    ((switchConf_t)                 \
    /* 0 */ SW_CONF_3STATE        | \
    /* 1 */ SW_CONF_3STATE  <<  2 | \
    /* 2 */ SW_CONF_3STATE  <<  4 | \
    /* 3 */ SW_CONF_3STATE  <<  6 | \
    /* 4 */ SW_CONF_2STATE  <<  8 | \
    /* 5 */ SW_CONF_2STATE  << 10 | \
    /* 6 */ SW_CONF_CONTROL << 12 | \
    /* 7 */ SW_CONF_CONTROL << 14   \
     )

#define CONTROLS_SWITCH_CONF_GET( conf, sw)  (switchConf_t)((conf >> (sw << 1)) & 0x03)

#define CONTROLS_SWITCH_GET( switches, sw)  (switchState_t)((switches >> (sw << 1)) & 0x03)

extern const char *ChannelNames[CHANNELS];

/* State of all controls.
 * Controls are sticks, switches and logical switches.
 */
typedef struct controlSet_t {
    
    /* Raw analog input channels */
    channelValue_t stickChannel[ PORT_ANALOG_INPUT_COUNT ];
    channelValue_t trimChannel[ PORT_TRIM_INPUT_COUNT ];
    channelValue_t auxChannel[ PORT_AUX_INPUT_COUNT ];

    /* Calibrated and mixed analog channels */
    channelValue_t analogChannel[ CHANNELS ];

    switchSet_t switches;
    switchSetConf_t switchSetConf;

} controlSet_t;

class Controls {

    private:
        controlSet_t controlSet;

    public:
        Controls();

        void init( switchSetConf_t conf);

        /* Retrieve all analog (sticks) and digital (switches) control values
         * from input implementation and update 'channels' structure.
         */
        void GetControlValues();

        channelValue_t stickGet( channel_t ch);
        channelValue_t trimGet( channel_t ch);
        channelValue_t auxGet( channel_t ch);

        void analogSet( channel_t ch, channelValue_t value);
        channelValue_t analogGet( channel_t ch);

        void switchSet( switch_t sw, switchState_t value);
        switchState_t switchGet( switch_t sw);

        void switchConfSet( switch_t sw, switchConf_t conf);
        switchConf_t switchConfGet( switch_t sw);

        bool evalSwitches( switchSet_t trigger);
};

#endif
