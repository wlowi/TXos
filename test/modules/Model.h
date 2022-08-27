
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
        void run( Controls &controls) final;
        void setDefaults() final;
        moduleSize_t getConfigSize() final;
        uint8_t *getConfig() final;

        /* From TableEditable */
        uint8_t getItemCount() final;
        const char *getItemName( uint8_t row) final;
        uint8_t getValueCount() final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;
};
#endif
