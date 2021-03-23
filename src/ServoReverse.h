/*
 * ServoReverse.h
 */

#ifndef _ServoReverse_h
#define _ServoReverse_h

#include "Arduino.h"
#include "ProcessChain.h"

class ServoReverse : public ProcessModule {
private:
    uint16_t reverseBits;

public:
    ServoReverse();
    void init();

    void setConfiguration( uint8_t type, uint8_t length, uint8_t *mem);
    uint8_t getConfiguration( uint8_t *mem, uint8_t ptr);

    void process( channelSet_t *channels);
};

#endif
