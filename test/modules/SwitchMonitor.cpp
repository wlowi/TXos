
#include "SwitchMonitor.h"

SwitchMonitor::SwitchMonitor() : Module( MODULE_SWITCH_MONITOR_TYPE, TEXT_MODULE_SWITCHES) {

}

void SwitchMonitor::run( channelSet_t &channels) {

    last = current;
    current = channels.switches;
    conf = channels.switchSetConf;
}

void SwitchMonitor::setDefaults() {

    /* no-op */
}

moduleSize_t SwitchMonitor::getConfigSize() {

    /* no-op */
    return 0;
}

uint8_t *SwitchMonitor::getConfig() {

    /* no-op */
    return nullptr;
}

/* From TableEditable */

bool SwitchMonitor::hasChanged() {

    return last != current;
}

uint8_t SwitchMonitor::getItemCount() {

    return SWITCHES;
}

const char *SwitchMonitor::getItemName( uint8_t row) {

    switch( Controls::switchConfGet( conf, row)) {
        case SW_CONF_UNUSED:
            return "---";

        case SW_CONF_2STATE:
            return "BI";

        case SW_CONF_3STATE:
            return "TRI";

        case SW_CONF_CONTROL:
            return "LOG";
    }
}

uint8_t SwitchMonitor::getValueCount() {

    return 1;
}

void SwitchMonitor::getValue( uint8_t row, uint8_t col, Cell *cell) {

    cell->setInt8( Controls::switchGet(current, row), 0, 3);
}

void SwitchMonitor::setValue( uint8_t row, uint8_t col, Cell *cell) {

    /* no-op */
}
