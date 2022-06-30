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

#include "ModelSelect.h"
#include "SystemConfig.h"

extern ModuleManager moduleManager;
extern SystemConfig systemConfig;

ModelSelect::ModelSelect() : Module( MODULE_MODEL_SELECT_TYPE, TEXT_MODULE_MODEL_SELECT) {

    setDefaults();
}

/* From Module */

void ModelSelect::run( channelSet_t &channels) {

    // noop
}

void ModelSelect::setDefaults() {

    // noop
}

/* From Module */

moduleSize_t ModelSelect::getConfigSize() {

    return 0;
}

uint8_t *ModelSelect::getConfig() {

    return nullptr;
}

/* From TableEditable */

/*
 * Called when we select a model.
 * This will load the model configuration.
 */
void ModelSelect::execute( uint8_t row) {

    LOGV("ModelSelect::execute( %d )\n", row);

    moduleManager.load( row +1);
    systemConfig.setModelID( row +1);
}

uint8_t ModelSelect::getItemCount() {

    return CONFIG_MODEL_COUNT;
}

const char *ModelSelect::getItemName( uint8_t row) {

    int8_t p = MODELNO_STRING_LEN;

    row++; // model number start at 1

    modelNo[p--] = '\0';

   	for(;;) {
   		modelNo[p--] = (row % 10) + '0';
        row /= 10;

      	if( row == 0 || p < 0) {
        	break;
      	}
    }

    while( p >= 0) {
        modelNo[p--] = ' ';
    }

    return modelNo;
}

uint8_t ModelSelect::getValueCount() {

    return 1;
}

void ModelSelect::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( moduleManager.parseModule( row +1, model) == CONFIGBLOCK_RC_OK) {
        cell->setString( model.getModelName(), MODEL_NAME_LEN);
    } else {
        /* Config block for this model is uninitialized.
         * Display model number instead of name.
         */
        cell->setString( modelNo, 1);
    }
}

void ModelSelect::setValue( uint8_t row, uint8_t col, Cell *cell) {

    // noop
}
