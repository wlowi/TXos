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

#include "Timer.h"

Timer::Timer() : Module( MODULE_TIMER_TYPE, TEXT_MODULE_TIMER) {

    setDefaults();
}

void Timer::run( Controls &controls) {

}

void Timer::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        CFG->swSetState = SW_STATE_ALL_DONTCARE;

    )
}

/* From Module */

moduleSize_t Timer::getConfigSize() {

    return (moduleSize_t)sizeof( configuration);
}

uint8_t *Timer::getConfig() {

    return (uint8_t*)&configuration;
}

/* From TableEditable */

uint8_t Timer::getRowCount() {

    return 1;
}

const char *Timer::getRowName( uint8_t row) {

    return TEXT_SWITCH;
}

uint8_t Timer::getColCount( uint8_t row) {

    return 1;
}

void Timer::getValue( uint8_t row, uint8_t col, Cell *cell) {

    cell->setSwitchSetState( 7, CFG->swSetState);
}

void Timer::setValue( uint8_t row, uint8_t col, Cell *cell) {

    CFG->swSetState = cell->getSwitchSetState();
}