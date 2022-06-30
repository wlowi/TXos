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

#include "TXos.h"
#include "ModuleManager.h"
#include "Model.h"

#define MODELNO_STRING_LEN    (3)

class ModelSelect : public Module {

    private:
        char modelNo[MODELNO_STRING_LEN +1]; // Temporarty space for numeric model ID
        Model model;
        
    public:
        ModelSelect();

        /* From Module */
        void run( channelSet_t &channels) final;
        void setDefaults() final;
        moduleSize_t getConfigSize() final;
        uint8_t *getConfig() final;

        /* From TableEditable */
        bool isEditable() final { return false; }; // override
        bool isExecutable() final { return true; }; // override
        void execute( uint8_t row ) final; // override

        uint8_t getItemCount() final;
        const char *getItemName( uint8_t row) final;
        uint8_t getValueCount() final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;
};

#endif
