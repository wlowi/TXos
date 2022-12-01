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

#include "DualExpo.h"

DualExpo::DualExpo() : Module( MODULE_DUAL_EXPO_TYPE, TEXT_MODULE_DUAL_EXPO) {

    setDefaults();
}

void DualExpo::run( Controls &controls) {

}

void DualExpo::setDefaults() {

    cfg.sw = SWITCH_NONE;
}

/* From Module */

moduleSize_t DualExpo::getConfigSize() {

    return (moduleSize_t)sizeof( cfg);
}

uint8_t *DualExpo::getConfig() {

    return (uint8_t*)&cfg;
}

/* From TableEditable */

uint8_t DualExpo::getRowCount() {

    return 1;
}

const char *DualExpo::getRowName( uint8_t row) {

    return TEXT_SWITCH;
}

uint8_t DualExpo::getColCount( uint8_t row) {

    return 1;
}

void DualExpo::getValue( uint8_t row, uint8_t col, Cell *cell) {

    cell->setSwitch( 7, cfg.sw);
}

void DualExpo::setValue( uint8_t row, uint8_t col, Cell *cell) {

    cfg.sw = cell->getSwitch();
}