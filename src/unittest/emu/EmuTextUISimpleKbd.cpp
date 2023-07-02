
#include "Arduino.h"
#include "TextUI.h"
#include "EmuTextUISimpleKbd.h"

EmuTextUISimpleKbd::EmuTextUISimpleKbd() : TextUIInput() {

}

bool EmuTextUISimpleKbd::pending() {

    return key != KEY_NONE;
}

void EmuTextUISimpleKbd::setEvent( Event *e) {

    e->setKeyEvent( key, count);

    key = KEY_NONE;
    count = 0;
}
