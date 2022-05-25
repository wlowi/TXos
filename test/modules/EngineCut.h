
#ifndef _EngineCut_h_
#define _EngineCut_h_

#include "TXos.h"
#include "Module.h"

typedef struct engineCut_t {

    percent_t cut_pct;
    digital_t trigger;

} engineCut_t;

class EngineCut : public Module {

    private:
        engineCut_t cfg;

    public:
        EngineCut();
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