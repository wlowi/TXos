/*
 * DigitalInput.cpp
 */

#ifndef _DigitalInput_h
#define _DigitalInput_h

#include "Arduino.h"
#include "TXos.h"

/* Numer of supported channels. */
#define DIGITAL_CHANNELS      ((channel_t)5)


class DigitalInput {
public:
    volatile digital_t switches;

public:
    DigitalInput();
    
    void init();
    
    static digital_t getSwitches();
};

#endif
