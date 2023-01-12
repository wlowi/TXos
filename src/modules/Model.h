/*
    TXos. A remote control transmitter OS.

    Copyright (C) 2022 Wolfgang Lohwasser

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#ifndef _Model_h_
#define _Model_h_

#include "Module.h"

/* First and last valid model ID */
#define MODEL_ID_MIN        ((configBlockID_t)1)
#define MODEL_ID_MAX        CONFIG_MODEL_COUNT

typedef uint8_t wingMix_t;

/* Make sure the defines are in the same order as the 
 * entires of wingMixNames in TXos.cpp
 */
#define WINGMIX_NORMAL      ((wingMix_t)0)
#define WINGMIX_DELTA       ((wingMix_t)1)
#define WINGMIX_VTAIL       ((wingMix_t)2)

typedef uint8_t mix_t;

/* Make sure the defines are in the same order as the 
 * entries of mixNames in TXos.cpp
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
    
    switch_t qrDiffSw;
    percent_t qrDiffPct;

    switch_t mixSw[TEXT_MIX_count];
    percent_t mixPct[TEXT_MIX_count];
    percent_t mixOffset[TEXT_MIX_count];

} model_t;

class Model : public Module {

    NON_PHASED_CONFIG( model_t)

    private:
        channelValue_t mixValue( channelValue_t v, uint8_t mix);
        void limitChannels( Controls &controls);

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
