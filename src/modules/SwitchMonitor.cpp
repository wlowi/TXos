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

#include "SwitchMonitor.h"

extern Controls controls;

SwitchMonitor::SwitchMonitor( Controls &controls) : Module( MODULE_SWITCH_MONITOR_TYPE, TEXT_MODULE_SWITCHES, COMM_SUBPACKET_NONE) , current( controls){

}

/* From Module */

void SwitchMonitor::run( Controls &controls) {

    current = controls;
}

void SwitchMonitor::setDefaults() {

    /* no-op */
}

/* From TableEditable */

uint8_t SwitchMonitor::getRowCount() {

    return SWITCHES;
}

const char *SwitchMonitor::getRowName( uint8_t row) {

    controls.copySwitchName( switchName, (switch_t)row);

    return switchName;
}

uint8_t SwitchMonitor::getColCount( uint8_t row) {

    return 1;
}

void SwitchMonitor::getValue( uint8_t row, uint8_t col, Cell *cell) {

    cell->setInt8( TEXT_SW_NAME_length +1, current.switchGet( row), 1, 0, 2);
}

void SwitchMonitor::setValue( uint8_t row, uint8_t col, Cell *cell) {

    /* no-op */
}

bool SwitchMonitor::hasChanged( uint8_t row, uint8_t col) {

    return true;
}
