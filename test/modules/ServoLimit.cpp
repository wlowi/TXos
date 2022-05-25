
#include "ServoLimit.h"

ServoLimit::ServoLimit() : Module( MODULE_SERVO_LIMIT_TYPE, TEXT_MODULE_SERVO_LIMIT) {

    setDefaults();
}

void ServoLimit::run( channelSet_t &channels) {

    channelValue_t v;

    for( channel_t ch = 0; ch < CHANNELS; ch++) {

        v = PCT_TO_CHANNEL( cfg.posLimit_pct[ch]);
        if( channels.analogChannel[ch] > v) {
            channels.analogChannel[ch] = v;
        }

        v = PCT_TO_CHANNEL( cfg.negLimit_pct[ch]);
        if( channels.analogChannel[ch] < v) {
            channels.analogChannel[ch] = v;
        }
    }
}

void ServoLimit::setDefaults() {

    for( channel_t ch=0; ch < CHANNELS; ch++) {
        cfg.posLimit_pct[ch] = PERCENT_MAX;
        cfg.negLimit_pct[ch] = PERCENT_MIN;
    }
}

moduleSize_t ServoLimit::getConfigSize() {

    return (moduleSize_t)sizeof( cfg);
}

uint8_t *ServoLimit::getConfig() {

    return (uint8_t*)&cfg;
}

/* From TableEditable */
uint8_t ServoLimit::getItemCount() {

    return CHANNELS;
}

const char *ServoLimit::getItemName( uint8_t row) {

    return ChannelNames[row];
}

uint8_t ServoLimit::getValueCount() {

    return 2;
}

void ServoLimit::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 0) {
        cell->setInt8( cfg.negLimit_pct[row]);
    } else {
        cell->setInt8( cfg.posLimit_pct[row]);
    }
}

void ServoLimit::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( col == 0) {
        cfg.negLimit_pct[row] = cell->getInt8();
    } else {
        cfg.posLimit_pct[row] = cell->getInt8();
    }

    if( cfg.negLimit_pct[row] > PERCENT_MAX) {
        cfg.negLimit_pct[row] = PERCENT_MAX;
    }

    if( cfg.negLimit_pct[row] < PERCENT_MIN) {
        cfg.negLimit_pct[row] = PERCENT_MIN;
    }

    if( cfg.posLimit_pct[row] > PERCENT_MAX) {
        cfg.posLimit_pct[row] = PERCENT_MAX;
    }

    if( cfg.posLimit_pct[row] < PERCENT_MIN) {
        cfg.posLimit_pct[row] = PERCENT_MIN;
    }

}
