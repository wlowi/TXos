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

#include "Phases.h"

Phases::Phases() : Module( MODULE_PHASES_TYPE, TEXT_MODULE_PHASES) {

    setDefaults();
}

void Phases::run( Controls &controls) {

}

void Phases::setDefaults() {

    cfg.sw = SWITCH_NONE;
}

/* From Module */

moduleSize_t Phases::getConfigSize() {

    return (moduleSize_t)sizeof( cfg);
}

uint8_t *Phases::getConfig() {

    return (uint8_t*)&cfg;
}

/* From TableEditable */

uint8_t Phases::getRowCount() {

    return 1;
}

const char *Phases::getRowName( uint8_t row) {

    return TEXT_SWITCH;
}

uint8_t Phases::getColCount( uint8_t row) {

    return 1;
}

void Phases::getValue( uint8_t row, uint8_t col, Cell *cell) {

    cell->setSwitch( 7, cfg.sw);
}

void Phases::setValue( uint8_t row, uint8_t col, Cell *cell) {

    cfg.sw = cell->getSwitch();
}