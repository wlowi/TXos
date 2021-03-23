/*
 * ModeSelector.h
 */
 
#ifndef _ModeSelector_h
#define _ModeSelector_h

#include "Arduino.h"
#include "ProcessChain.h"

class ModeSelector : public ProcessModule {
private:
    uint8_t mode;

public:
    ModeSelector();
    void init();

    void setConfiguration( uint8_t type, uint8_t length, uint8_t *mem);
    uint8_t getConfiguration( uint8_t *mem, uint8_t ptr);

    void process( channelSet_t *channels);
};

#endif
