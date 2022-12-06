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

float16 VccMonitor::getVcc() const {

    return vcc;
}

bool VccMonitor::belowWarn() const {

    return vcc < CFG->warnLevel; 
}

bool VccMonitor::belowAlert() const {

    return vcc < CFG->alertLevel;
}

/* From Module */

void VccMonitor::run( Controls &controls) {

    long v = controls.auxGet( 0);

    v = v * (ADC_VOLTAGE + CFG->vccAdjust) / ADC_VCC_RESOLUTION;
    v = v * (ADC_VOLTAGE_DIVIDER_R1 + ADC_VOLTAGE_DIVIDER_R2) / ADC_VOLTAGE_DIVIDER_R2;

    vcc = (float16)v;
}

void VccMonitor::setDefaults() {

    vcc = 1200;

    INIT_NON_PHASED_CONFIGURATION(

        CFG->vccAdjust = 0;
        CFG->warnLevel = VCC_WARN_LEVEL_PER_CELL * VCC_CELLS;
        CFG->alertLevel = VCC_ALERT_LEVEL_PER_CELL * VCC_CELLS;

    )
}

/* From TableEditable */

uint8_t VccMonitor::getRowCount() {

    return 3;
}

const char *VccMonitor::getRowName( uint8_t row) {

    if( row == 0) {
        return TEXT_WARN;
    } else if( row == 1) {
        return TEXT_ALERT;
    } else {
        return TEXT_VCC_ADJUST;
    }
}

bool VccMonitor::isColEditable( uint8_t row, uint8_t col) {

    return col == 0;
}

uint8_t VccMonitor::getColCount( uint8_t row) {

    if( row == 2) {
        return 2;
    }
    return 1;
}

void VccMonitor::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( row == 0) {
        cell->setFloat16( 8, CFG->warnLevel, 5, 0, 1200);
    } else if( row == 1) {
        cell->setFloat16( 8, CFG->alertLevel, 5, 0, 1200);
    } else {
        if( col == 0) {
            cell->setInt8( 3, CFG->vccAdjust, 0, -100, 100);
        } else {
            cell->setFloat16( 8, vcc, 5, 0, 2500);
        }
    }
}

void VccMonitor::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( row == 0) {
        CFG->warnLevel = cell->getFloat16();
    } else if( row == 1) {
        CFG->alertLevel = cell->getFloat16();
    } else {
        if( col == 0) {
            CFG->vccAdjust = cell->getFloat16();
        }
    }
}
