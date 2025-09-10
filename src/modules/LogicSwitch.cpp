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

#include "LogicSwitch.h"

extern Controls controls;

extern const char* const LogicTypes[TEXT_LOGIC_SW_TYPE_count];

const char* const InvTypes[2] = {
    TEXT_LOGIC_IS, TEXT_LOGIC_IS_NOT
};


/* The import/export dictionary.
 * See ImportExport.h
 */
DICTROWA(r1, COMM_DATATYPE_UINTARR, COMM_FIELD_SWITCH_ARRAY, logicSwitch_t, type, LOGIC_SWITCHES)
DICTROWA(r2, COMM_DATATYPE_UINTARR, COMM_FIELD_STATEA_ARRAY, logicSwitch_t, swStateA, LOGIC_SWITCHES)
DICTROWA(r3, COMM_DATATYPE_UINTARR, COMM_FIELD_STATEB_ARRAY, logicSwitch_t, swStateB, LOGIC_SWITCHES)
DICTROWA(r4, COMM_DATATYPE_UINTARR, COMM_FIELD_STATEC_ARRAY, logicSwitch_t, swStateC, LOGIC_SWITCHES)
DICTROWA(r5, COMM_DATATYPE_UINTARR, COMM_FIELD_BITS, logicSwitch_t, invert, LOGIC_SWITCHES)
DICT(LogicSwitch, COMM_SUBPACKET_LOGIC_SWITCH, &r1, &r2, &r3, &r4, &r5)

LogicSwitch::LogicSwitch() : Module(MODULE_LOGIC_SWITCH_TYPE, TEXT_MODULE_LOGIC_SWITCH, COMM_SUBPACKET_LOGIC_SWITCH) {

    setDefaults();
}

/* From Module */

COMM_RC_t LogicSwitch::exportConfig(ImportExport* exporter, uint8_t* config) const {

    return exporter->runExport(DICT_ptr(LogicSwitch), DICTROW_ptr(LogicSwitch), config, sizeof(logicSwitch_t));
}

COMM_RC_t LogicSwitch::importConfig(ImportExport* importer, uint8_t* config) const {

    return importer->runImport(DICT_ptr(LogicSwitch), DICTROW_ptr(LogicSwitch), config, sizeof(logicSwitch_t));
}

void LogicSwitch::run(Controls& controls) {

    bool swA;
    bool swB;
    bool swC;
    bool result;
    switch_t logicSw;

    for (switch_t sw = 0; sw < LOGIC_SWITCHES; sw++) {
        if (IS_SWITCH_UNUSED(CFG->swStateA[sw])
            || IS_SWITCH_UNUSED(CFG->swStateB[sw])) {
            continue;
        }

        swA = controls.evalSwitches(CFG->swStateA[sw]);
        swB = controls.evalSwitches(CFG->swStateB[sw]);
        swC = controls.evalSwitches(CFG->swStateC[sw]);

        swA = (CFG->invert[sw] & 0b0001) ? !swA : swA;
        swB = (CFG->invert[sw] & 0b0010) ? !swB : swB;
        swC = (CFG->invert[sw] & 0b0100) ? !swC : swC;

        switch (CFG->type[sw]) {
        case 0: // A & B
            result = swA && swB;
            break;
        case 1: // A | B
            result = swA || swB;
            break;
        case 2: // A & B & C
            result = swA && swB && swC;
            break;
        case 3: // A | B | C
            result = swA || swB || swC;
            break;
        case 4: // (A & B) | C
            result = (swA && swB) || swC;
            break;
        case 5: // (A | B) & C
            result = (swA || swB) && swC;
            break;
        default:
            result = false;
        }

        if (result) {
            logicSw = controls.getSwitchByType(SW_CONF_LOGIC, sw);
            if (IS_SWITCH_USED(logicSw)) {
                controls.switchSet(logicSw, SW_STATE_1);
            }
        }
    }
}

void LogicSwitch::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        for (switch_t sw = 0; sw < LOGIC_SWITCHES; sw++) {
            CFG->type[sw] = 0;
            INIT_SWITCH(CFG->swStateA[sw]);
            INIT_SWITCH(CFG->swStateB[sw]);
            INIT_SWITCH(CFG->swStateC[sw]);
            CFG->invert[sw] = 0;
        }
    )
}

/* From TableEditable */

uint8_t LogicSwitch::getRowCount() {

    return 4 * LOGIC_SWITCHES;
}

const char* LogicSwitch::getRowName(uint8_t row) {

    if (row % 4) {
        return TEXT_MSG_NONE;
    }
    else {
        controls.copySwitchName(switchName, (switch_t)(row / 4) + LOGIC_SWITCHES_FIRST_IDX);
        return switchName;
    }
}

uint8_t LogicSwitch::getColCount(uint8_t row) {

    return (row % 4) ? 3 : 1;
}

void LogicSwitch::getValue(uint8_t row, uint8_t col, Cell* cell) {

    uint8_t sw = row / 4;
    uint8_t r = (row % 4);

    if (r == 0) {
        cell->setList(4, LogicTypes, TEXT_LOGIC_SW_TYPE_count, CFG->type[sw]);
    }
    else if (r == 1) {
        if (col == 0) {
            cell->setLabel(1, TEXT_LOGIC_SW_A, 1);
        }
        else if (col == 1) {
            cell->setList(3, InvTypes, 2,
                IS_BIT_SET( CFG->invert[sw], 0) ? 1 : 0);
        }
        else if (col == 2) {
            cell->setSwitchState(6, CFG->swStateA[sw]);
        }
    }
    else if (r == 2) {
        if (col == 0) {
            cell->setLabel(1, TEXT_LOGIC_SW_B, 1);
        }
        else if (col == 1) {
            cell->setList(3, InvTypes, 2,
                IS_BIT_SET( CFG->invert[sw], 1) ? 1 : 0);
        }
        else if (col == 2) {
            cell->setSwitchState(6, CFG->swStateB[sw]);
        }
    }
    else if (r == 3) {
        if (col == 0) {
            cell->setLabel(1, TEXT_LOGIC_SW_C, 1);
        }
        else if (col == 1) {
            cell->setList(3, InvTypes, 2,
                IS_BIT_SET( CFG->invert[sw], 2) ? 1 : 0);
        }
        else if (col == 2) {
            cell->setSwitchState(6, CFG->swStateC[sw]);
        }
    }
}

void LogicSwitch::setValue(uint8_t row, uint8_t col, Cell* cell) {

    uint8_t sw = row / 4;
    uint8_t r = (row % 4);

    if (r == 0) {
        CFG->type[sw] = cell->getList();
    }
    else if (r == 1) {
        if (col == 1) {
            if (cell->getList() == 0) {
                BIT_CLEAR( CFG->invert[sw], 0);
            } else {
                BIT_SET( CFG->invert[sw], 0);
            }
        } else if (col == 2) {
            CFG->swStateA[sw] = cell->getSwitchState();
        }
    }
    else if (r == 2) {
        if (col == 1) {
            if (cell->getList() == 0) {
                BIT_CLEAR( CFG->invert[sw], 1);
            } else {
                BIT_SET( CFG->invert[sw], 1);
            }
        } else if (col == 2) {
            CFG->swStateB[sw] = cell->getSwitchState();
        }
    }
    else if (r == 3) {
        if (col == 1) {
            if (cell->getList() == 0) {
                BIT_CLEAR( CFG->invert[sw], 2);
            } else {
                BIT_SET( CFG->invert[sw], 2);
            }
        } else if (col == 2) {
            CFG->swStateC[sw] = cell->getSwitchState();
        }
    }
}
