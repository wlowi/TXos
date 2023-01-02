
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