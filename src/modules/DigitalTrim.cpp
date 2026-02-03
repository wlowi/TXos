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

#include "DigitalTrim.h"
#include "ModuleManager.h"
#include "AssignInput.h"

extern ModuleManager moduleManager;

extern const char* const LogicalChannelNames[LOGICAL_CHANNELS];

/* The import/export dictionary.
 * See ImportExport.h
 */
DICTROWA(r1, COMM_DATATYPE_INTARR, COMM_FIELD_PERCENT_ARRAY, digitalTrim_t, storedTrim_pct, PORT_TRIM_INPUT_COUNT)
DICT(DigitalTrim, COMM_SUBPACKET_ANALOG_TRIM, &r1)

/* We use the same module type as the AnalogTrim module.
 * This allows us to save Analog and Digital trim values into the same model config space.
 */
DigitalTrim::DigitalTrim() : Module(MODULE_ANALOG_TRIM_TYPE, TEXT_MODULE_ANALOG_TRIM, COMM_SUBPACKET_ANALOG_TRIM) {

    setDefaults();
}

/* From Module */

COMM_RC_t DigitalTrim::exportConfig(ImportExport* exporter, uint8_t* config) const {

    return exporter->runExport(DICT_ptr(DigitalTrim), DICTROW_ptr(DigitalTrim), config, sizeof(digitalTrim_t));
}

COMM_RC_t DigitalTrim::importConfig(ImportExport* importer, uint8_t* config) const {

    return importer->runImport(DICT_ptr(DigitalTrim), DICTROW_ptr(DigitalTrim), config, sizeof(digitalTrim_t));
}

void DigitalTrim::run(Controls& controls) {

    /** @todo fix digital trim */

    channelValue_t v;

    const AssignInput* assignInput = (AssignInput*)moduleManager.getModuleByType(MODULE_SET_MODEL, MODULE_ASSIGN_INPUT_TYPE);

    for (channel_t ch = 0; ch < MIX_CHANNELS; ch++) {
        channel_t in = assignInput->getInputChannel(ch);

        if (in < PORT_TRIM_INPUT_COUNT) {
            v = controls.logicalGet(ch) + PCT_TO_CHANNEL(CFG->storedTrim_pct[in]);
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

void DigitalTrim::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        for (channel_t ch = 0; ch < PORT_TRIM_INPUT_COUNT; ch++) {
            CFG->storedTrim_pct[ch] = 0;
        }
    )
}

/* From TableEditable */

uint8_t DigitalTrim::getRowCount() {

    return PORT_TRIM_INPUT_COUNT;
}

const char* DigitalTrim::getRowName(uint8_t row) {

    return LogicalChannelNames[row];
}

uint8_t DigitalTrim::getColCount(uint8_t row) {

    return 1;
}

bool DigitalTrim::isColEditable(uint8_t row, uint8_t col) {

    return col == 0;
}

void DigitalTrim::getValue(uint8_t row, uint8_t col, Cell* cell) {

    const AssignInput* assignInput = (AssignInput*)moduleManager.getModuleByType(MODULE_SET_MODEL, MODULE_ASSIGN_INPUT_TYPE);

    if (col == 0) {
        cell->setInt8(TEXT_INPUT_length + 6, CFG->storedTrim_pct[assignInput->getInputChannel(row)], 0, PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
    }
}

void DigitalTrim::setValue(uint8_t row, uint8_t col, Cell* cell) {

    const AssignInput* assignInput = (AssignInput*)moduleManager.getModuleByType(MODULE_SET_MODEL, MODULE_ASSIGN_INPUT_TYPE);

    if (col == 0) {
        CFG->storedTrim_pct[assignInput->getInputChannel(row)] = cell->getInt8();
    }
}
