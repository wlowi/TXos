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