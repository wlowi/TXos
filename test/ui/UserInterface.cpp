
#include "UserInterface.h"
#include "ModuleManager.h"
#include "Model.h"
#include "SystemConfig.h"
#include "ModelSelect.h"
#include "Buzzer.h"

extern DisplayImpl *displayImpl;
extern ModuleManager moduleManager;
extern SystemConfig systemConfig;
extern ModelSelect modelSelect;
extern Buzzer buzzer;

const buzzerCmd_t SoundClear[] = {
  BUZZER_PLAY( 1),
  BUZZER_PAUSE( 1),
  BUZZER_REPEAT( 0, 2),
  BUZZER_STOP()
};

void UserInterface::init() {

    module = NULL;
    
    screen[0] = SCREEN_INIT;
    screenPtr = 0;

    lcd = displayImpl->getLCD();
    lcd->clear();
    lcd->setFontSize( 2);
    lcd->println(TEXT_BOOTING);
}

void UserInterface::handle() {

    Event *event = displayImpl->getEvent();

    if( event->key != KEY_NONE) {
        LOGV("\nUserInterface::handle(): Event %d (count=%d)\n", event->key, event->count);
    }
    
    switch (screen[screenPtr]) {
    case SCREEN_INIT:
        switchScreen( SCREEN_HOME);
        break;
    
    case SCREEN_HOME:
        homeScreen( event);
        break;

    case SCREEN_MODEL:
        menuScreen( event, moduleManager.getModelMenu());
        break;

    case SCREEN_SYSTEM:
        menuScreen( event, moduleManager.getSystemMenu());
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
        lcd->printUInt( modelSelect.getModelID(), 3);
        lcd->println();
    
        Model *model = (Model*)moduleManager.getModelMenu()->getModuleByType(MODULE_MODEL_TYPE);
        lcd->print( model->getModelName());

        refresh = REFRESH_UPDATE;
    }

    if( refresh == REFRESH_UPDATE) {

        refresh = REFRESH_OK;
    }

    switch( event->key) {
    case KEY_ENTER:
        pushScreen( SCREEN_MODEL);
        break;
    
    case KEY_CLEAR:
        buzzer.play( SoundClear);
    }
}

void UserInterface::menuScreen( Event *event, Menu *menu) {

    uint8_t idx;

    if( refresh == REFRESH_FULL) {
        selectList.set( menu, true);
        refresh = REFRESH_OK;
    }

    selectList.process( lcd, event);

    if( event->pending()) {
        LOGV("UserInterface::menuScreen(): event pending %d\n", event->key);
        switch( event->key) {
        case KEY_ENTER:
            idx = selectList.current();
            if( idx == GO_BACK) {
                popScreen();
            } else {
                module = menu->getModule(idx);
                LOGV("UserInterface::menuScreen(): Module %s\n", module ? module->getName() : "NULL");
                pushScreen( SCREEN_CONFIG);
            }
            break;
        }
    }
}

void UserInterface::configScreen( Event *event) {

    uint8_t idx;

    if( module == NULL) {
        LOG("** UserInterface::configScreen(): No module\n");
        popScreen();
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
                moduleManager.saveModel( modelSelect.getModelID());
                systemConfig.save();
                popScreen();
            }
            break;
        }
    }
}

void UserInterface::switchScreen( uint8_t scr) {

    screen[screenPtr] = scr;
    refresh = REFRESH_FULL;
    LOGV("UserInterface::switchScreen(): Switch to %d\n", screen[screenPtr]);
}

void UserInterface::pushScreen( uint8_t scr) {

    if( screenPtr < SCREEN_STACK_SIZE-1) screenPtr++;
    screen[screenPtr] = scr;
    refresh = REFRESH_FULL;
    LOGV("UserInterface::pushScreen(): Switch to %d\n", screen[screenPtr]);
}

void UserInterface::popScreen() {

    if( screenPtr > 0) screenPtr--;
    refresh = REFRESH_FULL;
    LOGV("UserInterface::popScreen(): Switch to %d\n", screen[screenPtr]);
}