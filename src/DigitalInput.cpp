/*
 * DigitalInput.cpp
 */

#include <util/atomic.h>

#include "DigitalInput.h"

DigitalInput digitalInput;

DigitalInput::DigitalInput() {
    
    init();
}

void DigitalInput::init() {
    
    switches = 0;
}

digital_t DigitalInput::getSwitches() {
    
    return digitalInput.switches;
}
