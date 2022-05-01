
#include "EEPROM.h"

EEPROMClass::EEPROMClass( uint16_t len) {

    this->len = len;
    storage = new uint8_t[len];

    for( int idx = 0; idx < len; idx++) {
        storage[idx] = 255;
    }
}
