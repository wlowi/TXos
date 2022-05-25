
#include "ModelSelect.h"
#include "SystemConfig.h"

extern ModuleManager moduleManager;
extern SystemConfig systemConfig;

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

void ModelSelect::execute( uint8_t row) {

    LOG("ModelSelect::execute( %d )\n", row);

    moduleManager.load( row +1);
    systemConfig.setModelID( row +1);
}

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

    if( moduleManager.parseModule( row +1, model) == CONFIGBLOCK_RC_OK) {
        cell->value.string = model.getModelName();
    } else {
        cell->value.string = "?";
    }

    cell->type = STRING_T;
    cell->value.size = MODEL_NAME_LEN;
}

void ModelSelect::setValue( uint8_t row, uint8_t col, Cell *cell) {

    // noop
}
