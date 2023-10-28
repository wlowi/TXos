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

#include "Comm.h"

Comm::Comm() {

    textBuffer[0] = '\0';
}

bool Comm::ensureSpace( uint16_t bytes) const {

    // +2 because every line is terminated by newline + null
    return ( (idx + bytes +2) <= COMM_TEXT_MAXLEN);
}

void Comm::bufferChar( char ch) {

    // @TODO Escape character
    
    // Rotate
    uint16_t h = checksum & 0x8000;
    checksum <<= 1;
    if( h) {
        checksum |= 1;
    }

    checksum ^= (uint16_t)ch;
    textBuffer[idx++] = ch;
}

void Comm::bufferString( const char *s) {

    while( *s ) {
        bufferChar(*s);
        s++;
    }
}

void Comm::terminateLine() {

    textBuffer[idx++] = '\n';
    textBuffer[idx] = '\0';
}

void Comm::write() {

    writePart();
    subLevel = 0;
    checksum = 0;
}

void Comm::writePart() {

    if( idx > 0) {
#ifdef ARDUINO
    Serial.write( textBuffer );
    Serial.flush();
#else
    LOGV("Comm::write() Sent %ld bytes:\n%s", strlen(textBuffer), textBuffer);
#endif
    }

    textBuffer[0] = '\0';
    idx = 0;
}

void Comm::bufferUInt( uint32_t v) {

    char b[12]; // Max. 10 characters for a 32 bit number.
    uint8_t i = 0;

    if( v == 0 ) {
        b[i++] = '0';
    } else {
        while( v ) {
            b[i++] = (v % 10) + '0';
            v /= 10;
        }
    }

    // Output in reverse order 
    while( i > 0 ) {
        i--;
        bufferChar( b[i]);
    }
}

void Comm::bufferInt( int32_t v) {

    uint32_t uv;

    if( v < 0 ) {
        bufferChar( '-');
        uv = -v;
    } else {
        uv = v;
    }

    return bufferUInt( uv);
}

void Comm::open( char bType) {

    textBuffer[0] = '\0';
    idx = 0;
    subLevel = 0;
    checksum = 0;

    // "{TIII"
    if( ensureSpace( 5)) {
        bufferChar( '{');
        bufferChar( bType);
        terminateLine();
    }
}

void Comm::open( char bType, uint8_t bId) {

    textBuffer[0] = '\0';
    idx = 0;
    subLevel = 0;
    checksum = 0;

    // "{TIII"
    if( ensureSpace( 5)) {
        bufferChar( '{');
        bufferChar( bType);
        bufferUInt( bId);
        terminateLine();
    }
}

void Comm::openSub( char sType, uint8_t sId) {

    // "[TIII"
    if( ensureSpace( 5)) {
        bufferChar( '[');
        bufferChar( sType);
        bufferUInt( sId);
        terminateLine();
        subLevel++;
    }
}

void Comm::close() {

    // "}ccc"
    // "]"
    if( ensureSpace( subLevel == 0 ? 4 : 1)) {
        if( subLevel) {
            bufferChar( ']');
            subLevel--;
        } else {
            bufferChar( '}');
            bufferUInt( checksum);
        }
        terminateLine();
    }
}
void Comm::addString( char fName, const char *text) {

    uint16_t len = (uint16_t)strlen(text);

    // "N"text"
    if( ensureSpace( 2 + len)) {
        bufferChar( fName);
        bufferChar( COMM_DATATYPE_STRING);
        bufferString( text);
        terminateLine();
    }
}

void Comm::addChar( char fName, char ch) {

    // "N'C"
    if( ensureSpace( 3)) {
        bufferChar( fName);
        bufferChar( COMM_DATATYPE_CHAR);
        bufferChar( ch);
        terminateLine();
    }
}

void Comm::addBool( char fName, bool b) {

    // "N?B"
    if( ensureSpace( 3)) {
        bufferChar( fName);
        bufferChar( COMM_DATATYPE_BOOL);
        bufferChar( b ? '1' : '0');
        terminateLine();
    }
}

void Comm::addInt8( char fName, int8_t d) {

    // "N-w:sddd"
    if( ensureSpace( 8)) {
        bufferChar( fName);
        bufferChar( COMM_DATATYPE_SIGNED_INT);
        bufferChar( '1');
        bufferChar( COMM_DATATYPE_DELIM);
        bufferInt( d);
        terminateLine();
    }
}

void Comm::addUInt8( char fName, uint8_t d) {

    // "N+w:ddd"
    if( ensureSpace( 7)) {
        bufferChar( fName);
        bufferChar( COMM_DATATYPE_UNSIGNED_INT);
        bufferChar( '1');
        bufferChar( COMM_DATATYPE_DELIM);
        bufferUInt( d);
        terminateLine();
    }
}

void Comm::addInt16( char fName, int16_t d) {

    // "N-w:sddddd"
    if( ensureSpace( 10)) {
        bufferChar( fName);
        bufferChar( COMM_DATATYPE_SIGNED_INT);
        bufferChar( '2');
        bufferChar( COMM_DATATYPE_DELIM);
        bufferInt( d);
        terminateLine();
    }
}

void Comm::addUInt16( char fName, uint16_t d) {

    // "N+w:ddddd"
    if( ensureSpace( 9)) {
        bufferChar( fName);
        bufferChar( COMM_DATATYPE_UNSIGNED_INT);
        bufferChar( '2');
        bufferChar( COMM_DATATYPE_DELIM);
        bufferUInt( d);
        terminateLine();
    }
}
void Comm::addInt32( char fName, int32_t d) {

    // "N-w:sdddddddddd"
    if( ensureSpace( 15)) {
        bufferChar( fName);
        bufferChar( COMM_DATATYPE_SIGNED_INT);
        bufferChar( '4');
        bufferChar( COMM_DATATYPE_DELIM);
        bufferInt( d);
        terminateLine();
    }
}

void Comm::addUInt32( char fName, uint32_t d) {

    // "N+w:dddddddddd"
    if( ensureSpace( 14)) {
        bufferChar( fName);
        bufferChar( COMM_DATATYPE_UNSIGNED_INT);
        bufferChar( '4');
        bufferChar( COMM_DATATYPE_DELIM);
        bufferUInt( d);
        terminateLine();
    }
}

void Comm::addIntArr( char fName, const byte *arr, size_t sz, uint16_t cnt) {

    /* byteSz 1, 2 or 4 */
    uint8_t byteSz = sz / cnt;
    uint16_t charSz = (byteSz==1 ? 5 : 0) + (byteSz==2 ? 7 : 0) + (byteSz==4 ? 12 : 0);

    //LOGV("Comm::addIntArr(): byteSz %d charSz %d\n", byteSz, charSz);

    // "N#w:ddd,ddd,ddd..."
    if( ensureSpace( 4 + (cnt * charSz))) {
        bufferChar( fName);
        bufferChar( COMM_DATATYPE_SIGNED_ARRAY);
        bufferUInt( byteSz);
        bufferChar( COMM_DATATYPE_DELIM);
        for( uint16_t i = 0; i < cnt; i++) {
            if( i > 0) {
                bufferChar( ',');
            }
            switch(byteSz) {
                case 1:
                    bufferInt( ((const int8_t*)arr)[i]);
                    break;
                case 2:
                    bufferInt( ((const int16_t*)arr)[i]);
                    break;
                case 4:
                    bufferInt( ((const int32_t*)arr)[i]);
            }
        }
        terminateLine();
    }
}

void Comm::addUIntArr( char fName, const byte *arr, size_t sz, uint16_t cnt) {

    /* byteSz 1, 2 or 4 */
    uint8_t byteSz = sz / cnt;
    uint16_t charSz = (byteSz==1 ? 4 : 0) + (byteSz==2 ? 6 : 0) + (byteSz==4 ? 11 : 0);

    //LOGV("Comm::addUIntArr(): byteSz %d charSz %d\n", byteSz, charSz);

    // "N%w:ddd,ddd,ddd..."
    if( ensureSpace( 4 + (cnt * charSz))) {
        bufferChar( fName);
        bufferChar( COMM_DATATYPE_UNSIGNED_ARRAY);
        bufferUInt( byteSz);
        bufferChar( COMM_DATATYPE_DELIM);
        for( uint16_t i = 0; i < cnt; i++) {
            if( i > 0) {
                bufferChar( ',');
            }
            switch(byteSz) {
                case 1:
                    bufferUInt( ((const uint8_t*)arr)[i]);
                    break;
                case 2:
                    bufferUInt( ((const uint16_t*)arr)[i]);
                    break;
                case 4:
                    bufferUInt( ((const uint32_t*)arr)[i]);
            }
        }
        terminateLine();
    }
}
