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

#ifndef _Text_h_
#define _Text_h_

#define CC( t ) ((const char*)t)

// Module Names (13 letters max.)       1234567890123
#define TEXT_MODULE_BIND            CC("Bind RX")
#define TEXT_MODULE_CAL_STICKS      CC("Calib. Sticks")
#define TEXT_MODULE_CAL_TRIM        CC("Calib. Trim")
#define TEXT_MODULE_DUAL_EXPO       CC("Dual & Expo")
#define TEXT_MODULE_ENGINE_CUT      CC("Engine Cut")
#define TEXT_MODULE_MODEL           CC("Model")
#define TEXT_MODULE_MODEL_SELECT    CC("Model Select")
#define TEXT_MODULE_MONITOR         CC("Monitor")
#define TEXT_MODULE_PHASES          CC("Phases")
#define TEXT_MODULE_RANGE_TEST      CC("Range Test")
#define TEXT_MODULE_SERVO_LIMIT     CC("Servo Limit")
#define TEXT_MODULE_SERVO_REVERSE   CC("Servo Reverse")
#define TEXT_MODULE_SERVO_SUBTRIM   CC("Servo Subtrim")
#define TEXT_MODULE_SWITCHES        CC("Switches")
#define TEXT_MODULE_SYSTEM_SETUP    CC("System Setup")
#define TEXT_MODULE_TIMER           CC("Timer")
#define TEXT_MODULE_VCC_MONITOR     CC("Bat. Warning")

// Control channels (3 letters fixed)
#define TEXT_CONTROL_CHANNEL_1      CC("THR")
#define TEXT_CONTROL_CHANNEL_2      CC("AIL")
#define TEXT_CONTROL_CHANNEL_3      CC("ELV")
#define TEXT_CONTROL_CHANNEL_4      CC("RUD")
#define TEXT_CONTROL_CHANNEL_5      CC("CH5")
#define TEXT_CONTROL_CHANNEL_6      CC("CH6")
#define TEXT_CONTROL_CHANNEL_7      CC("CH7")
#define TEXT_CONTROL_CHANNEL_8      CC("CH8")
#define TEXT_CONTROL_CHANNEL_9      CC("CH9")

// Output channels (3 letters fixed)
#define TEXT_OUT_CHANNEL_1          CC("S1")
#define TEXT_OUT_CHANNEL_2          CC("S2")
#define TEXT_OUT_CHANNEL_3          CC("S3")
#define TEXT_OUT_CHANNEL_4          CC("S4")
#define TEXT_OUT_CHANNEL_5          CC("S5")
#define TEXT_OUT_CHANNEL_6          CC("S6")
#define TEXT_OUT_CHANNEL_7          CC("S7")
#define TEXT_OUT_CHANNEL_8          CC("S8")
#define TEXT_OUT_CHANNEL_9          CC("S9")

// Switch types (3 letters fixed)
// Note name length is 4 because the format includes the switch number
// n-tt  (1-BI)
#define TEXT_SW_NAME_length         4
#define TEXT_SW_TYPE_UNUSED         CC("----")
#define TEXT_SW_TYPE_2_STATE        CC("BI")
#define TEXT_SW_TYPE_3_STATE        CC("TR")
#define TEXT_SW_TYPE_CHANNEL        CC("CH")

// Calibration steps (9 letters max)
#define TEXT_CALIB_length           9
#define TEXT_CALIB_CENTER           CC("CENTER")
#define TEXT_CALIB_MINMAX           CC("MINMAX")
#define TEXT_CALIB_NONE             CC("CALIBRATE")

// Phases (7 letters max.)
#define TEXT_PHASE_NAME_length      7
#define TEXT_NOPHASE                CC("NOPHASE")
#define TEXT_PHASES_count           7
#define TEXT_PHASE_NORMAL           CC("Normal")
#define TEXT_PHASE_THERMAL          CC("Thermal")
#define TEXT_PHASE_SPEED            CC("Speed")
#define TEXT_PHASE_START            CC("Start")
#define TEXT_PHASE_LAND             CC("Landing")
#define TEXT_PHASE_ACRO             CC("Acro")
#define TEXT_PHASE_ACRO2            CC("Acro 2")

// Misc
#define TEXT_TXOS                   CC("TXos ")
#define TEXT_BACK                   CC("< back >")
#define TEXT_BOOTING                CC("Booting...")
#define TEXT_MODEL_SETUP            CC("Model setup")
#define TEXT_SYSTEM_SETUP           CC("System setup")
#define TEXT_MODEL                  CC("Model")
#define TEXT_MODEL_NAME             CC("Name")
#define TEXT_SWITCH                 CC("Switch")
#define TEXT_THR                    CC("THR")
#define TEXT_WINGMIX                CC("Mix")
#define TEXT_WARN                   CC("Warn")
#define TEXT_ALERT                  CC("Alert")
#define TEXT_VCC_ADJUST             CC("+/-")
#define TEXT_PHASE                  CC("Phase")
#define TEXT_RATE                   CC("Rate")
#define TEXT_EXPO                   CC("Expo")

// Wing mix options
#define TEXT_WINGMIX_length         6
#define TEXT_WINGMIX_1AIL           CC("1-Ail")
#define TEXT_WINGMIX_2AIL           CC("2-Ail")
#define TEXT_WINGMIX_DELTA          CC("Delta")
#define TEXT_WINGMIX_VTAIL          CC("V-Mix")
#define TEXT_WINGMIX_VTAIL2         CC("V-Mix2")

// Binary states
#define TEXT_ON_OFF_length          3
#define TEXT_ON                     CC("On")
#define TEXT_OFF                    CC("Off")

/* User interface warnings and messages */
#define TEXT_MSG_count              5

#define MSG_NONE                    0 //1234567890123
#define TEXT_MSG_NONE               CC(" ")

#define MSG_BAD_SYSCONFIG           1 //1234567890123
#define TEXT_MSG_BAD_SYSCONFIG      CC("Bad Sysconfig")

#define MSG_LOW_BATT                2 //1234567890123
#define TEXT_MSG_LOW_BATT           CC("Low Battery")

#define MSG_MODEL_LOAD_FAILED       3 //1234567890123
#define TEXT_MSG_MODEL_LOAD_FAILED  CC("Mod. load err")

#define MSG_CONFIG_SIZE             4 //1234567890123
#define TEXT_MSG_CONFIG_SIZE        CC("Mod. cfg size")

#endif
