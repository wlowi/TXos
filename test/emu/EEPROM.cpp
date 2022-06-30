
#include "EEPROM.h"

#include <iostream>
#include <fstream>
using namespace std;

EEPROMClass::EEPROMClass( uint16_t len) {

    this->len = len;
    storage = new uint8_t[len];

    for( int idx = 0; idx < len; idx++) {
        storage[idx] = 255;
    }
}

void EEPROMClass::loadFromFile() {

    streampos size;

    ifstream file( EEPROM_FILENAME, ios::in | ios::binary | ios::ate);
  
    if (file.is_open())
    {
        size = file.tellg();

        if( size == len) {
            printf("READING FILE: %s\n", EEPROM_FILENAME);
            file.seekg (0, ios::beg);
            file.read ( (char*)storage, size);
        } else {
            printf("FILE SIZE MISMATCH: %d %d \n", (uint16_t)size, len);
        }

        file.close();
    } else {
        printf("NO FILE: %s\n", EEPROM_FILENAME);
    }
}

void EEPROMClass::saveToFile() {

    ofstream file( EEPROM_FILENAME, ios::out | ios::binary);
  
    if (file.is_open())
    {
        printf("WRITING FILE: %s\n", EEPROM_FILENAME);
        
        file.write((char*)storage, len);

        file.close();

    } else {
        printf("FAILED TO OPEN FILE: %s\n", EEPROM_FILENAME);
    }
}
