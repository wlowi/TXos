
#ifndef _ModelSelect_h_
#define _ModelSelect_h_

#include "TXos.h"
#include "Module.h"
#include "ConfigBlock.h"

typedef struct modelSelect_t {

    char modelName[MODEL_NAME_LEN +1];

} modelSelect_t;

class ModelSelect : public Module {

    private:
        modelSelect_t cfg;

    public:
        ModelSelect();

        /* From Module */
        void run( channelSet_t &channels);
        void setDefaults();
        moduleSize_t getConfigSize();
        uint8_t *getConfig();

        /* From TableEditable */
        uint8_t getItemCount();
        const char *getItemName( uint8_t row);
        uint8_t getValueCount();
        TableEditType_t getValueType( uint8_t col);
        void getValue( uint8_t row, uint8_t col, Cell *cell);
        void setValue( uint8_t row, uint8_t col, Cell *cell);
};

#endif
