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

#ifndef _EEPROM_h_
#define _EEPROM_h_

#include "Arduino.h"

#define EEPROM_FILENAME  "EEPROM.DAT"

class EEPROMClass {

    private:
        uint8_t *storage;
        uint16_t len;

    public:
        EEPROMClass( uint16_t len);

        void loadFromFile();
        void saveToFile();

        uint8_t read( int idx )              { return storage[idx]; }
        void write( int idx, uint8_t val )   { storage[idx] = val; }
        void update( int idx, uint8_t val )  { storage[idx] = val; }

        uint16_t length() { return len; }
        
        template< typename T > T &get( int idx, T &t ){
            uint8_t *ptr = (uint8_t*) &t;
            for( int count = sizeof(T) ; count ; --count, ++idx )  *ptr++ = storage[idx];
            return t;
        }

        template< typename T > const T &put( int idx, const T &t ){
            const uint8_t *ptr = (const uint8_t*) &t;
            for( int count = sizeof(T) ; count ; --count, ++idx )  storage[idx] = *ptr++ ;
            return t;
        }
};

extern EEPROMClass EEPROM;

#endif