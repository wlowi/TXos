
#include "DisplayImpl.h"

DisplayImpl::DisplayImpl() {

    lcd = new LcdWidget( 160, 128, 1);
}

LcdWidget *DisplayImpl::getLCD( void) {

    return lcd;
}

Event *DisplayImpl::getEvent() {

    event.key = key;
    event.count = count;

    key = KEY_NONE;
    count = 0;

    return &event;
}
