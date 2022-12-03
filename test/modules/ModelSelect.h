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
        moduleSize_t getConfigSize() final;
        uint8_t *getConfig() final;

        /* From TableEditable */
        bool isRowEditable( uint8_t row) final { return false; }; // override
        bool isRowExecutable( uint8_t row) final { return true; }; // override
        void rowExecute( uint8_t row ) final; // override

        uint8_t getRowCount() final;
        const char *getRowName( uint8_t row) final;
        uint8_t getColCount( uint8_t) final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;
};

#endif
