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

#include "AnalogTrim.h"
#include "ModuleManager.h"
#include "AssignInput.h"

extern ModuleManager moduleManager;

extern const char* const LogicalChannelNames[LOGICAL_CHANNELS];

/* The import/export dictionary.
 * See ImportExport.h
 */
DICTROWA(r1, COMM_DATATYPE_INTARR, COMM_FIELD_PERCENT_ARRAY, analogTrim_t, storedTrim_pct, PORT_TRIM_INPUT_COUNT)
DICT(AnalogTrim, COMM_SUBPACKET_ANALOG_TRIM, &r1)

AnalogTrim::AnalogTrim() : Module(MODULE_ANALOG_TRIM_TYPE, TEXT_MODULE_ANALOG_TRIM, COMM_SUBPACKET_ANALOG_TRIM) {

    setDefaults();
}

/* From Module */

COMM_RC_t AnalogTrim::exportConfig(ImportExport* exporter, uint8_t* config) const {

    return exporter->runExport(DICT_ptr(AnalogTrim), DICTROW_ptr(AnalogTrim), config, sizeof(analogTrim_t));
}

COMM_RC_t AnalogTrim::importConfig(ImportExport* importer, uint8_t* config) const {

    return importer->runImport(DICT_ptr(AnalogTrim), DICTROW_ptr(AnalogTrim), config, sizeof(analogTrim_t));
}

void AnalogTrim::run(Controls& controls) {

    channelValue_t v;
    percent_t currentTrim_pct;
    percent_t adj_pct;

    const AssignInput* assignInput = (AssignInput*)moduleManager.getModuleByType(MODULE_SET_MODEL, MODULE_ASSIGN_INPUT_TYPE);

    current = &controls;

    switch (execStep) {
    case ANALOGTRIM_STEP_SAVE:
        /* On enter module UI */
        for (channel_t in = 0; in < PORT_TRIM_INPUT_COUNT; in++) {
            trim_pct[in] = CHANNEL_TO_PCT(controls.trimGet(in));
        }
        execStep = ANALOGTRIM_STEP_ADJUST;
        break;

    case ANALOGTRIM_STEP_ADJUST:
        /* While in module UI */
        for (channel_t in = 0; in < PORT_TRIM_INPUT_COUNT; in++) {

            currentTrim_pct = CHANNEL_TO_PCT(controls.trimGet(in));

            if (trim_pct[in] < 0 && currentTrim_pct > trim_pct[in]) {
                adj_pct = (currentTrim_pct < 0 ? currentTrim_pct : 0) - trim_pct[in];
                trim_pct[in] += adj_pct;
                CFG->storedTrim_pct[in] -= adj_pct;

            }
            else if (trim_pct[in] > 0 && currentTrim_pct < trim_pct[in]) {
                adj_pct = trim_pct[in] - (currentTrim_pct > 0 ? currentTrim_pct : 0);
                trim_pct[in] -= adj_pct;
                CFG->storedTrim_pct[in] += adj_pct;
            }
        }
        break;

    case ANALOGTRIM_STEP_NONE:
        [[fallthrough]];

    default:
        break;
    }

    for (channel_t ch = 0; ch < MIX_CHANNELS; ch++) {
        channel_t in = assignInput->getInputChannel(ch);

        if (in < PORT_TRIM_INPUT_COUNT) {
            v = controls.logicalGet(ch) + controls.trimGet(in) + PCT_TO_CHANNEL(CFG->storedTrim_pct[in]);
            if (v < CHANNELVALUE_MIN_LIMIT) {
                v = CHANNELVALUE_MIN_LIMIT;
            }
            else if (v > CHANNELVALUE_MAX_LIMIT) {
                v = CHANNELVALUE_MAX_LIMIT;
            }
            controls.logicalSet(ch, v);
        }
    }
}

void AnalogTrim::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        for (channel_t ch = 0; ch < PORT_TRIM_INPUT_COUNT; ch++) {
            CFG->storedTrim_pct[ch] = 0;
        }
    )

        execStep = ANALOGTRIM_STEP_NONE;
}

void AnalogTrim::moduleEnter() {

    LOG("AnalogTrim::moduleEnter()\n");

    /* Save trim values on module entry. */
    execStep = ANALOGTRIM_STEP_SAVE;
}

void AnalogTrim::moduleExit() {

    LOG("AnalogTrim::moduleExit()\n");
    execStep = ANALOGTRIM_STEP_NONE;
}

/* From TableEditable */

uint8_t AnalogTrim::getRowCount() {

    return PORT_TRIM_INPUT_COUNT;
}

const char* AnalogTrim::getRowName(uint8_t row) {

    return LogicalChannelNames[row];
}

uint8_t AnalogTrim::getColCount(uint8_t row) {

    return 2;
}

bool AnalogTrim::isColEditable(uint8_t row, uint8_t col) {

    return col == 1;
}

void AnalogTrim::getValue(uint8_t row, uint8_t col, Cell* cell) {

    const AssignInput* assignInput = (AssignInput*)moduleManager.getModuleByType(MODULE_SET_MODEL, MODULE_ASSIGN_INPUT_TYPE);

    if (col == 0) {
        cell->setInt8(TEXT_INPUT_length + 1, CHANNEL_TO_PCT(current->trimGet(assignInput->getInputChannel(row))), 0, PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
    }
    else {
        cell->setInt8(TEXT_INPUT_length + 6, CFG->storedTrim_pct[assignInput->getInputChannel(row)], 0, PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
    }
}

void AnalogTrim::setValue(uint8_t row, uint8_t col, Cell* cell) {

    const AssignInput* assignInput = (AssignInput*)moduleManager.getModuleByType(MODULE_SET_MODEL, MODULE_ASSIGN_INPUT_TYPE);

    if (col == 1) {
        CFG->storedTrim_pct[assignInput->getInputChannel(row)] = cell->getInt8();
    }
}
