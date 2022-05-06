
#ifndef _ServoLimit_h_
#define _ServoLimit_h_

#include "TXos.h"
#include "Module.h"

typedef struct servoLimit_t {

        percent_t posLimit_pct[CHANNELS];
        percent_t negLimit_pct[CHANNELS];

} servoLimit_t;

class ServoLimit : public Module {

    private:
        servoLimit_t cfg;

    public:
        ServoLimit();
        void run( channelSet_t &channels);
        void setDefaults();

        moduleSize_t getConfigSize();
        uint8_t *getConfig();
        void setConfig( uint8_t *config, moduleSize_t size);
};

#endif
