/*
 * ProcessChain.h
 */

#ifndef _ProcessChain_h
#define _ProcessChain_h

#include "Arduino.h"
#include "TXos.h"


#define CHANNELS           ((channel_t)9)

#define ONE_PERCENT        ((CHANNELVALUE_MAX - CHANNELVALUE_MIN) / 200)

/* Convert percentage to channel value */
#define PCT_TO_VALUE( p )  ( p * ONE_PERCENT )


/* Timings for all channels */
typedef struct channelSet_t {
    
    value_t analogChannel[ CHANNELS ];
    digital_t switches;

} channelSet_t;


/* ProcessModule interface */
class ProcessModule {
public:
    ProcessModule *next;

private:
    uint8_t moduleType;

public:
    ProcessModule( uint8_t type);

    virtual void setConfiguration( uint8_t type, uint8_t length, uint8_t *mem) = 0;
    virtual uint8_t getConfiguration( uint8_t *mem, uint8_t ptr) = 0;
    virtual void process( channelSet_t *channels) = 0;
    uint8_t getModuleType();
    
    ProcessModule *nextModule();
};


class ProcessChain {
private:
    ProcessModule *chain;

public:
    ProcessChain();
    
    void add( ProcessModule *module);
    ProcessModule *findModule( uint8_t type);
    ProcessModule *firstModule();

    void process( channelSet_t *channels);
};

#endif
