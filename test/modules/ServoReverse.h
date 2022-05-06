
#ifndef _ServoReverse_h_
#define _ServoReverse_h_

#include "TXos.h"
#include "Module.h"

typedef struct servoReverse_t {

        channelBits_t revBits;

} servoReverse_t;

class ServoReverse : public Module {

    private:
        servoReverse_t cfg;

    public:
        ServoReverse();
        void run( channelSet_t &channels);
        void setDefaults();

        moduleSize_t getConfigSize();
        uint8_t *getConfig();
        void setConfig( uint8_t *config, moduleSize_t size);
};

#endif
