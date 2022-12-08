
Board:
======
Arduino Mega or Mega 2560

Required libraries:
===================
Adafruit ST7735 library


Inputs:
8: 
   6 Analog              In1 - In6
   2 Switched Channels   In7 - In8

8 Switches:
   6 Switches            Sw1 - Sw6
   2 Analog switches     Sw7 - Sw8

Logical Channels:
11: Thr Ail Elv Rud Ai2 El2 Flp Fl2 Gea Ch8 Ch9

Servo Outputs:
9: Sv1 - Sv9


Module order of execution
=========================

Calibrate Sticks:
  Adjust range of all analog channels to +/- 1000

Calibrate Trim:
  Add trim offset to first 4 analog channels ( +/- 250 )

Switched Channels:
  Set value of switched channels Ac7,Ac8 from switch state (Range +/- 125%)

Analog Switches:
  Switch controlled by analog input Ac1 - Ac8
  
Channel Rate:
  Adjust range for analog channels Ac1 - Ac6 (Range +/- 125%)

Assign Inputs:
  Assign inputs to logical channels
  Ac1 - Thr
  Ac2 - Ail
  Ac3 - Elv
  Ac4 - Rud
  Ac5 - Flp
  Ac6 - Ch8
  Ac7 - Gea
  Ac8 - Ch9

Phases:
  no effect on channels



Phased Rate & Expo:
  Phase controlled rate limit and exponential function for Ail, Elv, Rud

Model:
  1 or 2 ailerons
  1 or 2 elevators
  1 or 2 flaps
  V tail mix
  Delta mix
  Ail -> Flp mix
  Flp -> Ail mix
  Flp -> Elv mix
  Ail -> Rud mix

Mixer:

Phased Trim:
  Phase controlled mid position of Ail, Elv, Rud



Engine Cut:
  Set Thr output to fixed value controlled by a switch

Servo Remap:
  Map logical channels to servo output
  Thr - Sv1
  Ail - Sv2
  Elv - Sv3
  Rud - Sv4
  Ai2 - Sv5
  Flp - Sv6
  Fl2 - Sv7
  Ch8 - Sv8
  Ch9 - Sv9

Servo Reverse:
  Revers servo direction for Sv1 - Sv9
  Default: off

Servo Subtrim:
  Adjust mid position for Sv1 - Sv9
  Default: 0

Servo Limit:
  Hard lower/upper limit for Sv1 - Sv9
  Default: +/- 125%

Servo Monitor:
  no effect on channels

Switch Monitor:
  no effect on channels

Timer:
  no effect on channels

vccMonitor:
  no effect on channels



TODO:
=====

Cell editing: Switch number and state
    1-BI:0
    2-TR:2
    3-CH:1

Module: Rate & Expo

Module: Channel Range

Module: Phase Trim

Module: Geber Switches

Module: Timer

  1234567890123
 +-------------+
1|Timer        |
2|<back>       |
3|Switch 1-TR:2|
4|Time   00:00 |
5|             |
6|             |
7|             |
8|             |
 +-------------+

Module: Mixer

Module: Servo remap

Module: Servo speed

Module processing within PPM generation frame.

