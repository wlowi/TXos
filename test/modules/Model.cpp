
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

void Model::run( channelSet_t &channels) {

    channelValue_t a1;
    channelValue_t a2;

    switch( cfg.wingMix) {
    case WINGMIX_1AIL:
        break;

    case WINGMIX_2AIL:
        channels.analogChannel[CHANNEL_AILERON2] = -channels.analogChannel[CHANNEL_AILERON];
        break;

    case WINGMIX_DELTA:
        a1 = channels.analogChannel[CHANNEL_AILERON];
        a2 = -channels.analogChannel[CHANNEL_AILERON];
        a1 += channels.analogChannel[CHANNEL_ELEVATOR];
        a2 += channels.analogChannel[CHANNEL_ELEVATOR];
        channels.analogChannel[CHANNEL_AILERON] = a1;
        channels.analogChannel[CHANNEL_ELEVATOR] = a2;
        break;

    case WINGMIX_VTAIL2:        
        channels.analogChannel[CHANNEL_AILERON2] = -channels.analogChannel[CHANNEL_AILERON];
        // fall through

    case WINGMIX_VTAIL:
        a1 = channels.analogChannel[CHANNEL_RUDDER];
        a2 = channels.analogChannel[CHANNEL_RUDDER];
        a1 += channels.analogChannel[CHANNEL_ELEVATOR];
        a2 -= channels.analogChannel[CHANNEL_ELEVATOR];
        channels.analogChannel[CHANNEL_RUDDER] = a1;
        channels.analogChannel[CHANNEL_ELEVATOR] = a2;
        break;

    default:
        // ignore
        break;
    }

    /** @TODO: limit channel values */
}

void Model::setDefaults() {

    memcpy( cfg.modelName, MODEL_NAME_DEFAULT, MODEL_NAME_LEN +1);
    cfg.wingMix = WINGMIX_1AIL;
}

moduleSize_t Model::getConfigSize() {

    return (moduleSize_t)sizeof( cfg);
}

uint8_t *Model::getConfig() {

    return (uint8_t*)&cfg;
}

/* From TableEditable */

uint8_t Model::getItemCount() {

    return 2;
}

const char *Model::getItemName( uint8_t row) {

    switch( row) {
    case 0: // model name
        return TEXT_MODEL_NAME;

    default: // wingmix type
        return TEXT_WINGMIX;
    }
}

uint8_t Model::getValueCount() {

    return 1;
}

void Model::getValue( uint8_t row, uint8_t col, Cell *cell) {

    switch( row) {
    case 0: // model name
        cell->setString( cfg.modelName, MODEL_NAME_LEN);
        break;

    default: // wingmix type
        cell->setList( wingMixNames, WINGMIX_OPTION_NUM, cfg.wingMix);
    }
}

void Model::setValue( uint8_t row, uint8_t col, Cell *cell) {

    switch( row) {
    case 0: // model name
        // nothing to do. inplace edit
        break;

    default: // wingmix type
        cfg.wingMix = cell->getList();
    }
}
