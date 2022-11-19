
#include "UserInterface.h"
#include "ModuleManager.h"
#include "Model.h"
#include "SystemConfig.h"

extern DisplayImpl *displayImpl;
extern ModuleManager moduleManager;
extern SystemConfig systemConfig;

void UserInterface::init() {

    module = NULL;
    screen = SCREEN_INIT;
    lcd = displayImpl->getLCD();
    lcd->clear();
    lcd->setFontSize( 2);
    lcd->println(TEXT_BOOTING);
}

void UserInterface::handle() {

    Event *event = displayImpl->getEvent();

    switch (screen) {
    case SCREEN_INIT:
        switchToScreen( SCREEN_HOME);
        break;
    
    case SCREEN_HOME:
        homeScreen( event);
        break;

    case SCREEN_MODEL:
        modelScreen( event);
        break;

    case SCREEN_SYSTEM:
        systemScreen( event);
        break;

    case SCREEN_CONFIG:
        configScreen( event);
        break;

    default:
        break;
    }
}

void UserInterface::homeScreen( Event *event) {

    if( refresh == REFRESH_FULL) {
        lcd->setBg(0,0,0);
        lcd->setFg(255,255,255);
        lcd->clear();

        lcd->print( TEXT_TXOS);
        lcd->println( TXOS_VERSION);

        lcd->print( TEXT_MODEL);
        lcd->printUInt( systemConfig.getModelID(), 3);
        lcd->println();
    
        Model *model = (Model*)moduleManager.getModelMenu()->getModuleByType(MODULE_MODEL_TYPE);
        lcd->print( model->getModelName());

        refresh = REFRESH_UPDATE;
    }

    if( refresh == REFRESH_UPDATE) {

        refresh = REFRESH_OK;
    }

    if( event->key == KEY_ENTER) {
        switchToScreen( SCREEN_MODEL);
    }
}

void UserInterface::modelScreen( Event *event) {

    uint8_t idx;

    if( refresh == REFRESH_FULL) {
        selectList.set( moduleManager.getModelMenu(), true);
        refresh = REFRESH_OK;
    }

    selectList.process( lcd, event);

    if( event->pending()) {
        LOGV("UserInterface::modelScreen(): event pending %d\n", event->key);
        switch( event->key) {
        case KEY_ENTER:
            idx = selectList.current();
            if( idx == GO_BACK) {
                switchToScreen( SCREEN_HOME);
            } else {
                module = moduleManager.getModelMenu()->getModule(idx);
                LOGV("UserInterface::modelScreen(): Module %s\n", module ? module->getName() : "NULL");
                switchToScreen( SCREEN_CONFIG);
            }
            break;
        }
    }
}

void UserInterface::systemScreen( Event *event) {

    uint8_t idx;

    if( refresh == REFRESH_FULL) {
        selectList.set( moduleManager.getSystemMenu(), true);
        refresh = REFRESH_OK;
    }

    selectList.process( lcd, event);

    if( event->pending()) {
        LOGV("UserInterface::systemScreen(): event pending %d\n", event->key);
        switch( event->key) {
        case KEY_ENTER:
            idx = selectList.current();
            if( idx == GO_BACK) {
                switchToScreen( SCREEN_MODEL);
            } else {
                module = moduleManager.getSystemMenu()->getModule(idx);
                LOGV("UserInterface::systemScreen(): Module %s\n", module ? module->getName() : "NULL");
                switchToScreen( SCREEN_CONFIG);
            }
            break;
        }
    }
}

void UserInterface::configScreen( Event *event) {

    uint8_t idx;

    if( module == NULL) {
        LOG("** UserInterface::configScreen(): No module\n");
        switchToScreen(SCREEN_HOME);
        return;
    }

    if( refresh == REFRESH_FULL) {
        selectList.set( module, true);
        refresh = REFRESH_OK;
    }

    selectList.process( lcd, event);

    if( event->pending()) {
        switch( event->key) {
        case KEY_ENTER:
            idx = selectList.current();
            if( idx == GO_BACK) {
                moduleManager.saveModel( systemConfig.getModelID());
                switchToScreen( SCREEN_HOME);
            }
            break;
        }
    }
}

void UserInterface::switchToScreen( uint8_t scr) {

    screen = scr;
    refresh = REFRESH_FULL;
    LOGV("UserInterface::switchScreen(): Switch to %d\n", scr);
}
