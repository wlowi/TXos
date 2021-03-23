/*
 * ServoLimit.h
 */

#ifndef _ServoLimit_h
#define _ServoLimit_h

#include "Arduino.h"
#include "ProcessChain.h"

class ServoLimit : public ProcessModule {
private:
    percent_t minLimitPct[ CHANNELS ];
    percent_t maxLimitPct[ CHANNELS ];

public:
    ServoLimit();
    void init();

    void setConfiguration( uint8_t type, uint8_t length, uint8_t *mem);
    uint8_t getConfiguration( uint8_t *mem, uint8_t ptr);

    void process( channelSet_t *channels);
};

#endif
