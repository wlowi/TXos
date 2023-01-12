/*
    TXos. A remote control transmitter OS.

    Copyright (C) 2022 Wolfgang Lohwasser

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

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
        saveToFile();
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
