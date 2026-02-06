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

#include "ModuleManager.h"
#include "Bind.h"
#include "Model.h"
#include "Output.h"

const char *BindModeNames[TEXT_BINDMODE_count] = {
    TEXT_BINDMODE_CPPM,
    TEXT_BINDMODE_DSM2,
    TEXT_BINDMODE_DSMX,
    TEXT_BINDMODE_CRSF,
    TEXT_BINDMODE_CRS4
};

/* Supported bind modes. A subset of BindModeNames. */
const char* BindModesSupported[TEXT_BINDMODE_count] = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr
};

extern Output output;
extern ModuleManager moduleManager;

Bind::Bind() : Module(MODULE_BIND_TYPE, TEXT_MODULE_BIND, COMM_SUBPACKET_NONE) {

    setDefaults();
}

/**
 * @brief Start bind
 *
 * Set the current bind mode in model and save the model.
 * Then activate bind mode with bind mode.
 */
void Bind::bindOn() {

    Model* model = (Model*)moduleManager.getModuleByType(MODULE_SET_MODEL, MODULE_MODEL_TYPE);
    model->setBindMode( bindModes[bindMode_idx]);
    moduleManager.saveModel( modelID);

    output.bindActivate( bindModes[bindMode_idx]);
    bindStep = BIND_STEP_ACTIVE;
    changed = true;
}

void Bind::bindOff() {

    output.bindDeactivate();
    bindStep = BIND_STEP_NONE;
    changed = true;
}

/* From Module */

void Bind::run(Controls& controls) {

    /* noop */
}

void Bind::setDefaults() {

    bindStep = BIND_STEP_NONE;
    changed = true;
}

/**
 * @brief Called when module is entered.
 *
 * Get supported bind modes from HF module.
 * Set correct model ID and current bind mode.
 */
void Bind::moduleEnter() {

    bindmode_t bindMode;

    isSupported = output.isBindSupported();
    modelID = output.getModelID();

    bindMode_count = output.getBindModeCount();
    bindModes = output.getBindModes();

    for( uint8_t i = 0; i < bindMode_count; i++) {
        BindModesSupported[i] = BindModeNames[bindModes[i]];
    }

    bindMode = output.getBindMode();
    bindMode_idx = 0;

    /* Find bindmode in supported bindmode list
     * and set correct index.
     */
    for( uint8_t i = 0; i < bindMode_count; i++) {
        if( bindMode == bindModes[i]) {
            bindMode_idx = i;
            break;
        }
    }
}

/**
 * @brief Called when module is left.
 *
 * Make sure bind mode is disabled when we leave the module.
 */
void Bind::moduleExit() {

    if (bindStep == BIND_STEP_ACTIVE) {
        bindOff();
    }
}

/* From TableEditable */

bool Bind::hasChanged(uint8_t row, uint8_t col) {

    bool ret = changed;

    changed = false;
    return ret;
}

bool Bind::isRowExecutable(uint8_t row) {

    return isSupported ? (row == 2) : false;
}

void Bind::rowExecute(TextUI* ui, uint8_t row) {

    if (row == 2) {
        if (bindStep == BIND_STEP_NONE) {
            bindOn();
        }
        else {
            bindOff();
        }
    }
}

bool Bind::isRowEditable(uint8_t row) {

    return (row == 0) && (bindMode_count > 0);
}

uint8_t Bind::getRowCount() {

    return 3;
}

const char* Bind::getRowName(uint8_t row) {

    if( row == 0) {
        return TEXT_BINDMODE;
    }
    else if (row == 1) {
        return TEXT_MODEL_ID;
    }
    else if (row == 2) {
        if (isSupported) {
            return TEXT_BIND;
        }
        else {
            return TEXT_NOT_SUPPORTED;
        }
    }

    return "";
}

uint8_t Bind::getColCount(uint8_t row) {

    return isSupported ? 1 : 0;
}

void Bind::getValue(uint8_t row, uint8_t col, Cell* cell) {

    const char* l;

    if (row == 0) {
        cell->setList(9, BindModesSupported, bindMode_count, bindMode_idx);
    }
    else if (row == 1) {
        cell->setInt8(9, modelID, 2, 0, 255);
    }
    else if (row == 2) {
        if (bindStep == BIND_STEP_ACTIVE) {
            l = TEXT_ACTIVE;
        }
        else {
            l = TEXT_OFF;
        }

        cell->setLabel(6, l, 6);
    }
}

void Bind::setValue(uint8_t row, uint8_t col, Cell* cell) {

    if( row == 0) {
        bindMode_idx = cell->getList();
    }
}
