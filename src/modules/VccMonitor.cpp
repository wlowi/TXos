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

#include "VccMonitor.h"

/* The import/export dictionary. 
 * See ImportExport.h
 */
DICTROW( r1, COMM_DATATYPE_INT16, COMM_FIELD_WARN, vccMonitor_t, warnLevel)
DICTROW( r2, COMM_DATATYPE_INT16, COMM_FIELD_ALERT, vccMonitor_t, alertLevel)
DICTROW( r3, COMM_DATATYPE_INT8, COMM_FIELD_ADJUST, vccMonitor_t, vccAdjust)
DICT( VccMonitor, COMM_SUBPACKET_VCC_MONITOR, &r1, &r2, &r3)

VccMonitor::VccMonitor() : Module( MODULE_VCC_MONITOR_TYPE, TEXT_MODULE_VCC_MONITOR, COMM_SUBPACKET_VCC_MONITOR) {

    setDefaults();
}

float2 VccMonitor::getVcc() const {

    return vcc;
}

bool VccMonitor::belowWarn() const {

    return vcc < CFG->warnLevel; 
}

bool VccMonitor::belowAlert() const {

    return vcc < CFG->alertLevel;
}

/* From Module */

COMM_RC_t VccMonitor::exportConfig( ImportExport *exporter, uint8_t *config) const {

    return exporter->runExport( DICT_ptr(VccMonitor), DICTROW_ptr(VccMonitor), config, sizeof(vccMonitor_t));
}

COMM_RC_t VccMonitor::importConfig( ImportExport *importer, uint8_t *config) const {

    return importer->runImport( DICT_ptr(VccMonitor), DICTROW_ptr(VccMonitor), config, sizeof(vccMonitor_t));
}

void VccMonitor::run( Controls &controls) {

    long v = controls.auxADCGet( 0);

    v = v * (ADC_VOLTAGE + CFG->vccAdjust) / ADC_VCC_RESOLUTION;
    float2 newVcc = v * (ADC_VOLTAGE_DIVIDER_R1 + ADC_VOLTAGE_DIVIDER_R2) / ADC_VOLTAGE_DIVIDER_R2;

    /* This avoids jitter on vcc value when the voltage changes slowly. */
    if( vccUpdateImmediate || newVcc < vcc || newVcc > vcc+10) { /* 0.1V */
        vcc = newVcc;
        vccUpdateImmediate = false;
    }
}

void VccMonitor::setDefaults() {

    vcc = 1200;
    vccUpdateImmediate = true;

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
        cell->setFloat2( 8, CFG->warnLevel, 5, 0, 1200);
    } else if( row == 1) {
        cell->setFloat2( 8, CFG->alertLevel, 5, 0, 1200);
    } else {
        if( col == 0) {
            cell->setInt8( 3, CFG->vccAdjust, 0, -100, 100);
        } else {
            cell->setFloat2( 8, vcc, 5, 0, 2500);
        }
    }
}

void VccMonitor::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( row == 0) {
        CFG->warnLevel = cell->getFloat2();
    } else if( row == 1) {
        CFG->alertLevel = cell->getFloat2();
    } else {
        if( col == 0) {
            CFG->vccAdjust = cell->getInt8();
            vccUpdateImmediate = true;
        }
    }
}
