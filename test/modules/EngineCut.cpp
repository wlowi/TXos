
#include "EngineCut.h"

EngineCut::EngineCut() : Module( MODULE_ENGINE_CUT_TYPE, TEXT_MODULE_ENGINE_CUT) {

    setDefaults();
}

void EngineCut::run( Controls &controls) {

    if( controls.evalSwitches( cfg.trigger) ) {
        controls.analogSet( CHANNEL_THROTTLE, PCT_TO_CHANNEL(cfg.cut_pct));
    }
}

void EngineCut::setDefaults() {

    cfg.cut_pct = -100;
    cfg.trigger = SW_STATE_ALL_DONTCARE;
}

moduleSize_t EngineCut::getConfigSize() {

    return (moduleSize_t)sizeof( cfg);
}

uint8_t *EngineCut::getConfig() {

    return (uint8_t*)&cfg;
}

/* From TableEditable */
uint8_t EngineCut::getRowCount() {

    return 2;
}

const char *EngineCut::getRowName( uint8_t row) {

    if( row == 0) {
        return TEXT_SWITCH;
    } else {
        return TEXT_THR;
    }
}

uint8_t EngineCut::getColCount( uint8_t row) {

    return 1;
}

void EngineCut::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( row == 0) {
        cell->setSwitchSetState( 7, cfg.trigger);
    } else {
        cell->setInt8( 7, cfg.cut_pct, PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
    }
}

void EngineCut::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( row == 0) {
        cfg.trigger = cell->getSwitchSetState();
    } else {
        cfg.cut_pct = cell->getInt8();
    }
}