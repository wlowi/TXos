
#include "ModelSelect.h"

ModelSelect::ModelSelect() : Module( MODULE_MODEL_SELECT_TYPE, TEXT_MODULE_MODEL_SELECT) {

    setDefaults();
}

/* From Module */

void ModelSelect::run( channelSet_t &channels) {

    // noop
}

void ModelSelect::setDefaults() {

    // noop
}

moduleSize_t ModelSelect::getConfigSize() {

    return 0;
}

uint8_t *ModelSelect::getConfig() {

    return NULL;
}

/* From TableEditable */

uint8_t ModelSelect::getItemCount() {

    return CONFIG_MODEL_COUNT;
}

const char *ModelSelect::getItemName( uint8_t row) {

    row++; // Start model number at 1

    if( row < 10) {
        modelNo[0] = ' ';
    } else {
        modelNo[0] = '0' + row / 10;
    }

    modelNo[1] = '0' + row % 10;
    modelNo[2] = '\0';

    return modelNo;
}

uint8_t ModelSelect::getValueCount() {

    return 1;
}

void ModelSelect::getValue( uint8_t row, uint8_t col, Cell *cell) {

    cell->type = STRING_T;
    cell->value.size = MODEL_NAME_LEN;
    cell->value.string = "model";
}

void ModelSelect::setValue( uint8_t row, uint8_t col, Cell *cell) {

    // noop
}
