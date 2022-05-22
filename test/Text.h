
#ifndef _Text_h_
#define _Text_h_

#define CC( t ) ((const char*)t)

// Module Names (13 letters max.)       1234567890123
#define TEXT_MODULE_MODEL_SELECT    CC("Model Select")
#define TEXT_MODULE_SERVO_LIMIT     CC("Servo limit")
#define TEXT_MODULE_SERVO_REVERSE   CC("Servo reverse")
#define TEXT_MODULE_SERVO_SUBTRIM   CC("Servo subtrim")

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
#define TEXT_SELECT                 CC("Select")
#define TEXT_MODEL                  CC("Model")

#endif