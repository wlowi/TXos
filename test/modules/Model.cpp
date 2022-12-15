
#include "Model.h"

/* Make sure the entries are in the same order as the 
 * defines in Models.h
 */
const char *wingMixNames[WINGMIX_OPTION_NUM] = {
    TEXT_WINGMIX_NORMAL,
    TEXT_WINGMIX_DELTA,
    TEXT_WINGMIX_VTAIL
};

/* Make sure the entries are in the same order as the 
 * defines in Models.h
 */
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

    channelValue_t ail;
    channelValue_t elv;
    channelValue_t rud;
    channelValue_t flp;
    channelValue_t spl;

    channelValue_t mixedValue;

   /* get original unmixed values */
    ail = controls.logicalGet( CHANNEL_AILERON);
    elv = controls.logicalGet( CHANNEL_ELEVATOR);
    rud = controls.logicalGet( CHANNEL_RUDDER);
    flp = controls.logicalGet( CHANNEL_FLAP);
    spl = controls.logicalGet( CHANNEL_SPOILER);

    controls.logicalSet( CHANNEL_FLAP2, controls.logicalGet( CHANNEL_FLAP));
    controls.logicalSet( CHANNEL_SPOILER2, controls.logicalGet( CHANNEL_SPOILER));

    switch( CFG->wingMix) {
    case WINGMIX_NORMAL:
        controls.logicalSet( CHANNEL_AILERON2, -ail);
        break;

    case WINGMIX_DELTA:
        controls.logicalSet( CHANNEL_AILERON, elv + ail);
        controls.logicalSet( CHANNEL_ELEVATOR, elv - ail);
        break;

    case WINGMIX_VTAIL:
        controls.logicalSet( CHANNEL_AILERON2, -ail);
        controls.logicalSet( CHANNEL_RUDDER, rud + elv);
        controls.logicalSet( CHANNEL_ELEVATOR, rud - elv);
        break;

    default:
        // ignore
        break;
    }

    for( channel_t ch = 0; ch < LOGICAL_CHANNELS; ch++) {
        channelValue_t v = controls.logicalGet( ch);
        if( v > CHANNELVALUE_MAX_LIMIT) {
            controls.logicalSet( ch, CHANNELVALUE_MAX_LIMIT);
            continue;
        }
        if( v < CHANNELVALUE_MIN_LIMIT ) {
            controls.logicalSet( ch, CHANNELVALUE_MIN_LIMIT);
        }
    }

    for( uint8_t mix = 0; mix < MIX_OPTION_NUM; mix++) {

        if( controls.evalSwitches( CFG->mixSw[mix])) {

#define MIX_VALUE( c ) (channelValue_t)( ((long)(c) - (long)CFG->mixOffset[mix]) * (long)CFG->mixPct[mix] / 100L)

            switch( mix) {
            case MIX_AIL_RUD:
                mixedValue = MIX_VALUE( ail );
                controls.logicalSet( CHANNEL_RUDDER, controls.logicalGet( CHANNEL_RUDDER) + mixedValue);
                break;

            case MIX_AIL_FLP:
                mixedValue = MIX_VALUE( ail );
                controls.logicalSet( CHANNEL_FLAP, controls.logicalGet( CHANNEL_FLAP) + mixedValue);
                controls.logicalSet( CHANNEL_FLAP2, controls.logicalGet( CHANNEL_FLAP2) + mixedValue);
                break;

            case MIX_SPL_AIL:
                mixedValue = MIX_VALUE( spl );
                controls.logicalSet( CHANNEL_AILERON, controls.logicalGet( CHANNEL_AILERON) + mixedValue);
                controls.logicalSet( CHANNEL_AILERON2, controls.logicalGet( CHANNEL_AILERON2) + mixedValue);
                break;

            case MIX_SPL_FLP:
                mixedValue = MIX_VALUE( spl );
                controls.logicalSet( CHANNEL_FLAP, controls.logicalGet( CHANNEL_FLAP) + mixedValue);
                controls.logicalSet( CHANNEL_FLAP2, controls.logicalGet( CHANNEL_FLAP2) + mixedValue);
                break;

            case MIX_SPL_ELV:
                mixedValue = MIX_VALUE( spl );
                controls.logicalSet( CHANNEL_ELEVATOR, controls.logicalGet( CHANNEL_ELEVATOR) + mixedValue);
                break;

            case MIX_FLP_AIL:
                mixedValue = MIX_VALUE( flp );
                controls.logicalSet( CHANNEL_AILERON, controls.logicalGet( CHANNEL_AILERON) + mixedValue);
                controls.logicalSet( CHANNEL_AILERON2, controls.logicalGet( CHANNEL_AILERON2) + mixedValue);
                break;

            case MIX_FLP_ELV:
                mixedValue = MIX_VALUE( flp );
                controls.logicalSet( CHANNEL_ELEVATOR, controls.logicalGet( CHANNEL_ELEVATOR) + mixedValue);
                break;

            case MIX_ELV_AIL:
                mixedValue = MIX_VALUE( elv );
                controls.logicalSet( CHANNEL_AILERON, controls.logicalGet( CHANNEL_AILERON) + mixedValue);
                controls.logicalSet( CHANNEL_AILERON2, controls.logicalGet( CHANNEL_AILERON2) + mixedValue);
                break;

            case MIX_ELV_FLP:
                mixedValue = MIX_VALUE( elv );
                controls.logicalSet( CHANNEL_FLAP, controls.logicalGet( CHANNEL_FLAP) + mixedValue);
                controls.logicalSet( CHANNEL_FLAP2, controls.logicalGet( CHANNEL_FLAP2) + mixedValue);
                break;

            case MIX_RUD_ELV:
                mixedValue = MIX_VALUE( abs(rud) );
                controls.logicalSet( CHANNEL_ELEVATOR, controls.logicalGet( CHANNEL_ELEVATOR) + mixedValue);
                break;

            default:
                // ignore
                break;
            }
        }
    }

    for( channel_t ch = 0; ch < LOGICAL_CHANNELS; ch++) {
        channelValue_t v = controls.logicalGet( ch);
        if( v > CHANNELVALUE_MAX_LIMIT) {
            controls.logicalSet( ch, CHANNELVALUE_MAX_LIMIT);
            continue;
        }
        if( v < CHANNELVALUE_MIN_LIMIT ) {
            controls.logicalSet( ch, CHANNELVALUE_MIN_LIMIT);
        }
    }
}

void Model::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        memcpy( CFG->modelName, MODEL_NAME_DEFAULT, MODEL_NAME_LEN +1);
        CFG->wingMix = WINGMIX_NORMAL;

        for( uint8_t mix = 0; mix < MIX_OPTION_NUM; mix++) {
            INIT_SWITCH( CFG->mixSw[mix]);
            CFG->mixPct[mix] = 0;
            CFG->mixOffset[mix] = 0;
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
