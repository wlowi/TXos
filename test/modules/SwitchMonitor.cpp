
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

    cell->setInt8( TEXT_SW_NAME_length +1, current.switchGet( row), 0, 3);
}

void SwitchMonitor::setValue( uint8_t row, uint8_t col, Cell *cell) {

    /* no-op */
}

bool SwitchMonitor::hasChanged( uint8_t row, uint8_t col) {

    return true;
}