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

#ifndef _TXosConfig_h
#define _TXosConfig_h

/* DE or EN */
#define UI_LANGUAGE              EN

/* Number of analog input channels.
 * This includes sticks, other analog inputs and
 * switched channels.
 */
#define ANALOG_CHANNELS          ((channel_t)8)

/* Logical channels for mixing.
 */
#define LOGICAL_CHANNELS         ((channel_t)11)

/* The number of transmitter channels.
 * This is the number of channels that will be used to
 * generate the PPM signal.
 */
#define PPM_CHANNELS             ((channel_t)9)

/* Total number of switches. Max is 16.
 * This includes channel switches and logical switches.
 */
#define SWITCHES                 ((uint8_t)8)

/* Switch configuration
 * In this case we have:
 *   4 tri state switches
 *   2 bi state switches
 *   2 control switches
 */

#define SWITCH_CONFIGURATION \
const switchConf_t switchConfiguration[SWITCHES] = { \
    SW_CONF_2STATE, \
    SW_CONF_3STATE, \
    SW_CONF_3STATE, \
    SW_CONF_2STATE, \
    SW_CONF_3STATE, \
    SW_CONF_3STATE, \
    SW_CONF_CHANNEL, \
    SW_CONF_CHANNEL \
};

/* Number of phases.
 * A value > 3 does not make any sense because we only 
 * have tri-state switches.
 */
#define PHASES                   ((phase_t)3)

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


/* Battery monitor */

/* Voltage mutliplied by 100 to maintain 2 fractional digits.
 * 500 = 5.00V
 */
#define ADC_VOLTAGE               500
/* 10 bits => 1024 steps */
#define ADC_VCC_RESOLUTION       1024

/* Voltage divider values in 1000Ohm units.*/
#define ADC_VOLTAGE_DIVIDER_R1     22
#define ADC_VOLTAGE_DIVIDER_R2     10

/* Number of battery cells */
#define VCC_CELLS                   8
/* Voltage mutliplied by 100 to maintain 2 fractional digits. */
#define VCC_WARN_LEVEL_PER_CELL   125
#define VCC_ALERT_LEVEL_PER_CELL  120

#endif
