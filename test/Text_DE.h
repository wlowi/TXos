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
#define TEXT_MODULE_ANALOG_SWITCH     CC("Geberschalter")
#define TEXT_MODULE_ASSIGN_INPUT      CC("Kanal Zuord.")
#define TEXT_MODULE_BIND              CC("Empf. binden")
#define TEXT_MODULE_CAL_STICKS        CC("Kalibrieren")
#define TEXT_MODULE_CAL_TRIM          CC("Trimmung Kal.")
#define TEXT_MODULE_CHANNEL_RANGE     CC("Geber Limit")
#define TEXT_MODULE_CHANNEL_REVERSE   CC("Geber Umkehr")
#define TEXT_MODULE_DUAL_EXPO         CC("Dual & Expo")
#define TEXT_MODULE_ENGINE_CUT        CC("Motor aus")
#define TEXT_MODULE_MODEL             CC("Modell")
#define TEXT_MODULE_MODEL_SELECT      CC("Modellauswahl")
#define TEXT_MODULE_MONITOR           CC("Servo Monitor")
#define TEXT_MODULE_PHASES            CC("Phasen")
#define TEXT_MODULE_PHASES_TRIM       CC("Phasentrim")
#define TEXT_MODULE_RANGE_TEST        CC("Reichw. Test")
#define TEXT_MODULE_SERVO_LIMIT       CC("Servo Limit")
#define TEXT_MODULE_SERVO_REMAP       CC("Servo Zuord.")
#define TEXT_MODULE_SERVO_REVERSE     CC("Servo Umkehr")
#define TEXT_MODULE_SERVO_SUBTRIM     CC("Servo Mitte")
#define TEXT_MODULE_STATISTICS        CC("Statistiken")
#define TEXT_MODULE_SWITCHED_CHANNELS CC("Schaltkanal")
#define TEXT_MODULE_SWITCHES          CC("Schalter")
#define TEXT_MODULE_SYSTEM_SETUP      CC("Systemeinst.")
#define TEXT_MODULE_TIMER             CC("Timer")
#define TEXT_MODULE_VCC_MONITOR       CC("Bat. Warnung")

// Analog input channels
#define TEXT_INPUT_CH_1             CC("In1")
#define TEXT_INPUT_CH_2             CC("In2")
#define TEXT_INPUT_CH_3             CC("In3")
#define TEXT_INPUT_CH_4             CC("In4")
#define TEXT_INPUT_CH_5             CC("In5")
#define TEXT_INPUT_CH_6             CC("In6")
#define TEXT_INPUT_CH_7             CC("In7")
#define TEXT_INPUT_CH_8             CC("In8")
#define TEXT_INPUT_CH_9             CC("In9")

// Control channels (3 letters fixed)
#define TEXT_CONTROL_CH_1           CC("Mot")
#define TEXT_CONTROL_CH_2           CC("QR ")
#define TEXT_CONTROL_CH_3           CC("HR ")
#define TEXT_CONTROL_CH_4           CC("SR ")
#define TEXT_CONTROL_CH_5           CC("QR2")
#define TEXT_CONTROL_CH_6           CC("WK ")
#define TEXT_CONTROL_CH_7           CC("WK2")
#define TEXT_CONTROL_CH_8           CC("LK ")
#define TEXT_CONTROL_CH_9           CC("LK2")
#define TEXT_CONTROL_CH_10          CC("FWk")
#define TEXT_CONTROL_CH_11          CC("K8 ")
#define TEXT_CONTROL_CH_12          CC("K9 ")

// Output channels (3 letters fixed)
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
#define TEXT_PHASE_THERMAL          CC("Thermik")
#define TEXT_PHASE_SPEED            CC("Speed")
#define TEXT_PHASE_START            CC("Start")
#define TEXT_PHASE_LAND             CC("Landung")
#define TEXT_PHASE_ACRO             CC("Akro")
#define TEXT_PHASE_ACRO2            CC("Akro 2")

// Misc
#define TEXT_ACTIVE                 CC("Aktiv")
#define TEXT_ALERT                  CC("Alarm")
#define TEXT_BACK                   CC("<<<<")
#define TEXT_BIND                   CC("Bind")
#define TEXT_BOOTING                CC("Booting...")
#define TEXT_CH                     CC("Ch")
#define TEXT_EXPO                   CC("Expo")
#define TEXT_MIX_OFFSET             CC("Of=")
#define TEXT_MODEL                  CC("Modell")
#define TEXT_MODEL_NAME             CC("Name")
#define TEXT_MODEL_SETUP            CC("Modelleinst.")
#define TEXT_PHASE                  CC("Phase")
#define TEXT_PHASE_PATTERN          CC("Ph-0")
#define TEXT_POS0                   CC("Pos0")
#define TEXT_POS1                   CC("Pos1")
#define TEXT_POS2                   CC("Pos2")
#define TEXT_RATE                   CC("Rate")
#define TEXT_SC_PATTERN             CC("SC*")
#define TEXT_START                  CC("Start")
#define TEXT_SW                     CC("Sw")
#define TEXT_SWITCH                 CC("Schalt")
#define TEXT_SYSTEM_SETUP           CC("Systemeinst.")
#define TEXT_TEST                   CC("Test")
#define TEXT_THR                    CC("Mot")
#define TEXT_TIME                   CC("Time")
#define TEXT_TXOS                   CC("TXos")
#define TEXT_VCC_ADJUST             CC("+/-")
#define TEXT_WARN                   CC("Warn")
#define TEXT_WINGMIX                CC("Mix")

// Wing mix options
#define TEXT_WINGMIX_length         6
#define TEXT_WINGMIX_NORMAL         CC("Normal")
#define TEXT_WINGMIX_DELTA          CC("Delta")
#define TEXT_WINGMIX_VTAIL          CC("V-Lw")

#define TEXT_MIX_AIL_RUD            CC("QR>SR")
#define TEXT_MIX_AIL_FLP            CC("QR>WK")
#define TEXT_MIX_SPL_AIL            CC("LK>QR")
#define TEXT_MIX_SPL_FLP            CC("LK>WK")
#define TEXT_MIX_SPL_ELV            CC("LK>HR")
#define TEXT_MIX_FLP_AIL            CC("WK>QR")
#define TEXT_MIX_FLP_ELV            CC("WK>HR")
#define TEXT_MIX_ELV_AIL            CC("HR>QR")
#define TEXT_MIX_ELV_FLP            CC("HR>WK")
#define TEXT_MIX_RUD_ELV            CC("SR>HR")

// Binary states
#define TEXT_ON_OFF_length          3
#define TEXT_ON                     CC("Ein")
#define TEXT_OFF                    CC("Aus")

// Statistics
#define TEXT_STATISTIC_UI           CC("UI")
#define TEXT_STATISTIC_MODULE       CC("MODULE")

/* User interface warnings and messages */
#define TEXT_MSG_count              5

#define MSG_NONE                    0 //1234567890123
#define TEXT_MSG_NONE               CC(" ")

#define MSG_BAD_SYSCONFIG           1 //1234567890123
#define TEXT_MSG_BAD_SYSCONFIG      CC("Bad Sysconfig")

#define MSG_LOW_BATT                2 //1234567890123
#define TEXT_MSG_LOW_BATT           CC("Batterie Warn")

#define MSG_MODEL_LOAD_FAILED       3 //1234567890123
#define TEXT_MSG_MODEL_LOAD_FAILED  CC("Mod. load err")

#define MSG_CONFIG_SIZE             4 //1234567890123
#define TEXT_MSG_CONFIG_SIZE        CC("Mod. cfg size")

#endif
