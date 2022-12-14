
#include "Model.h"

const char *wingMixNames[WINGMIX_OPTION_NUM] = {
    TEXT_WINGMIX_1AIL,
    TEXT_WINGMIX_2AIL,
    TEXT_WINGMIX_DELTA,
    TEXT_WINGMIX_VTAIL,
    TEXT_WINGMIX_VTAIL2
};

const char *mixNames[MIX_OPTION_NUM] = {
    TEXT_MIX_AIL_RUD,
    TEXT_MIX_AIL_FLP,
    TEXT_MIX_SPL_AIL,
    TEXT_MIX_SPL_FLP,
    TEXT_MIX_SPL_ELV,
    TEXT_MIX_FLP_AIL,
    TEXT_MIX_FLP_ELV,
    TEXT_MIX_ELV_AIL,
    TEXT_MIX_ELV_FLP,
    TEXT_MIX_RUD_ELV
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
        controls.logicalSet( CHANNEL_AILERON2, -controls.logicalGet( CHANNEL_AILERON));
        break;

    case WINGMIX_DELTA:
        a1 = controls.logicalGet( CHANNEL_AILERON);
        a2 = -a1;
        a1 += controls.logicalGet( CHANNEL_ELEVATOR);
        a2 += controls.logicalGet( CHANNEL_ELEVATOR);
        controls.logicalSet( CHANNEL_AILERON, a1);
        controls.logicalSet( CHANNEL_ELEVATOR, a2);
        break;

    case WINGMIX_VTAIL2:        
        controls.logicalSet( CHANNEL_AILERON2, -controls.logicalGet( CHANNEL_AILERON));

        [[fallthrough]];

    case WINGMIX_VTAIL:
        a1 = a2 = controls.logicalGet( CHANNEL_RUDDER);
        a1 += controls.logicalGet( CHANNEL_ELEVATOR);
        a2 -= controls.logicalGet( CHANNEL_ELEVATOR);
        controls.logicalSet( CHANNEL_RUDDER, a1);
        controls.logicalSet( CHANNEL_ELEVATOR, a2);
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

        for( switch_t sw = 0; sw < MIX_OPTION_NUM; sw++) {
            INIT_SWITCH( CFG->mixSw[sw]);
            CFG->mixPct[sw] = 0;
            CFG->mixOffset[sw] = 0;
        }
    )
}

/* From TableEditable */

uint8_t Model::getRowCount() {

    return 2 + 2 * MIX_OPTION_NUM;
}

const char *Model::getRowName( uint8_t row) {

    if( row == 0) {
        return TEXT_MODEL_NAME;
    } else if (row == 1) {
        return TEXT_WINGMIX;
    } else if ((row % 2) == 0) {
        return mixNames[ (row / 2) -1];
    } else {
        return TEXT_MSG_NONE;
    }
}

uint8_t Model::getColCount( uint8_t row) {

    if( row < 2 || (row % 2) == 0) {
        return 1;
    } else {
        return 3;
    }
}

void Model::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( row == 0) {
        cell->setString( 5, CFG->modelName, MODEL_NAME_LEN);
    } else if( row == 1) {
        cell->setList( 5, wingMixNames, WINGMIX_OPTION_NUM, CFG->wingMix);
    } else if( (row % 2) == 0) {
        cell->setSwitchState( 8, CFG->mixSw[(row / 2) -1]);
    } else {
        if( col == 0) {
            cell->setInt8( 1, CFG->mixPct[(row/2) -1], 4, PERCENT_MIN, PERCENT_MAX);
        } else if (col == 1) {
            cell->setLabel( 6, TEXT_MIX_OFFSET, 3);
        } else {
            cell->setInt8( 9, CFG->mixOffset[(row/2) -1], 4, PERCENT_MIN, PERCENT_MAX);
        }
    }
}

void Model::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( row == 0) {
        // nothing to do. inplace string edit
    } else if( row == 1) {
        CFG->wingMix = cell->getList();
    } else if( (row % 2) == 0) {
        CFG->mixSw[(row / 2) -1] = cell->getSwitchState();
    } else {
        if( col == 0) {
            CFG->mixPct[(row/2) -1] = cell->getInt8();
        } else if (col == 2) {
            CFG->mixOffset[(row/2) -1] = cell->getInt8();
        }
    }
}
