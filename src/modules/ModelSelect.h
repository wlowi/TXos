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

/*
    Select a model configuration from EEPROM.
 */

#ifndef _ModelSelect_h_
#define _ModelSelect_h_

#include "ModuleManager.h"
#include "Model.h"

#define MODELNO_STRING_LEN    (3)

typedef struct modelSelect_t {

    configBlockID_t modelID;

} modelSelect_t;

class ModelSelect : public Module {

    NON_PHASED_CONFIG( modelSelect_t)

    private:
        char modelNo[MODELNO_STRING_LEN +1]; // Temporary space for numeric model ID
        Model model;                         // Temporary to display model name
        
    public:
        ModelSelect();

        configBlockID_t getModelID() const;
        
        /* From Module */
        void run( Controls &controls) final;
        void setDefaults() final;
        COMM_RC_t exportConfig( ImportExport *exporter, uint8_t *config) const { return COMM_RC_OK; }
        COMM_RC_t importConfig( ImportExport *importer, uint8_t *config) const { return COMM_RC_OK; }

        /* From TableEditable */
        bool isRowEditable( uint8_t row) final { return false; }; // override
        bool isRowExecutable( uint8_t row) final { return true; }; // override
        void rowExecute( TextUI *ui, uint8_t row ) final; // override

        uint8_t getRowCount() final;
        const char *getRowName( uint8_t row) final;
        uint8_t getColCount( uint8_t) final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;
};

#endif
