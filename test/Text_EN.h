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

// Module Names (13 letters max.)         1234567890123
#define TEXT_MODULE_ANALOG_SWITCH     CC("Ch. Switches")
#define TEXT_MODULE_ASSIGN_INPUT      CC("Assign Ch.")
#define TEXT_MODULE_BIND              CC("Bind RX")
#define TEXT_MODULE_CAL_STICKS        CC("Calib. Sticks")
#define TEXT_MODULE_CAL_TRIM          CC("Calib. Trim")
#define TEXT_MODULE_CHANNEL_DELAY     CC("Channel Delay")
#define TEXT_MODULE_CHANNEL_RANGE     CC("Channel Limit")
#define TEXT_MODULE_CHANNEL_REVERSE   CC("Channel Rev.")
#define TEXT_MODULE_DUAL_EXPO         CC("Dual & Expo")
#define TEXT_MODULE_ENGINE_CUT        CC("Engine Cut")
#define TEXT_MODULE_MIXER             CC("Mixer")
#define TEXT_MODULE_MODEL             CC("Model")
#define TEXT_MODULE_MODEL_SELECT      CC("Model Select")
#define TEXT_MODULE_MONITOR           CC("Servo Monitor")
#define TEXT_MODULE_PHASES            CC("Phases")
#define TEXT_MODULE_PHASES_TRIM       CC("Phases Trim")
#define TEXT_MODULE_RANGE_TEST        CC("Range Test")
#define TEXT_MODULE_SERVO_LIMIT       CC("Servo Limit")
#define TEXT_MODULE_SERVO_REMAP       CC("Servo Remap")
#define TEXT_MODULE_SERVO_REVERSE     CC("Servo Reverse")
#define TEXT_MODULE_SERVO_SUBTRIM     CC("Servo Subtrim")
#define TEXT_MODULE_STATISTICS        CC("Statistics")
#define TEXT_MODULE_SWITCHED_CHANNELS CC("Switched Ch.")
#define TEXT_MODULE_SWITCHES          CC("Switches")
#define TEXT_MODULE_SYSTEM_SETUP      CC("System Setup")
#define TEXT_MODULE_TIMER             CC("Timer")
#define TEXT_MODULE_VCC_MONITOR       CC("Bat. Warning")

// Analog input channels
#define TEXT_INPUT_length           3
#define TEXT_INPUT_CH_1             CC("In1")
#define TEXT_INPUT_CH_2             CC("In2")
#define TEXT_INPUT_CH_3             CC("In3")
#define TEXT_INPUT_CH_4             CC("In4")
#define TEXT_INPUT_CH_5             CC("In5")
#define TEXT_INPUT_CH_6             CC("In6")
#define TEXT_INPUT_CH_7             CC("In7")
#define TEXT_INPUT_CH_8             CC("In8")
#define TEXT_INPUT_CH_9             CC("In9")

// Control channels
#define TEXT_CONTROL_length         3
#define TEXT_CONTROL_CH_1           CC("Thr")
#define TEXT_CONTROL_CH_2           CC("Ail")
#define TEXT_CONTROL_CH_3           CC("Elv")
#define TEXT_CONTROL_CH_4           CC("Rud")
#define TEXT_CONTROL_CH_5           CC("Ai2")
#define TEXT_CONTROL_CH_6           CC("Flp")
#define TEXT_CONTROL_CH_7           CC("Fl2")
#define TEXT_CONTROL_CH_8           CC("Spl")
#define TEXT_CONTROL_CH_9           CC("Sp2")
#define TEXT_CONTROL_CH_10          CC("Gea")
#define TEXT_CONTROL_CH_11          CC("Ch8")
#define TEXT_CONTROL_CH_12          CC("Ch9")

// Output channels
#define TEXT_OUT_length             2
#define TEXT_OUT_CH_1               CC("S1")
#define TEXT_OUT_CH_2               CC("S2")
#define TEXT_OUT_CH_3               CC("S3")
#define TEXT_OUT_CH_4               CC("S4")
#define TEXT_OUT_CH_5               CC("S5")
#define TEXT_OUT_CH_6               CC("S6")
#define TEXT_OUT_CH_7               CC("S7")
#define TEXT_OUT_CH_8               CC("S8")
#define TEXT_OUT_CH_9               CC("S9")

// Switch types (2 letters fixed)
// Note name length is 3 because the format includes the switch number
// ntt  (1BI)
#define TEXT_SW_NAME_length         3
#define TEXT_SW_NAME_STATE_length   5

#define TEXT_SW_TYPE_UNUSED         CC("---")
#define TEXT_SW_TYPE_2_STATE        CC("BI")
#define TEXT_SW_TYPE_3_STATE        CC("TR")
#define TEXT_SW_TYPE_CHANNEL        CC("CS")
#define TEXT_SW_TYPE_FIXED_ON       CC("ON")

// Calibration steps (7 letters max)
#define TEXT_CALIB_length           7
#define TEXT_CALIB_CENTER           CC("CENTER")
#define TEXT_CALIB_MINMAX           CC("MIN/MAX")
#define TEXT_CALIB_NONE             CC("CALIB.")

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
#define TEXT_ACTIVE                 CC("Active")
#define TEXT_AIL_DIFF               CC("Ail-dif")
#define TEXT_ALERT                  CC("Alert")
#define TEXT_ARROW_LEFT             CC("<")
#define TEXT_ARROW_RIGHT            CC(">")
#define TEXT_BACK                   CC("<<<<")
#define TEXT_BIND                   CC("Bind")
#define TEXT_BOOTING                CC("Booting...")
#define TEXT_CH                     CC("Ch")
#define TEXT_EXPO                   CC("Expo")
#define TEXT_MIX_ARROW              CC("=>")
#define TEXT_MIX_OFFSET             CC("Of=")
#define TEXT_MODEL                  CC("Model")
#define TEXT_MODEL_NAME             CC("Name")
#define TEXT_MODEL_SETUP            CC("Model setup")
#define TEXT_PHASE                  CC("Phase")
#define TEXT_PHASE_PATTERN          CC("Ph-0")
#define TEXT_POS0                   CC("Pos0")
#define TEXT_POS1                   CC("Pos1")
#define TEXT_POS2                   CC("Pos2")
#define TEXT_RATE                   CC("Rate")
#define TEXT_SC_PATTERN             CC("SC*")
#define TEXT_SEC                    CC("sec")
#define TEXT_START                  CC("Start")
#define TEXT_SW                     CC("Sw")
#define TEXT_SWITCH                 CC("Switch")
#define TEXT_SYSTEM_SETUP           CC("System setup")
#define TEXT_TEST                   CC("Test")
#define TEXT_THR                    CC("THR")
#define TEXT_TIME                   CC("Time")
#define TEXT_TXOS                   CC("TXos")
#define TEXT_VCC_ADJUST             CC("+/-")
#define TEXT_WARN                   CC("Warn")
#define TEXT_WINGMIX                CC("Mix")

// Wing mix options
#define TEXT_WINGMIX_length         6
#define TEXT_WINGMIX_NORMAL         CC("Normal")
#define TEXT_WINGMIX_DELTA          CC("Delta")
#define TEXT_WINGMIX_VTAIL          CC("V-Tail")

#define TEXT_MIX_AIL_RUD            CC("Ail>Rud")
#define TEXT_MIX_AIL_FLP            CC("Ail>Flp")
#define TEXT_MIX_SPL_AIL            CC("Spl>Ail")
#define TEXT_MIX_SPL_FLP            CC("Spl>Flp")
#define TEXT_MIX_SPL_ELV            CC("Spl>Elv")
#define TEXT_MIX_FLP_AIL            CC("Fpl>Ail")
#define TEXT_MIX_FLP_ELV            CC("Fpl>Elv")
#define TEXT_MIX_ELV_AIL            CC("Elv>Ail")
#define TEXT_MIX_ELV_FLP            CC("Elv>Flp")
#define TEXT_MIX_RUD_ELV            CC("Rud>Elv")

// Binary states
#define TEXT_ON_OFF_length          3
#define TEXT_ON                     CC("On")
#define TEXT_OFF                    CC("Off")

// Statistics
#define TEXT_STATISTIC_TIMING       CC("Timing")
#define TEXT_STATISTIC_OVERRUN      CC("PPM-Ovr")

#define TEXT_STATISTIC_UI           CC("UI")
#define TEXT_STATISTIC_MODULE       CC("Modules")
#define TEXT_STATISTIC_PPMOVER      CC("PPM-Ovr")
#define TEXT_STATISTIC_FRAMETIME    CC("Frame")
#define TEXT_STATISTIC_WDT          CC("WDT")

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
