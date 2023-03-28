/*
  TXos. A remote control transmitter OS.

  MIT License

  Copyright (c) 2023 wlowi

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

/*
    Interface code to the actual implementation of input controls.
 */

#ifndef _Controls_h_
#define _Controls_h_

#include "TXos.h"

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

#define TRIMVALUE_MID           ((channelValue_t)   0)
#define TRIMVALUE_MIN_LIMIT     ((channelValue_t)-250)
#define TRIMVALUE_MAX_LIMIT     ((channelValue_t) 250)

/* Logical channels
 * first 4 are trimable channels
 * first 9 are mixable channels
 * 12 channels in total
*/
#define CHANNEL_THROTTLE        ((channel_t)0)
#define CHANNEL_AILERON         ((channel_t)1)
#define CHANNEL_ELEVATOR        ((channel_t)2)
#define CHANNEL_RUDDER          ((channel_t)3)

#define CHANNEL_FLAP            ((channel_t)4)
#define CHANNEL_SPOILER         ((channel_t)5)
#define CHANNEL_GEAR            ((channel_t)6)
#define CHANNEL_8               ((channel_t)7)
#define CHANNEL_9               ((channel_t)8)

#define CHANNEL_AILERON2        ((channel_t)9)
#define CHANNEL_FLAP2           ((channel_t)10)
#define CHANNEL_SPOILER2        ((channel_t)11)

#define H( v ) ((uint8_t)((v) >> 8))
#define L( v ) ((uint8_t)((v) & 0xff))

/* This represents a percentage from -125% to 125%
 */
typedef int8_t percent_t;
#define PERCENT_MIN             ((percent_t)-100)
#define PERCENT_MAX             ((percent_t)100)

#define PERCENT_MIN_LIMIT       ((percent_t)-125)
#define PERCENT_MAX_LIMIT       ((percent_t)125)

#define PCT_TO_CHANNEL( p) ((channelValue_t)p*10)

#define CHANNEL_TO_PCT( c) ((percent_t)(c/10))


/************* SWITCHES ***********/

/* State of a single switch
 * Bit 7     1 == invalid / unused switch
 * Bit 4-6   switch State
 * Bit 0-3   switch number 
 */
typedef uint8_t switch_t;

#define SWITCH_UNUSED_FLAG              ((switch_t)0x80)
#define SWITCH_NUMBER_MASK              ((switch_t)0x0f)
#define SWITCH_STATE_MASK               ((switch_t)0x70)

#define INIT_SWITCH( sw)                sw = SWITCH_UNUSED_FLAG
#define SET_SWITCH_UNUSED( sw)          sw |= SWITCH_UNUSED_FLAG    
#define SET_SWITCH_USED( sw)            sw &= ~SWITCH_UNUSED_FLAG
#define IS_SWITCH_UNUSED( sw)           (sw & SWITCH_UNUSED_FLAG)
#define IS_SWITCH_USED( sw)             (!IS_SWITCH_UNUSED( sw))

#define SET_SWITCH( sw, num)            sw = (((sw) & ~SWITCH_NUMBER_MASK) | ((num) & SWITCH_NUMBER_MASK))
#define GET_SWITCH( sw)                 (sw & SWITCH_NUMBER_MASK)

#define SET_SWITCH_STATE( sw, state)    sw = (((sw) & ~SWITCH_STATE_MASK) | (((state) << 4) & SWITCH_STATE_MASK))
#define GET_SWITCH_STATE( sw)           ((switchState_t)(((sw) & SWITCH_STATE_MASK) >> 4))


/* State of all controls.
 * Controls are sticks, switches and logical switches.
 */
typedef struct controlSet_t {
    
    /* Raw analog input channels */
    channelValue_t stickADCChannel[ PORT_ANALOG_INPUT_COUNT ];
    percent_t range_pct[PORT_ANALOG_INPUT_COUNT];

    channelValue_t trimADCChannel[ PORT_TRIM_INPUT_COUNT ];
    channelValue_t auxADCChannel[ PORT_AUX_INPUT_COUNT ];

    /* Calibrated analog input channels */
    channelValue_t inputChannel[ INPUT_CHANNELS ];
    channelValue_t trimChannel[ PORT_TRIM_INPUT_COUNT ];

    /* Mixed channels */
    channelValue_t logicalChannel[ LOGICAL_CHANNELS ];

    /* Calibrated and mixed analog channels */
    channelValue_t outChannel[ PPM_CHANNELS ];

    switch_t switches[SWITCHES];

} controlSet_t;

class Controls {

    private:
        controlSet_t controlSet;

    public:
        Controls();

        void init();

        /* Retrieve all analog (sticks) and digital (switches) control values
         * from input implementation and update 'channels' structure.
         */
        void GetControlValues();

        channelValue_t stickADCGet( channel_t ch);
        channelValue_t trimADCGet( channel_t ch);
        channelValue_t auxADCGet( channel_t ch);

        void rangeSet( channel_t ch, percent_t value);
        percent_t rangeGet( channel_t ch);

        void inputSet( channel_t ch, channelValue_t value);
        channelValue_t inputGet( channel_t ch);

        void trimSet( channel_t ch, channelValue_t value);
        channelValue_t trimGet( channel_t ch);

        void logicalSet( channel_t ch, channelValue_t value);
        channelValue_t logicalGet( channel_t ch);

        void outputSet( channel_t ch, channelValue_t value);
        channelValue_t outputGet( channel_t ch);

        void switchSet( switch_t sw, switchState_t value);
        switchState_t switchGet( switch_t sw);
        switchConf_t switchConfGet( switch_t sw);

        /* Return the switch number of a switch with a particular type.
         * There may be more than one switch per type.
         * Use idx to identiy the one you need.
         */
        switch_t getSwitchByType( switchConf_t type, uint8_t idx);

        void copySwitchName( char *b, switch_t sw);
        void copySwitchNameAndState( char *b, switch_t sw);

        bool evalSwitches( switch_t trigger);
};

#endif
