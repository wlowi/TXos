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

#include "VccMonitor.h"

VccMonitor::VccMonitor() : Module( MODULE_VCC_MONITOR_TYPE, TEXT_MODULE_VCC_MONITOR) {

    setDefaults();
}

bool VccMonitor::vccHasChanged() const {

    return vcc != prevVcc;
}

float16 VccMonitor::getVcc() const {

    return vcc;
}

void VccMonitor::run( Controls &controls) {

    long v = controls.auxGet( 0);

    v = v * 500L / 1024L;
    v = v * 32L / 10L;

    vcc = (float16)v;
}

void VccMonitor::setDefaults() {

    vcc = 1200;
    prevVcc = 1200;
    cfg.warnLevel = 960;
    cfg.alertLevel = 880;
}

/* From Module */

moduleSize_t VccMonitor::getConfigSize() {

    return (moduleSize_t)sizeof( cfg);
}

uint8_t *VccMonitor::getConfig() {

    return (uint8_t*)&cfg;
}

/* From TableEditable */

uint8_t VccMonitor::getRowCount() {

    return 2;
}

const char *VccMonitor::getRowName( uint8_t row) {

    if( row == 0) {
        return TEXT_WARN;
    } else {
        return TEXT_ALERT;
    }
}

uint8_t VccMonitor::getColCount( uint8_t row) {

    return 1;
}

void VccMonitor::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( row == 0) {
        cell->setInt16( 6, cfg.warnLevel, 0, 1200);
    } else {
        cell->setInt16( 6, cfg.alertLevel, 0, 1200);
    }
}

void VccMonitor::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( row == 0) {
        cfg.warnLevel = cell->getInt16();
    } else {
        cfg.alertLevel = cell->getInt16();
    }
}
