
#include "UserInterface.h"
#include "DisplayBox.h"

extern DisplayBox *displayBox;

void UserInterface::init() {

    screen = SCREEN_INIT;
    lcd = displayBox->getLCD();
    lcd->clear();
    lcd->setFontSize( 2);
    lcd->println("Booting...");
}

void UserInterface::handle() {

    switch (screen)
    {
    case SCREEN_INIT:
        switchScreen( SCREEN_HOME);
        break;
    
    case SCREEN_HOME:
        homeScreen();
        break;

    case SCREEN_SELECT:
        selectScreen();
        break;

    case SCREEN_CONFIG:
        configScreen();
        break;

    default:
        break;
    }
}

void UserInterface::homeScreen() {

    if( refresh = REFRESH_FULL) {
        lcd->clear();
        lcd->print( TEXT_TXOS);
        lcd->println( TXOS_VERSION);
        refresh = REFRESH_UPDATE;
    }

    if( refresh = REFRESH_UPDATE) {

        refresh = REFRESH_OK;
    }

    if( displayBox->getKey() == KEY_ENTER) {
        switchScreen( SCREEN_SELECT);
    }
}

void UserInterface::selectScreen() {

    if( refresh = REFRESH_FULL) {
        lcd->clear();
        refresh = REFRESH_UPDATE;
    }

    if( refresh = REFRESH_UPDATE) {

        refresh = REFRESH_OK;
    }

    if( displayBox->getKey() == KEY_ENTER) {
        switchScreen( SCREEN_HOME);
    }
}

void UserInterface::configScreen() {

}

void UserInterface::switchScreen( uint8_t scr) {

    screen = scr;
    refresh = REFRESH_FULL;
}
