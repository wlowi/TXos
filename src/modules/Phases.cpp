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

Phases::Phases() : Module( MODULE_PHASES_TYPE, TEXT_MODULE_PHASES) {

    setDefaults();
}

phase_t Phases::getPhase() {

    return phase;
}

const char *Phases::getPhaseName() {

    return PhaseNames[CFG->phaseName[phase]];
}

/* From Module */

void Phases::run( Controls &controls) {

    switchState_t state;
    switch_t sw;

    if( IS_SWITCH_UNUSED( CFG->sw)) {
        return;
    }

    state = controls.switchGet( CFG->sw);
        
    sw = controls.getSwitchByType( SW_CONF_PHASES, 0);
    if( IS_SWITCH_USED(sw)) {
        controls.switchSet( sw, state);
    }

    sw = controls.getSwitchByType( SW_CONF_PHASE_N, state);
    if( IS_SWITCH_USED(sw)) {
        controls.switchSet( sw, SW_STATE_1);
    }

    if( state < PHASES && state != phase) {
        phase = state;

        LOGV("Phases::run: switch to phase %d\n", phase);
        moduleManager.switchPhase( phase);
    }
}

void Phases::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        INIT_SWITCH( CFG->sw);

        phase = 0;

        for( phase_t p = 0; p < PHASES; p++) {
            CFG->phaseName[p] = p;
        }

    )

    strcpy( phaseText, TEXT_PHASE_PATTERN);
}

/* From TableEditable */

uint8_t Phases::getRowCount() {

    /* One for the switch + number of phases */
    return 1 + PHASES;
}

const char *Phases::getRowName( uint8_t row) {

    if( row == 0) {
        return TEXT_SWITCH;
    }

    phaseText[3] = '0'+row;
    phaseText[4] = '\0';

    return phaseText;
}

uint8_t Phases::getColCount( uint8_t row) {

    return 1;
}

void Phases::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( row == 0) {
        cell->setSwitch( 7, CFG->sw);
    } else {
        cell->setList( 6, PhaseNames, TEXT_PHASES_count, CFG->phaseName[row-1]);
    }
}

void Phases::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( row == 0) {
        CFG->sw = cell->getSwitch();
    } else {
        CFG->phaseName[row-1] = cell->getList();
    }
}
