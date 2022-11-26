
#include "ServoMonitor.h"

ServoMonitor::ServoMonitor( Controls &controls) : Module( MODULE_SERVO_MONITOR_TYPE, TEXT_MODULE_MONITOR) , current( controls) {

}

void ServoMonitor::run( Controls &controls) {

    current = controls;
}

void ServoMonitor::setDefaults() {

    /* no-op */
}

moduleSize_t ServoMonitor::getConfigSize() {

    /* no-op */
    return 0;
}

uint8_t *ServoMonitor::getConfig() {

    /* no-op */
    return nullptr;
}

/* From TableEditable */

uint8_t ServoMonitor::getRowCount() {

    return CHANNELS;
}

const char *ServoMonitor::getRowName( uint8_t row) {

    return ChannelNames[row];
}

uint8_t ServoMonitor::getColCount( uint8_t row) {

    return 1;
}

void ServoMonitor::getValue( uint8_t row, uint8_t col, Cell *cell) {

    cell->setInt16( 4, CHANNEL_TO_PCT ( current.analogGet( row)), 0, 3);
}

void ServoMonitor::setValue( uint8_t row, uint8_t col, Cell *cell) {

    /* no-op */
}

bool ServoMonitor::hasChanged( uint8_t row, uint8_t col) {

    return true;
}