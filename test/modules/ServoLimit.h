
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

        /* From TableEditable */
        uint8_t getItemCount();
        const char *getItemName( uint8_t row);
        uint8_t getValueCount();
        void getValue( uint8_t row, uint8_t col, Cell *cell);
        void setValue( uint8_t row, uint8_t col, Cell *cell);
};

#endif
