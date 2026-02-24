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

#include "ServoMonitor.h"

extern const char* const OutputChannelNames[PPM_CHANNELS];

ServoMonitor::ServoMonitor( Controls &controls) : Module( MODULE_SERVO_MONITOR_TYPE, TEXT_MODULE_MONITOR, COMM_SUBPACKET_NONE) , current( controls) {

}

/* From Module */

void ServoMonitor::run( Controls &controls) {

    current = controls;
}

void ServoMonitor::setDefaults() {

    /* no-op */
}

/* From TableEditable */

uint8_t ServoMonitor::getRowCount() {

    return PPM_CHANNELS;
}

const char *ServoMonitor::getRowName( uint8_t row) {

    return OutputChannelNames[row];
}

uint8_t ServoMonitor::getColCount( uint8_t row) {

    return 1;
}

void ServoMonitor::getValue( uint8_t row, uint8_t col, Cell *cell) {

    cell->setInt8( TEXT_OUT_length +1, CHANNEL_TO_PCT ( current.outputGet( row)), 0, PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
}

void ServoMonitor::setValue( uint8_t row, uint8_t col, Cell *cell) {

    /* no-op */
}

bool ServoMonitor::hasChanged( uint8_t row, uint8_t col) {

    return true;
}
