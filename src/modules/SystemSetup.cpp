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

#include "SystemSetup.h"
#include "UserInterface.h"

extern UserInterface userInterface;

SystemSetup::SystemSetup() : Module( MODULE_SYSTEM_SETUP_TYPE, TEXT_MODULE_SYSTEM_SETUP) {

}

/* From Module */

void SystemSetup::run( Controls &controls) {

}

void SystemSetup::setDefaults() {

    /* no-op */
}

/* From TableEditable */

void SystemSetup::moduleExecute() {

    LOG("SystemSetup::execute(): called\n");
    userInterface.pushScreen( SCREEN_SYSTEM);
}

uint8_t SystemSetup::getRowCount() {

    return 0;
}

const char *SystemSetup::getRowName( uint8_t row) {

    return "?";
}

uint8_t SystemSetup::getColCount( uint8_t row) {

    return 0;
}

void SystemSetup::getValue( uint8_t row, uint8_t col, Cell *cell) {

    /* no-op */
}

void SystemSetup::setValue( uint8_t row, uint8_t col, Cell *cell) {

    /* no-op */
}
