
#include "ModelSelect.h"

ModelSelect::ModelSelect() : Module( MODULE_MODEL_SELECT_TYPE, TEXT_MODULE_MODEL_SELECT) {

    setDefaults();
}

/* From Module */

void ModelSelect::run( channelSet_t &channels) {

    // noop
}

void ModelSelect::setDefaults() {

    memcpy( cfg.modelName, MODEL_NAME_DEFAULT, MODEL_NAME_LEN +1);
}

moduleSize_t ModelSelect::getConfigSize() {

    return (moduleSize_t)sizeof( cfg);
}

uint8_t *ModelSelect::getConfig() {

    return (uint8_t*)&cfg;
}

/* From TableEditable */

uint8_t ModelSelect::getItemCount() {

    return CONFIG_MODELS;
}

const char *ModelSelect::getItemName( uint8_t row) {

    return "model";
}

uint8_t ModelSelect::getValueCount() {

    return 0;
}

TableEditType_t ModelSelect::getValueType( uint8_t col) {

    return INT8_T; // Dont care
}

void ModelSelect::getValue( uint8_t row, uint8_t col, Cell *cell) {

    // noop
}

void ModelSelect::setValue( uint8_t row, uint8_t col, Cell *cell) {

    // noop
}
