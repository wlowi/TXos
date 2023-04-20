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

#ifndef _Text_h_
#define _Text_h_

#define CC( t ) ((const char*)t)

// Module Names (13 letters max.)         1234567890123
#define TEXT_MODULE_ANALOG_SWITCH     CC("Geberschalter")
#define TEXT_MODULE_ANALOG_TRIM       CC("Trimmspeicher")
#define TEXT_MODULE_ASSIGN_INPUT      CC("Kanal Zuord.")
#define TEXT_MODULE_BIND              CC("Empf. binden")
#define TEXT_MODULE_CAL_STICKS        CC("Kalibrieren")
#define TEXT_MODULE_CAL_TRIM          CC("Trimmung Kal.")
#define TEXT_MODULE_CHANNEL_DELAY     CC("Kanal Verz.")
#define TEXT_MODULE_CHANNEL_RANGE     CC("Geber Limit")
#define TEXT_MODULE_CHANNEL_REVERSE   CC("Geber Umkehr")
#define TEXT_MODULE_DUAL_EXPO         CC("Dual & Expo")
#define TEXT_MODULE_ENGINE_CUT        CC("Motor aus")
#define TEXT_MODULE_LOGIC_SWITCH      CC("Logikschalter")
#define TEXT_MODULE_MIXER             CC("Mischer")
#define TEXT_MODULE_MODE_ASSIGN       CC("Mode Zuord.")
#define TEXT_MODULE_MODEL             CC("Modell")
#define TEXT_MODULE_MODEL_SELECT      CC("Modellauswahl")
#define TEXT_MODULE_MONITOR           CC("Servo Monitor")
#define TEXT_MODULE_PHASES            CC("Phasen")
#define TEXT_MODULE_PHASES_TRIM       CC("Phasentrimm")
#define TEXT_MODULE_RANGE_TEST        CC("Reichw. Test")
#define TEXT_MODULE_SERVO_LIMIT       CC("Servo Limit")
#define TEXT_MODULE_SERVO_REMAP       CC("Servo Zuord.")
#define TEXT_MODULE_SERVO_REVERSE     CC("Servo Umkehr")
#define TEXT_MODULE_SERVO_SUBTRIM     CC("Servo Mitte")
#define TEXT_MODULE_SERVO_TEST        CC("Servo Test")
#define TEXT_MODULE_STATISTICS        CC("Statistiken")
#define TEXT_MODULE_SWITCHED_CHANNELS CC("Schaltkanal")
#define TEXT_MODULE_SWITCHES          CC("Schalter")
#define TEXT_MODULE_SYSTEM_SETUP      CC("Systemeinst.")
#define TEXT_MODULE_TIMER             CC("Timer")
#define TEXT_MODULE_VCC_MONITOR       CC("Bat. Warnung")

// Analog input channels
#define TEXT_INPUT_length           ((uint8_t)3)
#define TEXT_INPUT_CH_1             CC("In1")
#define TEXT_INPUT_CH_2             CC("In2")
#define TEXT_INPUT_CH_3             CC("In3")
#define TEXT_INPUT_CH_4             CC("In4")
#define TEXT_INPUT_CH_5             CC("In5")
#define TEXT_INPUT_CH_6             CC("In6")
#define TEXT_INPUT_CH_7             CC("In7")
#define TEXT_INPUT_CH_8             CC("In8")
#define TEXT_INPUT_CH_9             CC("In9")
#define TEXT_INPUT_NONE             CC("---")

// Control channels
#define TEXT_CONTROL_length         ((uint8_t)3)
#define TEXT_CONTROL_CH_1           CC("Mot")
#define TEXT_CONTROL_CH_2           CC("QR ")
#define TEXT_CONTROL_CH_3           CC("HR ")
#define TEXT_CONTROL_CH_4           CC("SR ")
#define TEXT_CONTROL_CH_5           CC("WK ")
#define TEXT_CONTROL_CH_6           CC("LK ")
#define TEXT_CONTROL_CH_7           CC("FWk")
#define TEXT_CONTROL_CH_8           CC("K8 ")
#define TEXT_CONTROL_CH_9           CC("K9 ")

#define TEXT_CONTROL_CH_10          CC("QR2")
#define TEXT_CONTROL_CH_11          CC("WK2")
#define TEXT_CONTROL_CH_12          CC("LK2")

// Output channels
#define TEXT_OUT_length             ((uint8_t)2)
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
#define TEXT_SW_NAME_length         ((uint8_t)3)
#define TEXT_SW_NAME_STATE_length   ((uint8_t)5)

#define TEXT_SW_TYPE_UNUSED         CC("---")
#define TEXT_SW_TYPE_2_STATE        CC("BI")
#define TEXT_SW_TYPE_3_STATE        CC("TR")
#define TEXT_SW_TYPE_CHANNEL        CC("CS")
#define TEXT_SW_TYPE_FIXED_ON       CC(" ON")
#define TEXT_SW_TYPE_LOGIC          CC("LS")
#define TEXT_SW_TYPE_PHASE          CC("PHA")
#define TEXT_SW_TYPE_PHASE_N        CC("PH")

// Logic Switches
#define TEXT_LOGIC_SW_TYPE_count    ((uint8_t)6)
#define TEXT_LOGIC_SW_TYPE_length   ((uint8_t)7)

#define TEXT_LOGIC_SW_TYPE1         CC("A&B")
#define TEXT_LOGIC_SW_TYPE2         CC("A|B")
#define TEXT_LOGIC_SW_TYPE3         CC("A&B&C")
#define TEXT_LOGIC_SW_TYPE4         CC("A|B|C")
#define TEXT_LOGIC_SW_TYPE5         CC("(A&B)|C")
#define TEXT_LOGIC_SW_TYPE6         CC("(A|B)&C")

#define TEXT_LOGIC_SW_A             CC("A")
#define TEXT_LOGIC_SW_B             CC("B")
#define TEXT_LOGIC_SW_C             CC("C")

// Calibration steps (7 letters max)
#define TEXT_CALIB_length           ((uint8_t)7)
#define TEXT_CALIB_CENTER           CC("CENTER")
#define TEXT_CALIB_MINMAX           CC("MIN/MAX")
#define TEXT_CALIB_NONE             CC("CALIB.")

// Phases (7 letters max.)
#define TEXT_PHASE_NAME_length      ((uint8_t)7)
#define TEXT_NOPHASE                CC("NOPHASE")
#define TEXT_PHASES_count           ((uint8_t)9)
#define TEXT_PHASE_NORMAL           CC("Normal")
#define TEXT_PHASE_THERMAL          CC("Thermik")
#define TEXT_PHASE_SPEED            CC("Speed")
#define TEXT_PHASE_TAKEOFF          CC("Start")
#define TEXT_PHASE_LAND             CC("Landung")
#define TEXT_PHASE_ACRO             CC("Akro")
#define TEXT_PHASE_ACRO2            CC("Akro 2")
#define TEXT_PHASE_DIST             CC("Strecke")
#define TEXT_PHASE_TOW              CC("Schlepp")

// Misc
#define TEXT_ACTIVE                 CC("Aktiv")
#define TEXT_AIL_DIFF               CC("QR-Diff")
#define TEXT_ALERT                  CC("Alarm")
#define TEXT_ARROW_LEFT             CC("<")
#define TEXT_ARROW_RIGHT            CC(">")
#define TEXT_BACK                   CC("<<<<")
#define TEXT_BIND                   CC("Bind")
#define TEXT_BOOTING                CC("Booting...")
#define TEXT_CH                     CC("Ch")
#define TEXT_EXPO                   CC("Expo")
#define TEXT_MIX_ARROW              CC("=>")
#define TEXT_MIX_OFFSET             CC("Of=")
#define TEXT_MODEL                  CC("Modell")
#define TEXT_MODEL_NAME             CC("Name")
#define TEXT_MODEL_SETUP            CC("Modelleinst.")
#define TEXT_PHASE                  CC("Phase")
#define TEXT_PHASE_PATTERN          CC("Ph-*")
#define TEXT_POS0                   CC("Pos0")
#define TEXT_POS1                   CC("Pos1")
#define TEXT_POS2                   CC("Pos2")
#define TEXT_RATE                   CC("Rate")
#define TEXT_SEC                    CC("sek")
#define TEXT_START                  CC("Start")
#define TEXT_SW                     CC("Sw")
#define TEXT_SWITCH                 CC("Schalt")
#define TEXT_SYSTEM_SETUP           CC("Systemeinst.")
#define TEXT_TEST                   CC("Test")
#define TEXT_THR                    CC("Mot")
#define TEXT_TIME                   CC("Zeit")
#define TEXT_TXOS                   CC("TXos")
#define TEXT_VCC_ADJUST             CC("+/-")
#define TEXT_WARN                   CC("Warn")
#define TEXT_WINGMIX                CC("Mix")

// Wing mix options
#define TEXT_WINGMIX_length         ((uint8_t)6)
#define TEXT_WINGMIX_count          ((uint8_t)3)
#define TEXT_WINGMIX_NORMAL         CC("Normal")
#define TEXT_WINGMIX_DELTA          CC("Delta")
#define TEXT_WINGMIX_VTAIL          CC("V-Lw")

#define TEXT_MIX_count              ((uint8_t)10)
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
#define TEXT_ON_OFF_length          ((uint8_t)3)
#define TEXT_ON                     CC("Ein")
#define TEXT_OFF                    CC("Aus")

// Statistics
#define TEXT_STATISTIC_TIMING       CC("Timing")
#define TEXT_STATISTIC_OVERRUN      CC("PPM-Ovr")

#define TEXT_STATISTIC_UI           CC("UI")
#define TEXT_STATISTIC_MODULE       CC("Modules")
#define TEXT_STATISTIC_PPMOVER      CC("PPM-Ovr")
#define TEXT_STATISTIC_FRAMETIME    CC("Frame")
#define TEXT_STATISTIC_WDT          CC("WDT")

/* User interface warnings and messages */
#define TEXT_MSG_count              ((uint8_t)5)

#define MSG_NONE                    ((uint8_t)0) //1234567890123
#define TEXT_MSG_NONE               CC(" ")

#define MSG_BAD_SYSCONFIG           ((uint8_t)1) //1234567890123
#define TEXT_MSG_BAD_SYSCONFIG      CC("Bad Sysconfig")

#define MSG_LOW_BATT                ((uint8_t)2) //1234567890123
#define TEXT_MSG_LOW_BATT           CC("Batterie Warn")

#define MSG_MODEL_LOAD_FAILED       ((uint8_t)3) //1234567890123
#define TEXT_MSG_MODEL_LOAD_FAILED  CC("Mod. load err")

#define MSG_CONFIG_SIZE             ((uint8_t)4) //1234567890123
#define TEXT_MSG_CONFIG_SIZE        CC("Mod. cfg size")

#endif
