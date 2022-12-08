
#ifndef _Model_h_
#define _Model_h_

#include "Module.h"

/* First and last valid model ID */
#define MODEL_ID_MIN        ((configBlockID_t)1)
#define MODEL_ID_MAX        CONFIG_MODEL_COUNT

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

    NON_PHASED_CONFIG( model_t)

    private:
        void adjustControlSurfaceCount();

    public:
        Model();

        uint8_t ailerons;
        uint8_t flaps;

        char *getModelName() { return CFG->modelName; }

        uint8_t getAileronCount() const { return ailerons; }
        uint8_t getFlapsCount() const { return flaps; }

        /* From Module */
        void run( Controls &controls) final;
        void setDefaults() final;

        /* From TableEditable */
        uint8_t getRowCount() final;
        const char *getRowName( uint8_t row) final;
        uint8_t getColCount( uint8_t row) final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;
};
#endif
