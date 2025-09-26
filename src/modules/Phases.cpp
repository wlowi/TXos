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

#include "Phases.h"
#include "ModuleManager.h"

extern ModuleManager moduleManager;

extern const char* const PhaseNames[TEXT_PHASES_count];

/* The import/export dictionary.
 * See ImportExport.h
 */
DICTROWA(r1, COMM_DATATYPE_UINTARR, COMM_FIELD_SWITCH, phases_t, swState, PHASES)
DICTROWA(r2, COMM_DATATYPE_UINTARR, COMM_FIELD_PHASE_NAMES, phases_t, phaseName, PHASES)
DICT(Phases, COMM_SUBPACKET_PHASES, &r1, &r2)

Phases::Phases() : Module(MODULE_PHASES_TYPE, TEXT_MODULE_PHASES, COMM_SUBPACKET_PHASES) {

    setDefaults();
}

phase_t Phases::getPhaseNo() {

    return phase;
}

phase_t Phases::getPhaseType() {

    return CFG->phaseName[phase];
}

const char* Phases::getPhaseName() {

    return PhaseNames[CFG->phaseName[phase]];
}

/* From Module */

COMM_RC_t Phases::exportConfig(ImportExport* exporter, uint8_t* config) const {

    return exporter->runExport(DICT_ptr(Phases), DICTROW_ptr(Phases), config, sizeof(phases_t));
}

COMM_RC_t Phases::importConfig(ImportExport* importer, uint8_t* config) const {

    return importer->runImport(DICT_ptr(Phases), DICTROW_ptr(Phases), config, sizeof(phases_t));
}

void Phases::run(Controls& controls) {

    uint8_t i;

    switchState_t state;
    switch_t phaseSw;

    /* Phase 0 is the default phase and active if no other phase is active.
     * We scan phases from highest number to lowest.
     */
    for (i = PHASES - 1; i > 0; i--) {
        if (IS_SWITCH_USED(CFG->swState[i])) {
            state = controls.switchGet(GET_SWITCH(CFG->swState[i]));
            if (state == GET_SWITCH_STATE(CFG->swState[i])) {
                break;
            }
        }
    }

    // Update the phases switch
    phaseSw = controls.getSwitchByType(SW_CONF_PHASES, 0);
    if (IS_SWITCH_USED(phaseSw)) {
        controls.switchSet(phaseSw, (switchState_t)i);
    }

    if (i != phase) { // The phase has changed
        phase = i;
        LOGV("Phases::run: switch to phase %d\n", i);
        moduleManager.switchPhase(i);
    }
}

void Phases::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        phase = 0;

        for (phase_t p = 0; p < PHASES; p++) {
            INIT_SWITCH(CFG->swState[p]);
            CFG->phaseName[p] = p;
        }

    )
}

/* From TableEditable */

uint8_t Phases::getRowCount() {

    return 2 * PHASES;
}

const char* Phases::getRowName(uint8_t row) {

    if (row == 0) {
        return TEXT_DEFAULT;
    } else if ((row % 2) == 0) {
        return TEXT_SWITCH;
    }

    phaseText[0] = ' ';
    phaseText[1] = '0' + (row / 2);
    phaseText[2] = '\0';

    return phaseText;
}

uint8_t Phases::getColCount(uint8_t row) {

    if( row == 0) {
        return 0;
    }

    return 1;
}

void Phases::getValue(uint8_t row, uint8_t col, Cell* cell) {

    if (row % 2) {
        cell->setList(3, PhaseNames, TEXT_PHASES_count, CFG->phaseName[row / 2]);
    } else {
        cell->setSwitchState(7, CFG->swState[row / 2]);
    }
}

void Phases::setValue(uint8_t row, uint8_t col, Cell* cell) {

    if (row % 2) {
        CFG->phaseName[row / 2] = cell->getList();
    }
    else {
        CFG->swState[row / 2] = cell->getSwitchState();
    }
}
