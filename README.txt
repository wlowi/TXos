
Board:
======
Arduino Mega or Mega 2560

Required libraries:
===================
Adafruit ST7735 library


Inputs:
9: 
   6 Analog              In1 - In6
   3 Switched Channels   In7 - In9

9 Switches:
   6 Switches            Sw1 - Sw6
   2 Analog switches     7CS - 8CS
   1 Fixed ON            9ON

Logical Channels:
12: Thr Ail Elv Rud Ai2 Flp Fl2 Spl Sp2 Gea Ch8 Ch9

Servo Outputs:
9: S1 - S9


Module order of execution
=========================

OK: Calibrate Sticks:
  Adjust range of all analog channels to +/- 1250

OK: Calibrate Trim:
  Add trim offset to first 4 analog channels ( +/- 250 )

OK: Switched Channels:
  Set value of switched channels In7,In8 from switch state (Range +/- 125%)

OK: Analog Switches:
  Switch controlled by analog input In1 - In8

OK: Channel Range:
  Adjust range for analog channels In1 - In6 (Range +/- 125%)

OK: Channel Reverse:
  Reverse channel input

OK: Assign Inputs:
  Assign inputs to logical channels
  Ac1 - Thr
  Ac2 - Ail
  Ac3 - Elv
  Ac4 - Rud
  Ac5 - Flp
  Ac6 - Ch8
  Ac7 - Gea
  Ac8 - Ch9

OK: Phases:
  no effect on channels

OK: Phased Rate & Expo:
  Phase controlled rate limit and exponential function for Ail, Elv, Rud

OK: Model:
  V tail mix
  Delta mix
  QR Diff
  Ail -> Flp mix
  Flp -> Ail mix
  Flp -> Elv mix
  Ail -> Rud mix

General Mixer:

OK: Phased Trim:
  Phase controlled mid position of Ail, Elv, Flaps and Spoiler

OK: Engine Cut:
  Set Thr output to fixed value controlled by a switch

OK: Servo Remap:
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

Servo Speed:

OK: Servo Reverse:
  Revers servo direction for Sv1 - Sv9
  Default: off

OK: Servo Subtrim:
  Adjust mid position for Sv1 - Sv9
  Default: 0

OK: Servo Limit:
  Hard lower/upper limit for Sv1 - Sv9
  Default: +/- 125%

OK: Servo Monitor:
  no effect on channels

OK: Switch Monitor:
  no effect on channels

OK: Timer:
  no effect on channels

OK: vccMonitor:
  no effect on channels



TODO:
=====

Cell editing: Switch number and state
    1-BI:0
    2-TR:2
    3-CH:1

Module processing within PPM generation frame.

