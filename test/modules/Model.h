
#ifndef _Model_h_
#define _Model_h_

#include "Module.h"

/* First and last valid model ID */
#define MODEL_ID_MIN        ((configBlockID_t)1)
#define MODEL_ID_MAX        CONFIG_MODEL_COUNT

typedef uint8_t wingMix_t;

#define WINGMIX_OPTION_NUM  ((uint8_t)3)

/* Make sure the defines are in the same order as the 
 * entires of wingMixNames in Models.cpp
 */
#define WINGMIX_NORMAL      ((wingMix_t)0)
#define WINGMIX_DELTA       ((wingMix_t)1)
#define WINGMIX_VTAIL       ((wingMix_t)2)

typedef uint8_t mix_t;

#define MIX_OPTION_NUM      ((uint8_t)10)

/* Make sure the defines are in the same order as the 
 * entries of mixNames in Models.cpp
 */
#define MIX_AIL_RUD         ((mix_t)0)
#define MIX_AIL_FLP         ((mix_t)1)
#define MIX_SPL_AIL         ((mix_t)2)
#define MIX_SPL_FLP         ((mix_t)3)
#define MIX_SPL_ELV         ((mix_t)4)
#define MIX_FLP_AIL         ((mix_t)5)
#define MIX_FLP_ELV         ((mix_t)6)
#define MIX_ELV_AIL         ((mix_t)7)
#define MIX_ELV_FLP         ((mix_t)8)
#define MIX_RUD_ELV         ((mix_t)9)

typedef struct model_t {

    char modelName[MODEL_NAME_LEN +1];
    wingMix_t wingMix;

    switch_t mixSw[MIX_OPTION_NUM];
    percent_t mixPct[MIX_OPTION_NUM];
    percent_t mixOffset[MIX_OPTION_NUM];

} model_t;

class Model : public Module {

    NON_PHASED_CONFIG( model_t)

    public:
        Model();

        char *getModelName() { return CFG->modelName; }

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
