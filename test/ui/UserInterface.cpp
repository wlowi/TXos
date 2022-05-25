
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

    switch (screen)
    {
    case SCREEN_INIT:
        switchScreen( SCREEN_HOME);
        break;
    
    case SCREEN_HOME:
        homeScreen( event);
        break;

    case SCREEN_SELECT:
        selectScreen( event);
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
    
        Model *model = (Model*)moduleManager.getModuleByType(MODULE_MODEL_TYPE);
        lcd->print( model->getModelName());

        refresh = REFRESH_UPDATE;
    }

    if( refresh == REFRESH_UPDATE) {

        refresh = REFRESH_OK;
    }

    if( event->key == KEY_ENTER) {
        switchScreen( SCREEN_SELECT);
    }
}

void UserInterface::selectScreen( Event *event) {

    uint8_t idx;
    uint8_t cnt;
    Module *mod;

    if( refresh == REFRESH_FULL) {
        selectList.set( &moduleManager);
        refresh = REFRESH_OK;
    }

    selectList.process( lcd, event);

    if( event->pending()) {
        LOG("UserInterface::selectScreen(): event pending %d\n", event->key);
        switch( event->key) {
            case KEY_ENTER:
                idx = selectList.current();
                if( idx == 0) {
                    switchScreen( SCREEN_HOME);
                } else {
                    module = moduleManager.getModule(idx-1);
                    LOG("UserInterface::selectScreen(): Module %s\n", module ? module->getName() : "NULL");
                    switchScreen( SCREEN_CONFIG);
                }
                break;
        }
    }
}

void UserInterface::configScreen( Event *event) {

    uint8_t idx;
    uint8_t cnt;

    if( module == NULL) {
        LOG("** UserInterface::configScreen(): No module\n",1);
        switchScreen(SCREEN_SELECT);
        return;
    }

    if( refresh == REFRESH_FULL) {
        selectList.set( module);
        refresh = REFRESH_OK;
    }

    selectList.process( lcd, event);

    if( event->pending()) {
        switch( event->key) {
            case KEY_ENTER:
                idx = selectList.current();
                if( idx == 0) {
                    switchScreen( SCREEN_SELECT);
                }
                break;
        }
    }
}

void UserInterface::switchScreen( uint8_t scr) {

    screen = scr;
    refresh = REFRESH_FULL;
    LOG("UserInterface::switchScreen(): Switch to %d\n", scr);
}
