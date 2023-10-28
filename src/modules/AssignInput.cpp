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
#include "AssignInput.h"
#include "ModeAssign.h"

extern ModuleManager moduleManager;

extern const char* const InputChannelNames[INPUT_CHANNELS];
extern const char* const LogicalChannelNames[LOGICAL_CHANNELS];

AssignInput::AssignInput() : Module( MODULE_ASSIGN_INPUT_TYPE, TEXT_MODULE_ASSIGN_INPUT) {

    setDefaults();
}

/* From Module */

void AssignInput::exportConfig( Comm *exporter, uint8_t *config, moduleSize_t configSz) const {

    const assignInput_t *cfg = (assignInput_t*)config;

    exporter->openSub( 'M', MODULE_ASSIGN_INPUT_TYPE);
    exporter->addUIntArr( 'C', (const byte*)cfg->source, sizeof(cfg->source), MIX_CHANNELS);
    exporter->close();
}

void AssignInput::run( Controls &controls) {

    for( channel_t ch = 0; ch < MIX_CHANNELS; ch++) {
        controls.logicalSet( ch, controls.inputGet( CFG->source[ch]));
    }
}

void AssignInput::setDefaults() {

    const ModeAssign *modeAssign = (ModeAssign*)moduleManager.getModuleByType( MODULE_SET_SYSTEM, MODULE_MODE_ASSIGN_TYPE);

    INIT_NON_PHASED_CONFIGURATION(

        for( channel_t ch = 0; ch < MIX_CHANNELS; ch++) {
            if( ch < MODE_CHANNELS) {
                CFG->source[ch] = modeAssign ? modeAssign->getModeChannel( ch) : ch;
            } else if( ch < ANALOG_CHANNELS){
                CFG->source[ch] = ch;
            } else {
                /* Assign to NO CHANNEL. */
                CFG->source[ch] = INPUT_CHANNELS -1;
            }
        }
    )
}

/* From TableEditable */

uint8_t AssignInput::getRowCount() {

    return MIX_CHANNELS;
}

const char *AssignInput::getRowName( uint8_t row) {

    return LogicalChannelNames[row];
}

uint8_t AssignInput::getColCount( uint8_t row) {

    return 2;
}

void AssignInput::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 0) {
        cell->setLabel( TEXT_CONTROL_length +1, TEXT_ARROW_LEFT, 1);
    } else if( col == 1) {
        cell->setList( TEXT_CONTROL_length +3, InputChannelNames, INPUT_CHANNELS, CFG->source[row]);
    }
}

void AssignInput::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 1) {
        CFG->source[row] = cell->getList();
    }
}
