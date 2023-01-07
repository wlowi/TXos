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

#include "SwitchMonitor.h"

extern Controls controls;

SwitchMonitor::SwitchMonitor( Controls &controls) : Module( MODULE_SWITCH_MONITOR_TYPE, TEXT_MODULE_SWITCHES) , current( controls){

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