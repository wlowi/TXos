/*
  TXos. A remote control transmitter OS.

  MIT License

  Copyright (c) 2023 wlowi

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
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
        void exportConfig( ImportExport *exporter, uint8_t *config) const;
        void importConfig( ImportExport *importer, uint8_t *config) const;

        /* From TableEditable */
        uint8_t getRowCount() final;
        const char *getRowName( uint8_t row) final;
        uint8_t getColCount( uint8_t row) final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;
};
#endif
