
#include "EngineCut.h"

EngineCut::EngineCut() : Module( MODULE_ENGINE_CUT_TYPE, TEXT_MODULE_ENGINE_CUT) {

    setDefaults();
}

/* From Module */

void EngineCut::run( Controls &controls) {

    if( controls.evalSwitches( CFG->swState) ) {
        controls.logicalSet( CHANNEL_THROTTLE, PCT_TO_CHANNEL(CFG->cut_pct));
    }
}

void EngineCut::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        CFG->cut_pct = -100;
        INIT_SWITCH( CFG->swState);

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
        cell->setSwitchState( 7, CFG->swState);
    } else {
        cell->setInt8( 7, CFG->cut_pct, 0, PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
    }
}

void EngineCut::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( row == 0) {
        CFG->swState = cell->getSwitchState();
    } else {
        CFG->cut_pct = cell->getInt8();
    }
}