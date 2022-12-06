
#include "ServoMonitor.h"

ServoMonitor::ServoMonitor( Controls &controls) : Module( MODULE_SERVO_MONITOR_TYPE, TEXT_MODULE_MONITOR) , current( controls) {

}

/* From Module */

void ServoMonitor::run( Controls &controls) {

    current = controls;
}

void ServoMonitor::setDefaults() {

    /* no-op */
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

    cell->setInt16( 4, CHANNEL_TO_PCT ( current.analogGet( row)), 0, 0, PERCENT_MAX_LIMIT);
}

void ServoMonitor::setValue( uint8_t row, uint8_t col, Cell *cell) {

    /* no-op */
}

bool ServoMonitor::hasChanged( uint8_t row, uint8_t col) {

    return true;
}