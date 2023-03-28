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

#include "EngineCut.h"

EngineCut::EngineCut() : Module( MODULE_ENGINE_CUT_TYPE, TEXT_MODULE_ENGINE_CUT) {

    setDefaults();
}

bool EngineCut::isSave() {

    return save;
}

/* From Module */

void EngineCut::run( Controls &controls) {

    save = controls.evalSwitches( CFG->swState);

    if( save) {
        controls.logicalSet( CHANNEL_THROTTLE, PCT_TO_CHANNEL(CFG->cut_pct));
    }
    
}

void EngineCut::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        CFG->cut_pct = -100;
        INIT_SWITCH( CFG->swState);

    )

    save = false;
}

/* From TableEditable */
uint8_t EngineCut::getRowCount() {

    return 2;
}

const char *EngineCut::getRowName( uint8_t row) {

    if( row == 0) {
        return TEXT_SWITCH;
    } else {
        return TEXT_THR;
    }
}

uint8_t EngineCut::getColCount( uint8_t row) {

    return 1;
}

void EngineCut::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( row == 0) {
        cell->setSwitchState( 7, CFG->swState);
    } else {
        cell->setInt8( 7, CFG->cut_pct, 0, PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
    }
}

void EngineCut::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( row == 0) {
        CFG->swState = cell->getSwitchState();
    } else {
        CFG->cut_pct = cell->getInt8();
    }
}
