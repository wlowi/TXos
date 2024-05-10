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

#ifndef _TXosConfig_h
#define _TXosConfig_h

/* Supported languages */
#define EN 1
#define DE 2

/* Supported HF modules */
#define HF_SPEKTRUM_PPM 1
#define HF_JETI_TU2     2

#include "TXosLocalConfig.h"

/* Analog input channels */
#define ANALOG_CHANNELS               (ANALOG_STICK_CHANNELS + ANALOG_OTHER_CHANNELS + SWITCHED_CHANNELS)

/* Input Channels:
 * ANALOG_CHANNELS plus one that decouples the channel from input.
 */
#define INPUT_CHANNELS                (ANALOG_CHANNELS +1)

/* Logical channels for mixing.
 */
#define LOGICAL_CHANNELS              ((channel_t)12)

/* Main channels like AIL, ELV, RUD, FLP
 * without their mix counterparts like AIL2, FLP2
 */
#define MIX_CHANNELS                  ((channel_t)9)

/* First 4 channels. Stick channels */
#define MODE_CHANNELS                 ((channel_t)4)

#define MECHANICAL_SWITCHES_FIRST_IDX ((uint8_t)0)

#define CHANNEL_SWITCHES_FIRST_IDX    ((uint8_t)MECHANICAL_SWITCHES)

/* In your mind add one "Always On" logical switch.
 */
#define LOGIC_SWITCHES_FIRST_IDX      (CHANNEL_SWITCHES_FIRST_IDX + CHANNEL_SWITCHES +1)




#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_EMU)

/* Port definitions */

#define PORT_TFT_CS              10
#define PORT_TFT_DC               9
#define PORT_TFT_RST             -1

/* Rotary encoder
 * Must be in range A8 - A15
 */
#define PORT_ROTENC_CLK         A12
#define PORT_ROTENC_DIR         A13
#define PORT_ROTENC_BUTTON      A14

#if (ANALOG_STICK_CHANNELS + ANALOG_OTHER_CHANNELS) == 4
    #define PORT_ANALOG_INPUT_COUNT   4
    #define PORT_ANALOG_INPUT        A0,A1,A2,A3
#elif (ANALOG_STICK_CHANNELS + ANALOG_OTHER_CHANNELS) == 5
    #define PORT_ANALOG_INPUT_COUNT   5
    #define PORT_ANALOG_INPUT        A0,A1,A2,A3,A4
#elif (ANALOG_STICK_CHANNELS + ANALOG_OTHER_CHANNELS) == 6
    #define PORT_ANALOG_INPUT_COUNT   6
    #define PORT_ANALOG_INPUT        A0,A1,A2,A3,A4,A5
#else
    #error "ANALOG CHANNEL CONFIG failed."
#endif

/* Analog trim inputs */
#define PORT_TRIM_INPUT_COUNT     4
#define PORT_TRIM_INPUT          A8,A9,A10,A11

/* Auxiliary analog input: Vcc monitor */
#define PORT_AUX_INPUT_COUNT      1
#define PORT_AUX_INPUT          A15


/* Two state or three state switches */
#if MECHANICAL_SWITCHES == 0
    #define PORT_SWITCH_INPUT_COUNT   0
    #define PORT_SWITCH_INPUT        
#elif MECHANICAL_SWITCHES == 1
    #define PORT_SWITCH_INPUT_COUNT   1
    #define PORT_SWITCH_INPUT        22
#elif MECHANICAL_SWITCHES == 2
    #define PORT_SWITCH_INPUT_COUNT   2
    #define PORT_SWITCH_INPUT        22,23
#elif MECHANICAL_SWITCHES == 3
    #define PORT_SWITCH_INPUT_COUNT   3
    #define PORT_SWITCH_INPUT        22,23,24
#elif MECHANICAL_SWITCHES == 4
    #define PORT_SWITCH_INPUT_COUNT   4
    #define PORT_SWITCH_INPUT        22,23,24,25
#elif MECHANICAL_SWITCHES == 5
    #define PORT_SWITCH_INPUT_COUNT   5
    #define PORT_SWITCH_INPUT        22,23,24,25,26
#elif MECHANICAL_SWITCHES == 6
    #define PORT_SWITCH_INPUT_COUNT   6
    #define PORT_SWITCH_INPUT        22,23,24,25,26,27
#else
    #error "SWITCH CONFIG failed."
#endif


#define PORT_HF_RELAIS            2
#define PORT_BIND_RELAIS          3
#define PORT_BUZZER              31


#if HF_MODULE == HF_SPEKTRUM_PPM
    #undef PPM_CHANNELS
    #define PPM_CHANNELS                  ((channel_t)9)

#elif HF_MODULE == HF_JETI_TU2
    #undef ENABLE_BIND_MODULE
    #undef ENABLE_RANGETEST_MODULE

#else
  #error "Set HF_MODULE in TXosLocalConfig.h to a supported value."
#endif

/* Battery monitor */

/* Voltage mutliplied by 100 to maintain 2 fractional digits.
 * 500 = 5.00V
 */
#define ADC_VOLTAGE               500
/* 10 bits => 1024 steps */
#define ADC_VCC_RESOLUTION       1024




#elif defined(ARDUINO_ARCH_ESP32)

/* Port definitions */

#define PORT_TFT_CS               5
#define PORT_TFT_DC               2
#define PORT_TFT_RST             -1

/* Rotary encoder
 * Must be in range A8 - A15
 */
#define PORT_ROTENC_CLK          22
#define PORT_ROTENC_DIR          21
#define PORT_ROTENC_BUTTON       19

#if (ANALOG_STICK_CHANNELS + ANALOG_OTHER_CHANNELS) == 4
    #define PORT_ANALOG_INPUT_COUNT   4
    #define PORT_ANALOG_INPUT        36,39,34,35
#elif (ANALOG_STICK_CHANNELS + ANALOG_OTHER_CHANNELS) == 5
    #define PORT_ANALOG_INPUT_COUNT   5
    #define PORT_ANALOG_INPUT        36,39,34,35,32
#elif (ANALOG_STICK_CHANNELS + ANALOG_OTHER_CHANNELS) == 6
    #define PORT_ANALOG_INPUT_COUNT   6
    #define PORT_ANALOG_INPUT        36,39,34,35,32,33
#else
    #error "ANALOG CHANNEL CONFIG failed."
#endif

/* Analog trim inputs */
#define PORT_TRIM_INPUT_COUNT     4
#define PORT_TRIM_INPUT          25,26,27,14

/* Auxiliary analog input: Vcc monitor */
#define PORT_AUX_INPUT_COUNT      1
#define PORT_AUX_INPUT           12


/* Two state or three state switches */
#if MECHANICAL_SWITCHES == 0
    #define PORT_SWITCH_INPUT_COUNT   0
    #define PORT_SWITCH_INPUT        
#elif MECHANICAL_SWITCHES == 1
    #define PORT_SWITCH_INPUT_COUNT   1
    #define PORT_SWITCH_INPUT         4
#elif MECHANICAL_SWITCHES == 2
    #define PORT_SWITCH_INPUT_COUNT   2
    #define PORT_SWITCH_INPUT         4,16
#elif MECHANICAL_SWITCHES == 3
    #define PORT_SWITCH_INPUT_COUNT   3
    #define PORT_SWITCH_INPUT         4,16,17
#elif MECHANICAL_SWITCHES == 4
    #define PORT_SWITCH_INPUT_COUNT   4
    #define PORT_SWITCH_INPUT         4,16,17
#elif MECHANICAL_SWITCHES == 5
    #define PORT_SWITCH_INPUT_COUNT   5
    #define PORT_SWITCH_INPUT         4,16,17
#elif MECHANICAL_SWITCHES == 6
    #define PORT_SWITCH_INPUT_COUNT   6
    #define PORT_SWITCH_INPUT         4,16,17
#else
    #error "SWITCH CONFIG failed."
#endif


#define PORT_HF_RELAIS            2
#define PORT_BIND_RELAIS          3
#define PORT_BUZZER              13


#if HF_MODULE == HF_SPEKTRUM_PPM
    #undef PPM_CHANNELS
    #define PPM_CHANNELS                  ((channel_t)9)

#elif HF_MODULE == HF_JETI_TU2
    #undef ENABLE_BIND_MODULE
    #undef ENABLE_RANGETEST_MODULE

#else
  #error "Set HF_MODULE in TXosLocalConfig.h to a supported value."
#endif

/* Battery monitor */

/* Voltage mutliplied by 100 to maintain 2 fractional digits.
 * 500 = 5.00V
 */
#define ADC_VOLTAGE               330
/* 10 bits => 1024 steps */
#define ADC_VCC_RESOLUTION       4096


#else
  #error "No supported architecture"
#endif


/* Voltage divider values in 1000Ohm units.*/
#define ADC_VOLTAGE_DIVIDER_R1     22
#define ADC_VOLTAGE_DIVIDER_R2     10

/* Number of battery cells */
#define VCC_CELLS                   8
/* Voltage mutliplied by 100 to maintain 2 fractional digits. */
#define VCC_WARN_LEVEL_PER_CELL   125
#define VCC_ALERT_LEVEL_PER_CELL  120

#endif
