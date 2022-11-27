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

#include "Bind.h"
#include "Ports.h"

extern Ports ports;

Bind::Bind() : Module( MODULE_BIND_TYPE, TEXT_MODULE_BIND) {

    setDefaults();
}

void Bind::run( Controls &controls) {

    /* noop */
}

void Bind::setDefaults() {

    bindStep = BIND_STEP_NONE;
}

/* From Module */

bool Bind::isRowExecutable( uint8_t row) {

    return (row == 0);
}

void Bind::rowExecute( uint8_t row ) {

    switch( bindStep) {
    case BIND_STEP_NONE:
        ports.hfOff();
        delay( 500);
        ports.bindOn();
        delay( 100);
        ports.hfOn();
        bindStep = BIND_STEP_ACTIVE;
        break;

    case BIND_STEP_ACTIVE:
    default:
        ports.hfOff();
        delay( 500);
        ports.bindOff();
        delay( 100);
        ports.hfOn();
        bindStep = BIND_STEP_NONE;
        break;
    }
}

moduleSize_t Bind::getConfigSize() {

    return 0;
}

uint8_t *Bind::getConfig() {

    return nullptr;
}

/* From TableEditable */

uint8_t Bind::getRowCount() {

    return 1;
}

const char *Bind::getRowName( uint8_t row) {

    return "BIND";
}

uint8_t Bind::getColCount( uint8_t row) {

    return 1;
}

void Bind::getValue( uint8_t row, uint8_t col, Cell *cell) {

    switch( bindStep) {
    case BIND_STEP_ACTIVE:
        strcpy( stepName, "ACTIVE");
        break;

    case BIND_STEP_NONE:
        default:
        strcpy( stepName, "OFF   ");
    }

    cell->setString( 5, stepName, 6);
}

void Bind::setValue( uint8_t row, uint8_t col, Cell *cell) {

    /* noop */
}