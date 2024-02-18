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

#include "RangeTest.h"
#include "Ports.h"

extern Ports ports;

RangeTest::RangeTest() : Module( MODULE_RANGE_TEST_TYPE, TEXT_MODULE_RANGE_TEST, COMM_SUBPACKET_NONE) {

    setDefaults();
}

void RangeTest::rangeTestOn() {

    ports.bindOn();
    rangeTestStep = RANGETEST_STEP_ACTIVE;
    changed = true;
}

void RangeTest::rangeTestOff() {

    ports.bindOff();
    rangeTestStep = RANGETEST_STEP_NONE;
    changed = true;
}

/* From Module */

void RangeTest::run( Controls &controls) {

    /* noop */
}

void RangeTest::setDefaults() {

    rangeTestStep = RANGETEST_STEP_NONE;
    changed = true;
}

void RangeTest::moduleExit() {

    if( rangeTestStep == RANGETEST_STEP_ACTIVE) {
        rangeTestOff();
    }
}

/* From TableEditable */

bool RangeTest::hasChanged( uint8_t row, uint8_t col) {

    bool ret = changed;

    changed = false;
    return ret;
}

bool RangeTest::isRowExecutable( uint8_t row) {

    return (row == 0);
}

void RangeTest::rowExecute( TextUI *ui, uint8_t row ) {

    if( rangeTestStep == RANGETEST_STEP_NONE) {
        rangeTestOn();
    } else {
        rangeTestOff();
    }
}

uint8_t RangeTest::getRowCount() {

    return 1;
}

const char *RangeTest::getRowName( uint8_t row) {

    return TEXT_TEST;
}

uint8_t RangeTest::getColCount( uint8_t row) {

    return 1;
}

void RangeTest::getValue( uint8_t row, uint8_t col, Cell *cell) {

    const char *l;

    if( rangeTestStep == RANGETEST_STEP_ACTIVE) {
        l = TEXT_ACTIVE;
    } else {
        l = TEXT_OFF;
    }

    cell->setLabel( 6, l, 6);
}

void RangeTest::setValue( uint8_t row, uint8_t col, Cell *cell) {

    /* noop */
}
