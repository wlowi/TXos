
#ifndef _EngineCut_h_
#define _EngineCut_h_

#include "TXos.h"
#include "Module.h"

typedef struct engineCut_t {

    percent_t cut_pct;
    switchSet_t trigger;

} engineCut_t;

class EngineCut : public Module {

    private:
        engineCut_t cfg;

    public:
        EngineCut();
        void run( Controls &controls) final;
        void setDefaults() final;

        moduleSize_t getConfigSize() final;
        uint8_t *getConfig() final;

        /* From TableEditable */
        uint8_t getRowCount() final;
        const char *getRowName( uint8_t row) final;
        uint8_t getColCount( uint8_t row) final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;
};

#endif