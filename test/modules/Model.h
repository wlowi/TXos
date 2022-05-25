
#ifndef _Model_h_
#define _Model_h_

#include "TXos.h"
#include "Module.h"

typedef uint8_t wingMix_t;

#define WINGMIX_OPTION_NUM  ((uint8_t)5)

#define WINGMIX_1AIL        ((wingMix_t)0)
#define WINGMIX_2AIL        ((wingMix_t)1)
#define WINGMIX_DELTA       ((wingMix_t)2)
#define WINGMIX_VTAIL       ((wingMix_t)3)
#define WINGMIX_VTAIL2      ((wingMix_t)4)

typedef struct model_t {

    char modelName[MODEL_NAME_LEN +1];
    wingMix_t wingMix;

} model_t;

class Model : public Module {

    private:
        model_t cfg;

    public:
        Model();

        char *getModelName() { return cfg.modelName; }

        /* From Module */
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
