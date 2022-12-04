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

/* From Module */

void RangeTest::run( Controls &controls) {

    /* noop */
}

void RangeTest::setDefaults() {

    rangeTestStep = RANGETEST_STEP_NONE;
}

/* From TableEditable */

bool RangeTest::isRowExecutable( uint8_t row) {

    return (row == 0);
}

void RangeTest::rowExecute( uint8_t row ) {

    switch( rangeTestStep) {
    case RANGETEST_STEP_NONE:
        ports.bindOn();
        rangeTestStep = RANGETEST_STEP_ACTIVE;
        break;

    case RANGETEST_STEP_ACTIVE:
    default:
        ports.bindOff();
        rangeTestStep = RANGETEST_STEP_NONE;
        break;
    }
}

uint8_t RangeTest::getRowCount() {

    return 1;
}

const char *RangeTest::getRowName( uint8_t row) {

    return "RANGE";
}

uint8_t RangeTest::getColCount( uint8_t row) {

    return 1;
}

void RangeTest::getValue( uint8_t row, uint8_t col, Cell *cell) {

    switch( rangeTestStep) {
    case RANGETEST_STEP_ACTIVE:
        strcpy( stepName, "ACTIVE");
        break;

    case RANGETEST_STEP_NONE:
        default:
        strcpy( stepName, "OFF   ");
    }

    cell->setString( 6, stepName, 6);
}

void RangeTest::setValue( uint8_t row, uint8_t col, Cell *cell) {

    /* noop */
}