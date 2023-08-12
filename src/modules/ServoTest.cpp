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

#include "ServoTest.h"

#define TEST_COUNT 7

const char* const testNames[TEST_COUNT] {

    "-125%",
    "-100%",
    "  off",
    "   0%",
    "+100%",
    "+125%",
    "Sweep"
};

ServoTest::ServoTest() : Module( MODULE_SERVO_TEST_TYPE, TEXT_MODULE_SERVO_TEST) {

    setDefaults();
}

/* From Module */

void ServoTest::run( Controls &controls) {

    channelValue_t v;

    if( testNo != 2) { // off

        for( channel_t ch = 0; ch < PPM_CHANNELS; ch++) {        

            switch( testNo) {
            case 0: // -125%
                v = CHANNELVALUE_MIN_LIMIT;
                break;

            case 1: // -100%
                v = CHANNELVALUE_MIN;
                break;

            case 2: // off
                v = controls.outputGet( ch);
                break;

            case 3: // 0%
                v = CHANNELVALUE_MID;
                break;

            case 4: // 100%
                v = CHANNELVALUE_MAX;
                break;

            case 5: // 125%
                v = CHANNELVALUE_MAX_LIMIT;
                break;

            case 6: // Sweep
                v = CHANNELVALUE_MID; // @todo
                break;
            }

            controls.outputSet( ch, v);
        }
    }
}

void ServoTest::setDefaults() {

    testNo = 2;
}

void ServoTest::moduleExit() {

    setDefaults();
}

/* From TableEditable */

uint8_t ServoTest::getRowCount() {

    return 1;
}

const char *ServoTest::getRowName( uint8_t row) {

    return "Test";
}

uint8_t ServoTest::getColCount( uint8_t row) {

    return 1;
}

void ServoTest::getValue( uint8_t row, uint8_t col, Cell *cell) {

    cell->setList( 5, testNames, TEST_COUNT, testNo);
}

void ServoTest::setValue( uint8_t row, uint8_t col, Cell *cell) {

    testNo = cell->getList();
}

