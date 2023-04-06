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

#include "Bind.h"
#include "Ports.h"

extern Ports ports;

Bind::Bind() : Module( MODULE_BIND_TYPE, TEXT_MODULE_BIND) {

    setDefaults();
}

void Bind::bindOn() {

    ports.hfOff();
    delay( 500);
    ports.bindOn();
    delay( 100);
    ports.hfOn();
    bindStep = BIND_STEP_ACTIVE;
    changed = true;
}

void Bind::bindOff() {

    ports.hfOff();
    delay( 500);
    ports.bindOff();
    delay( 100);
    ports.hfOn();
    bindStep = BIND_STEP_NONE;
    changed = true;
}

/* From Module */

void Bind::run( Controls &controls) {

    /* noop */
}

void Bind::setDefaults() {

    bindStep = BIND_STEP_NONE;
    changed = true;
}

void Bind::moduleExit() {

    if( bindStep == BIND_STEP_ACTIVE) {
        bindOff();
    }
}

/* From TableEditable */

bool Bind::hasChanged( uint8_t row, uint8_t col) {

    bool ret = changed;

    changed = false;
    return ret;
}

bool Bind::isRowExecutable( uint8_t row) {

    return (row == 0);
}

void Bind::rowExecute( TextUI *ui, uint8_t row ) {

    if( bindStep == BIND_STEP_NONE) {
        bindOn();
    } else {
        bindOff();
    }
}

uint8_t Bind::getRowCount() {

    return 1;
}

const char *Bind::getRowName( uint8_t row) {

    return TEXT_BIND;
}

uint8_t Bind::getColCount( uint8_t row) {

    return 1;
}

void Bind::getValue( uint8_t row, uint8_t col, Cell *cell) {

    const char *l;

    if( bindStep == BIND_STEP_ACTIVE) {
        l = TEXT_ACTIVE;
    } else {
        l = TEXT_OFF;
    }

    cell->setLabel( 6, l, 6);
}

void Bind::setValue( uint8_t row, uint8_t col, Cell *cell) {

    /* noop */
}
