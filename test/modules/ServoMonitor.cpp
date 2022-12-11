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

#include "ServoMonitor.h"

extern const char *OutputChannelNames[PPM_CHANNELS];

ServoMonitor::ServoMonitor( Controls &controls) : Module( MODULE_SERVO_MONITOR_TYPE, TEXT_MODULE_MONITOR) , current( controls) {

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

    cell->setInt16( 4, CHANNEL_TO_PCT ( current.outputGet( row)), 0, 0, PERCENT_MAX_LIMIT);
}

void ServoMonitor::setValue( uint8_t row, uint8_t col, Cell *cell) {

    /* no-op */
}

bool ServoMonitor::hasChanged( uint8_t row, uint8_t col) {

    return true;
}