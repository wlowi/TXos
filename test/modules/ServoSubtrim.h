
#ifndef _ServoSubtrim_h_
#define _ServoSubtrim_h_

#include "TXos.h"
#include "Module.h"

typedef struct servoSubtrim_t {

        percent_t trim_pct[CHANNELS];

} servoSubtrim_t;

class ServoSubtrim : public Module {

    private:
        servoSubtrim_t cfg;

    public:
        ServoSubtrim();
        void run( channelSet_t &channels);
        void setDefaults();

        moduleSize_t getConfigSize();
        uint8_t *getConfig();
        void setConfig( uint8_t *config, moduleSize_t size);

        /* From TableEditable */
        uint8_t getItemCount();
        const char *getItemName( uint8_t row);
        uint8_t getValueCount();
        TableEditType_t getValueType( uint8_t col);
        void getValue( uint8_t row, uint8_t col, Cell *cell);
        void setValue( uint8_t row, uint8_t col, Cell *cell);
};

#endif
