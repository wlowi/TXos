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
#include "UserInterface.h"

extern ModuleManager moduleManager;
extern UserInterface userInterface;

ModelSelect::ModelSelect() : Module( MODULE_MODEL_SELECT_TYPE, TEXT_MODULE_MODEL_SELECT) {

    setDefaults();
}

configBlockID_t ModelSelect::getModelID() const {

    return CFG->modelID;
}

/* From Module */

void ModelSelect::run( Controls &controls) {

    // noop
}

void ModelSelect::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        CFG->modelID = 1;

    )
}

/* From TableEditable */

/*
 * Called when we select a model.
 * This will load the model configuration.
 */
void ModelSelect::rowExecute( uint8_t row) {

    LOGV("ModelSelect::execute( %d )\n", row);

    CFG->modelID = row +1;
    moduleManager.loadModel( CFG->modelID);
    userInterface.toScreen( SCREEN_HOME);
}

uint8_t ModelSelect::getRowCount() {

    return CONFIG_MODEL_COUNT;
}

const char *ModelSelect::getRowName( uint8_t row) {

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

uint8_t ModelSelect::getColCount( uint8_t row) {

    return 1;
}

void ModelSelect::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( moduleManager.parseModule( row +1, model) == CONFIGBLOCK_RC_OK) {
        cell->setString( MODELNO_STRING_LEN+1, model.getModelName(), MODEL_NAME_LEN);
    } else {
        /* Config block for this model is uninitialized.
         * Display model number instead of name.
         */
        cell->setString( MODELNO_STRING_LEN+1, modelNo, 1);
    }
}

void ModelSelect::setValue( uint8_t row, uint8_t col, Cell *cell) {

    // noop
}
