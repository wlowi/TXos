
#include "ServoMonitor.h"

ServoMonitor::ServoMonitor() : Module( MODULE_SERVO_MONITOR_TYPE, TEXT_MODULE_MONITOR) {

}

void ServoMonitor::run( channelSet_t &channels) {

    last = current;
    current = channels.switches;
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

bool ServoMonitor::hasChanged() {

    return last != current;
}

uint8_t ServoMonitor::getItemCount() {

    return CHANNELS;
}

const char *ServoMonitor::getItemName( uint8_t row) {

    return ChannelNames[row];
}

uint8_t ServoMonitor::getValueCount() {

    return 1;
}

void ServoMonitor::getValue( uint8_t row, uint8_t col, Cell *cell) {

    cell->setInt8( Controls::switchGet(current, row), 0, 3);
}

void ServoMonitor::setValue( uint8_t row, uint8_t col, Cell *cell) {

    /* no-op */
}