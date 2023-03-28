/*
  TXos. A remote control transmitter OS.

  MIT License

  Copyright (c) 2023 wlowi

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
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
