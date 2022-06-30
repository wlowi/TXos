
#include "EngineCut.h"

EngineCut::EngineCut() : Module( MODULE_ENGINE_CUT_TYPE, TEXT_MODULE_ENGINE_CUT) {

    setDefaults();
}

void EngineCut::run( channelSet_t &channels) {

    if( cfg.trigger == channels.switches ) {
        channels.analogChannel[CHANNEL_THROTTLE] = PCT_TO_CHANNEL(cfg.cut_pct);
    }
}

void EngineCut::setDefaults() {

    cfg.cut_pct = -100;
    cfg.trigger = 0;
}

moduleSize_t EngineCut::getConfigSize() {

    return (moduleSize_t)sizeof( cfg);
}

uint8_t *EngineCut::getConfig() {

    return (uint8_t*)&cfg;
}

/* From TableEditable */
uint8_t EngineCut::getItemCount() {

    return 2;
}

const char *EngineCut::getItemName( uint8_t row) {

    switch( row) {
    case 0:
        return TEXT_SWITCH;

    default: 
        return TEXT_THR;
    }
}

uint8_t EngineCut::getValueCount() {

    return 1;
}

void EngineCut::getValue( uint8_t row, uint8_t col, Cell *cell) {

    switch( row) {
    case 0:
        cell->setInt8( cfg.trigger, PERCENT_MIN, PERCENT_MAX);
        break;

    default:
        cell->setInt8( cfg.cut_pct, PERCENT_MIN, PERCENT_MAX);
    }
}

void EngineCut::setValue( uint8_t row, uint8_t col, Cell *cell) {

    switch( row) {
    case 0:
        cfg.trigger = cell->getInt8();
        break;

    default:
        cfg.cut_pct = cell->getInt8();
    }
}