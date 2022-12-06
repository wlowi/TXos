
#include "EngineCut.h"

EngineCut::EngineCut() : Module( MODULE_ENGINE_CUT_TYPE, TEXT_MODULE_ENGINE_CUT) {

    setDefaults();
}

/* From Module */

void EngineCut::run( Controls &controls) {

    if( controls.evalSwitches( CFG->trigger) ) {
        controls.analogSet( CHANNEL_THROTTLE, PCT_TO_CHANNEL(CFG->cut_pct));
    }
}

void EngineCut::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        CFG->cut_pct = -100;
        CFG->trigger = SW_STATE_ALL_DONTCARE;

    )
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
        cell->setSwitchSetState( 7, CFG->trigger);
    } else {
        cell->setInt8( 7, CFG->cut_pct, 0, PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
    }
}

void EngineCut::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( row == 0) {
        CFG->trigger = cell->getSwitchSetState();
    } else {
        CFG->cut_pct = cell->getInt8();
    }
}