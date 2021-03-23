/*
 * Serial.h
 * 
 */

#ifndef _Serial_h
#define _Serial_h

#include "Arduino.h"
#include "Buffer.h"

class Serial
{
public:
    Serial( unsigned int bd);

    void printi( int v);
    void printu( unsigned int v);
    void printc( char ch);
    void prints( const char *ch);

    boolean available();
    int read();
    int getc();
};

#endif
