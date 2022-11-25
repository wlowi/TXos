
#include "SystemSetup.h"
#include "UserInterface.h"

extern UserInterface userInterface;

SystemSetup::SystemSetup() : Module( MODULE_SYSTEM_SETUP_TYPE, TEXT_MODULE_SYSTEM_SETUP) {

}

void SystemSetup::run( Controls &controls) {

}

void SystemSetup::setDefaults() {

    /* no-op */
}

moduleSize_t SystemSetup::getConfigSize() {

    /* no-op */
    return 0;
}

uint8_t *SystemSetup::getConfig() {

    /* no-op */
    return nullptr;
}

/* From TableEditable */

void SystemSetup::moduleExecute() {

    LOG("SystemSetup::execute(): called\n");
    userInterface.switchToScreen( SCREEN_SYSTEM);
}

uint8_t SystemSetup::getRowCount() {

    return 0;
}

const char *SystemSetup::getRowName( uint8_t row) {

    return "?";
}

uint8_t SystemSetup::getColCount( uint8_t row) {

    return 0;
}

void SystemSetup::getValue( uint8_t row, uint8_t col, Cell *cell) {

    /* no-op */
}

void SystemSetup::setValue( uint8_t row, uint8_t col, Cell *cell) {

    /* no-op */
}