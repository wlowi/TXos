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
#define TEXT_MODULE_ENGINE_CUT      CC("Engine Cut")
#define TEXT_MODULE_MODEL           CC("Model")
#define TEXT_MODULE_MODEL_SELECT    CC("Model Select")
#define TEXT_MODULE_MONITOR         CC("Monitor")
#define TEXT_MODULE_RANGE_TEST      CC("Range Test")
#define TEXT_MODULE_SERVO_LIMIT     CC("Servo limit")
#define TEXT_MODULE_SERVO_REVERSE   CC("Servo reverse")
#define TEXT_MODULE_SERVO_SUBTRIM   CC("Servo subtrim")
#define TEXT_MODULE_SWITCHES        CC("Switches")
#define TEXT_MODULE_SYSTEM_SETUP    CC("System Setup")

// Control channels (3 letters max.)
#define TEXT_CONTROL_CHANNEL_1      CC("THR")
#define TEXT_CONTROL_CHANNEL_2      CC("AIL")
#define TEXT_CONTROL_CHANNEL_3      CC("ELV")
#define TEXT_CONTROL_CHANNEL_4      CC("RUD")
#define TEXT_CONTROL_CHANNEL_5      CC("CH5")
#define TEXT_CONTROL_CHANNEL_6      CC("CH6")
#define TEXT_CONTROL_CHANNEL_7      CC("CH7")
#define TEXT_CONTROL_CHANNEL_8      CC("CH8")
#define TEXT_CONTROL_CHANNEL_9      CC("CH9")

// Output channels (3 letters max.)
#define TEXT_OUT_CHANNEL_1          CC("S1")
#define TEXT_OUT_CHANNEL_2          CC("S2")
#define TEXT_OUT_CHANNEL_3          CC("S3")
#define TEXT_OUT_CHANNEL_4          CC("S4")
#define TEXT_OUT_CHANNEL_5          CC("S5")
#define TEXT_OUT_CHANNEL_6          CC("S6")
#define TEXT_OUT_CHANNEL_7          CC("S7")
#define TEXT_OUT_CHANNEL_8          CC("S8")
#define TEXT_OUT_CHANNEL_9          CC("S9")

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
#define TEXT_WINGMIX_1AIL           CC(" 1-Ail")
#define TEXT_WINGMIX_2AIL           CC(" 2-Ail")
#define TEXT_WINGMIX_DELTA          CC(" Delta")
#define TEXT_WINGMIX_VTAIL          CC(" V-Mix")
#define TEXT_WINGMIX_VTAIL2         CC("V-Mix2")

#endif
