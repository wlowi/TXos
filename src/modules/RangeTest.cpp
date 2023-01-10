/*
    TXos. A remote control transmitter OS.

    Copyright (C) 2022 Wolfgang Lohwasser

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#include "RangeTest.h"
#include "Ports.h"

extern Ports ports;

RangeTest::RangeTest() : Module( MODULE_RANGE_TEST_TYPE, TEXT_MODULE_RANGE_TEST) {

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

void RangeTest::rowExecute( uint8_t row ) {

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