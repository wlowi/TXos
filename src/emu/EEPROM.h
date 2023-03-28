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
