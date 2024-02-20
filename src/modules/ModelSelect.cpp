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

#include "ModelSelect.h"
#include "SystemConfig.h"

extern ModuleManager moduleManager;
extern SystemConfig systemConfig;

const uint8_t STATE_SELECT = 0;
const uint8_t STATE_MANAGE = 1;

/* Managing function (row) numbers */
const uint8_t F_LOAD = 0;
const uint8_t F_REMOVE = 1;
const uint8_t F_COPY = 2;
const uint8_t F_PASTE = 3;

ModelSelect::ModelSelect() : Module(MODULE_MODEL_SELECT_TYPE, TEXT_MODULE_MODEL_SELECT, COMM_SUBPACKET_NONE) {

    setDefaults();
}

configBlockID_t ModelSelect::getModelID() const {

    return CFG->modelID;
}

/* From Module */

void ModelSelect::run(Controls& controls) {

    // noop
}

void ModelSelect::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        CFG->modelID = 1;

    )
}

void ModelSelect::moduleEnter() {

    state = STATE_SELECT;
    copyID = 0;
    selectedID = 0;
}

/* From TableEditable */

/*
 * Called when we select a model.
 * This will load the model configuration.
 */
void ModelSelect::rowExecute(TextUI* ui, uint8_t row) {

    LOGV("ModelSelect::execute( %d )\n", row);

    switch (state) {
    case STATE_SELECT:
        selectedID = row + 1;
        state = STATE_MANAGE;
        ui->forceRefresh( 0);
        break;

    case STATE_MANAGE:
        switch (row) {
        case F_LOAD:
            CFG->modelID = selectedID;
            moduleManager.loadModel( CFG->modelID);
            systemConfig.save();
            ui->toHome();
            break;

        case F_REMOVE:
            moduleManager.removeModel(selectedID);
            state = STATE_SELECT;
            ui->forceRefresh( 0);
            break;

        case F_COPY:
            copyID = selectedID;
            state = STATE_SELECT;
            ui->forceRefresh( 0);
            break;

        case F_PASTE:
            moduleManager.copyModel(copyID, selectedID);
            state = STATE_SELECT;
            ui->forceRefresh( 0);
            break;
        }
        break;
    }
}

uint8_t ModelSelect::getRowCount() {

    if (state == STATE_SELECT) {
        return moduleManager.getModelCount();
    }
    else {
        return copyID == 0 ? 3 : 4;
    }
}

const char* ModelSelect::getRowName(uint8_t row) {

    int8_t p = MODELNO_STRING_LEN;

    if (state == STATE_SELECT) {
        row++; // model number start at 1

        modelNo[p--] = '\0';

        for (;;) {
            modelNo[p--] = (row % 10) + '0';
            row /= 10;

            if (row == 0 || p < 0) {
                break;
            }
        }

        while (p >= 0) {
            modelNo[p--] = ' ';
        }
    }
    else {
        if (row == F_LOAD) {
            return TEXT_LOAD;
        }
        else if (row == F_REMOVE) {
            return TEXT_REMOVE;
        }
        else if (row == F_COPY) {
            return TEXT_COPY;
        }
        else if (row == F_PASTE) {
            return TEXT_PASTE;
        }
    }

    return modelNo;
}

uint8_t ModelSelect::getColCount(uint8_t row) {

    return state == STATE_SELECT ? 1 : 0;
}

void ModelSelect::getValue(uint8_t row, uint8_t col, Cell* cell) {

    /* There is only one column */

    if (moduleManager.parseModule(row + 1, model) == CONFIGBLOCK_RC_OK) {
        cell->setString(MODELNO_STRING_LEN + 1, model.getModelName(), MODEL_NAME_LEN);
    }
    else {
     /* Config block for this model is uninitialized.
      * Display model number instead of name.
      */
        cell->setString(MODELNO_STRING_LEN + 1, modelNo, 1);
    }
}

void ModelSelect::setValue(uint8_t row, uint8_t col, Cell* cell) {

    // noop
}
