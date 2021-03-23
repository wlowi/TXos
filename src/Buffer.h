/*
 * Buffer.h
 */

#ifndef _Buffer_h
#define _Buffer_h

#include "Arduino.h"

class Buffer
{
public:
    Buffer( byte sz);
    
    boolean isFull();
    boolean isEmpty();
    
    void clear();
    
    byte size();
    
    boolean put( byte ch);
    boolean get( byte *ch);

private:
    byte *buffer;
    byte bufferSize;
    byte in;
    byte out;
};
 
 #endif
 
 
