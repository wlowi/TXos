
#include "Model.h"

const char *wingMixNames[WINGMIX_OPTION_NUM] = {
    TEXT_WINGMIX_1AIL,
    TEXT_WINGMIX_2AIL,
    TEXT_WINGMIX_DELTA,
    TEXT_WINGMIX_VTAIL,
    TEXT_WINGMIX_VTAIL2
};

Model::Model() : Module( MODULE_MODEL_TYPE, TEXT_MODULE_MODEL) {

    setDefaults();
}

/* From Module */

void Model::run( Controls &controls) {

    channelValue_t a1;
    channelValue_t a2;

    switch( CFG->wingMix) {
    case WINGMIX_1AIL:
        break;

    case WINGMIX_2AIL:
        controls.analogSet( CHANNEL_AILERON2, -controls.analogGet( CHANNEL_AILERON));
        break;

    case WINGMIX_DELTA:
        a1 = controls.analogGet( CHANNEL_AILERON);
        a2 = -a1;
        a1 += controls.analogGet( CHANNEL_ELEVATOR);
        a2 += controls.analogGet( CHANNEL_ELEVATOR);
        controls.analogSet( CHANNEL_AILERON, a1);
        controls.analogSet( CHANNEL_ELEVATOR, a2);
        break;

    case WINGMIX_VTAIL2:        
        controls.analogSet( CHANNEL_AILERON2, -controls.analogGet( CHANNEL_AILERON));

        [[fallthrough]];

    case WINGMIX_VTAIL:
        a1 = a2 = controls.analogGet( CHANNEL_RUDDER);
        a1 += controls.analogGet( CHANNEL_ELEVATOR);
        a2 -= controls.analogGet( CHANNEL_ELEVATOR);
        controls.analogSet( CHANNEL_RUDDER, a1);
        controls.analogSet( CHANNEL_ELEVATOR, a2);
        break;

    default:
        // ignore
        break;
    }

    /** @TODO: limit channel values */
}

void Model::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        memcpy( CFG->modelName, MODEL_NAME_DEFAULT, MODEL_NAME_LEN +1);
        CFG->wingMix = WINGMIX_1AIL;

    )
}

/* From TableEditable */

uint8_t Model::getRowCount() {

    return 2;
}

const char *Model::getRowName( uint8_t row) {

    if( row == 0) {
        return TEXT_MODEL_NAME;
    } else {
        return TEXT_WINGMIX;
    }
}

uint8_t Model::getColCount( uint8_t row) {

    return 1;
}

void Model::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( row == 0) {
        cell->setString( 5, CFG->modelName, MODEL_NAME_LEN);
    } else {
        cell->setList( 5, wingMixNames, WINGMIX_OPTION_NUM, CFG->wingMix);
    }
}

void Model::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( row == 0) {
        // nothing to do. inplace string edit
    } else {
        CFG->wingMix = cell->getList();
    }
}
