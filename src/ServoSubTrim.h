/*
 * ServoSubTrim.h
 */

#ifndef _ServoSubTrim_h
#define _ServoSubTrim_h

#include "Arduino.h"
#include "ProcessChain.h"

class ServoSubTrim : public ProcessModule {
private:
    percent_t subTrimPct[ CHANNELS ];

public:
    ServoSubTrim();
    void init();

    void setConfiguration( uint8_t type, uint8_t length, uint8_t *mem);
    uint8_t getConfiguration( uint8_t *mem, uint8_t ptr);

    void process( channelSet_t *channels);
};

#endif
