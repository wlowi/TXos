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

/* Run Servo tests 
 *
 * Note that the servo test module obeys servo limits 
 */

#include "ServoTest.h"
#include "ServoRemap.h"
#include "ModuleManager.h"

extern ModuleManager moduleManager;

#define TEST_COUNT 7

const uint8_t TEST_OFF = 3;

const char* const testNames[TEST_COUNT] {

    "-125%",
    "-100%",
    "   0%",
    "  off",
    "Sweep",
    "+100%",
    "+125%"

};

const channelValue_t INCREMENT = 10;

ServoTest::ServoTest() : Module( MODULE_SERVO_TEST_TYPE, TEXT_MODULE_SERVO_TEST) {

    setDefaults();
}

/* From Module */

void ServoTest::run( Controls &controls) {

    if( testNo != TEST_OFF) { // off

        if( doInit) {
            for( channel_t ch = 0; ch < PPM_CHANNELS; ch++) {
                lastV[ch] = controls.outputGet( ch);
                increment[ch] = INCREMENT;
            }
            doInit = false;
        }

        ServoRemap *remap = (ServoRemap*)moduleManager.getModuleByType( MODULE_SET_MODEL, MODULE_SERVO_REMAP_TYPE);

        for( channel_t ch = 0; ch < PPM_CHANNELS; ch++) {        

            // For safety: Do not test engine channel
            if( remap && (remap->getSourceForChannel(ch) == CHANNEL_THROTTLE)) {
                continue;
            }

            switch( testNo) {
            case 0: // -125%
                moveTo( ch, CHANNELVALUE_MIN_LIMIT);
                break;

            case 1: // -100%
                moveTo( ch, CHANNELVALUE_MIN);
                break;

            case 2: // 0%
                moveTo( ch, CHANNELVALUE_MID);
                break;

//            case 3: off

            case 4: // Sweep
                lastV[ch] += increment[ch];

                if( lastV[ch] > CHANNELVALUE_MAX) {
                    lastV[ch] = CHANNELVALUE_MAX;
                    increment[ch] = -increment[ch];
                } else if( lastV[ch] < CHANNELVALUE_MIN) {
                    lastV[ch] = CHANNELVALUE_MIN;
                    increment[ch] = -increment[ch];
                }
                break;

            case 5: // 100%
                moveTo( ch, CHANNELVALUE_MAX);
                break;

            case 6: // 125%
                moveTo( ch, CHANNELVALUE_MAX_LIMIT);
                break;
            }

            controls.outputSet( ch, lastV[ch]);
        }
    }
}

void ServoTest::moveTo( channel_t ch, channelValue_t v) {

    if (lastV[ch] < v)
    {
        lastV[ch] += INCREMENT;
        if (lastV[ch] > v)
        {
            lastV[ch] = v;
        }
    }
    else if (lastV[ch] > v)
    {
        lastV[ch] -= INCREMENT;
        if (lastV[ch] < v)
        {
            lastV[ch] = v;
        }
    }
}

void ServoTest::setDefaults() {

    testNo = TEST_OFF; // test off
    doInit = false;
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

    uint8_t newTest;

    newTest = cell->getList(); 
    if( testNo == TEST_OFF && newTest != TEST_OFF ) {
        doInit = true;
    }
    testNo = newTest;
}

