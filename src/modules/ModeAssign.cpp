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

#include "ModeAssign.h"

extern const char* const InputChannelNames[INPUT_CHANNELS];
extern const char* const LogicalChannelNames[LOGICAL_CHANNELS];

ModeAssign::ModeAssign() : Module( MODULE_MODE_ASSIGN_TYPE, TEXT_MODULE_MODE_ASSIGN) {

    setDefaults();
}

channel_t ModeAssign::getModeChannel( channel_t ch) const {

    return CFG->source[ch];
}

/* From Module */

void ModeAssign::exportConfig( Comm *exporter, uint8_t *config, moduleSize_t configSz) const {

    const modeAssign_t *cfg = (modeAssign_t*)config;

    exporter->openSub( 'M', MODULE_MODE_ASSIGN_TYPE);
    exporter->addUIntArr( 'C', (const byte*)cfg->source, sizeof(cfg->source), MODE_CHANNELS);
    exporter->close();
}

void ModeAssign::run( Controls &controls) {

    /* noop */
}

void ModeAssign::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        for( channel_t ch = 0; ch < MODE_CHANNELS; ch++) {
            CFG->source[ch] = ch;
        }
    )
}

/* From TableEditable */

uint8_t ModeAssign::getRowCount() {

    return MODE_CHANNELS;
}

const char *ModeAssign::getRowName( uint8_t row) {

    return LogicalChannelNames[row];
}

uint8_t ModeAssign::getColCount( uint8_t row) {

    return 2;
}

void ModeAssign::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 0) {
        cell->setLabel( TEXT_CONTROL_length +1, TEXT_ARROW_LEFT, 1);
    } else if( col == 1) {
        cell->setList( TEXT_CONTROL_length +3, InputChannelNames, MODE_CHANNELS, CFG->source[row]);
    }
}

void ModeAssign::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 1) {
        CFG->source[row] = cell->getList();
    }
}
