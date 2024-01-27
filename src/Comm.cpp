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

Comm::Comm( Stream *stream) : inOut(stream) {

    textBuffer[0] = '\0';
    stream->setTimeout( 500);
}

/* SEND API */

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

void Comm::bufferName( nameType_t name) {

    bufferChar( (char)(name >> 8) );
    bufferChar( (char)(name) );
}

void Comm::terminateLine() {

    textBuffer[idx++] = '\n';
    textBuffer[idx] = '\0';
}

size_t Comm::write() {

    size_t bytesWritten;

    bytesWritten = writePart();
    subLevel = 0;
    checksum = 0;

    return bytesWritten;
}

size_t Comm::writePart() {

    size_t bytesWritten = 0;

    if( idx > 0) {
#if defined(ARDUINO)
        bytesWritten = inOut->write( textBuffer );
        inOut->flush();
#elif defined(DESIGNER)
        bytesWritten = inOut->write( textBuffer );
        inOut->flush();
#elif defined(UNITTEST)
        LOGV("Comm::write() Sent %ld bytes:\n%s", strlen(textBuffer), textBuffer);
#endif
    }

    textBuffer[0] = '\0';
    idx = 0;

    return bytesWritten;
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

void Comm::open( nameType_t bType) {

    textBuffer[0] = '\0';
    idx = 0;
    subLevel = 0;
    checksum = 0;

    // "{TT"
    if( ensureSpace( 3)) {
        bufferChar( COMM_CHAR_OPEN );
        bufferName( bType);
        terminateLine();
    }
}

void Comm::openSub( nameType_t sType) {

    // "[t"
    if( ensureSpace( 3)) {
        bufferChar( COMM_CHAR_SUBOPEN );
        bufferName( sType);
        terminateLine();
        subLevel++;
    }
}

void Comm::close() {

    // "}ccccc"
    // "]"
    if( ensureSpace( subLevel == 0 ? 6 : 1)) {
        if( subLevel) {
            bufferChar( COMM_CHAR_SUBCLOSE );
            subLevel--;
        } else {
            bufferChar( COMM_CHAR_CLOSE );
            bufferUInt( checksum);
        }
        terminateLine();
    }
}

void Comm::addString( nameType_t fName, const char *text) {

    uint16_t len = (uint16_t)strlen(text);

    // "N"LLLLL:text"
    if( ensureSpace( 9 + len)) {
        bufferName( fName);
        bufferChar( COMM_CHAR_DATATYPE_STRING);
        bufferUInt( len);
        bufferChar( COMM_CHAR_DELIM);
        bufferString( text);
        terminateLine();
    }
}

void Comm::addChar( nameType_t fName, char ch) {

    // "N':C"
    if( ensureSpace( 5)) {
        bufferName( fName);
        bufferChar( COMM_CHAR_DATATYPE_CHAR);
        bufferChar( COMM_CHAR_DELIM);
        bufferChar( ch);
        terminateLine();
    }
}

void Comm::addBool( nameType_t fName, bool b) {

    // "N?:B"
    if( ensureSpace( 5)) {
        bufferName( fName);
        bufferChar( COMM_CHAR_DATATYPE_BOOL);
        bufferChar( COMM_CHAR_DELIM);
        bufferChar( b ? '1' : '0');
        terminateLine();
    }
}

void Comm::addInt8( nameType_t fName, int8_t d) {

    // "N-W:sddd"
    if( ensureSpace( 9)) {
        bufferName( fName);
        bufferChar( COMM_CHAR_DATATYPE_SIGNED_INT);
        bufferChar( '1');
        bufferChar( COMM_CHAR_DELIM);
        bufferInt( d);
        terminateLine();
    }
}

void Comm::addUInt8( nameType_t fName, uint8_t d) {

    // "N+W:ddd"
    if( ensureSpace( 8)) {
        bufferName( fName);
        bufferChar( COMM_CHAR_DATATYPE_UNSIGNED_INT);
        bufferChar( '1');
        bufferChar( COMM_CHAR_DELIM);
        bufferUInt( d);
        terminateLine();
    }
}

void Comm::addInt16( nameType_t fName, int16_t d) {

    // "N-W:sddddd"
    if( ensureSpace( 11)) {
        bufferName( fName);
        bufferChar( COMM_CHAR_DATATYPE_SIGNED_INT);
        bufferChar( '2');
        bufferChar( COMM_CHAR_DELIM);
        bufferInt( d);
        terminateLine();
    }
}

void Comm::addUInt16( nameType_t fName, uint16_t d) {

    // "N+W:ddddd"
    if( ensureSpace( 10)) {
        bufferName( fName);
        bufferChar( COMM_CHAR_DATATYPE_UNSIGNED_INT);
        bufferChar( '2');
        bufferChar( COMM_CHAR_DELIM);
        bufferUInt( d);
        terminateLine();
    }
}
void Comm::addInt32( nameType_t fName, int32_t d) {

    // "N-W:sdddddddddd"
    if( ensureSpace( 16)) {
        bufferName( fName);
        bufferChar( COMM_CHAR_DATATYPE_SIGNED_INT);
        bufferChar( '4');
        bufferChar( COMM_CHAR_DELIM);
        bufferInt( d);
        terminateLine();
    }
}

void Comm::addUInt32( nameType_t fName, uint32_t d) {

    // "N+W:dddddddddd"
    if( ensureSpace( 15)) {
        bufferName( fName);
        bufferChar( COMM_CHAR_DATATYPE_UNSIGNED_INT);
        bufferChar( '4');
        bufferChar( COMM_CHAR_DELIM);
        bufferUInt( d);
        terminateLine();
    }
}

void Comm::addIntArr( nameType_t fName, const byte *arr, size_t sz, uint16_t cnt) {

    /* byteSz 1, 2 or 4 */
    uint8_t byteSz = sz / cnt;
    uint16_t charSz = (byteSz==1 ? 5 : 0) + (byteSz==2 ? 7 : 0) + (byteSz==4 ? 12 : 0);

    // "N#W:LLLLL:ddd,ddd,ddd..."
    if( ensureSpace( 11 + (cnt * charSz))) {
        bufferName( fName);
        bufferChar( COMM_CHAR_DATATYPE_SIGNED_ARRAY);
        bufferUInt( byteSz);
        bufferChar( COMM_CHAR_DELIM);
        bufferUInt( cnt);
        bufferChar( COMM_CHAR_DELIM);

        for( uint16_t i = 0; i < cnt; i++) {
            if( i > 0) {
                bufferChar( COMM_CHAR_SEPERATOR);
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

void Comm::addUIntArr( nameType_t fName, const byte *arr, size_t sz, uint16_t cnt) {

    /* byteSz 1, 2 or 4 */
    uint8_t byteSz = sz / cnt;
    uint16_t charSz = (byteSz==1 ? 4 : 0) + (byteSz==2 ? 6 : 0) + (byteSz==4 ? 11 : 0);

    // "N%w:LLLLL:ddd,ddd,ddd..."
    if( ensureSpace( 11 + (cnt * charSz))) {
        bufferName( fName);
        bufferChar( COMM_CHAR_DATATYPE_UNSIGNED_ARRAY);
        bufferUInt( byteSz);
        bufferChar( COMM_CHAR_DELIM);
        bufferUInt( cnt);
        bufferChar( COMM_CHAR_DELIM);

        for( uint16_t i = 0; i < cnt; i++) {
            if( i > 0) {
                bufferChar( COMM_CHAR_SEPERATOR);
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

/* RECEIVE API */

uint8_t Comm::drain() {

    while( available()) {
        getChar();
    }

    return COMM_RC_OK;
}

uint8_t Comm::nextPacket( nameType_t *pType) {

    if( searchOpen()) {
        if( parseName( pType) == COMM_RC_OK) {
            optionalChar( COMM_CHAR_NL);
            return COMM_RC_OK;
        }
    }

    return COMM_RC_NODATA;
}

uint8_t Comm::nextField( nameType_t *fType, char *dType, uint8_t *width, uint16_t *count) {

    uint8_t rc = COMM_RC_OK;

    if( optionalChar( COMM_CHAR_CLOSE )) {
        optionalChar( COMM_CHAR_NL);
        return COMM_RC_END;
    }

    if( optionalChar( COMM_CHAR_SUBOPEN )) {
        pushBack( COMM_CHAR_SUBOPEN);
        return COMM_RC_SUBSTART;
    }

    if( optionalChar( COMM_CHAR_SUBCLOSE )) {
        optionalChar( COMM_CHAR_NL);
        return COMM_RC_END;
    }

    if( parseName( fType) != COMM_RC_OK) {
        return COMM_RC_NODATA;
    }

    *dType = getChar();

    if( !verifyType( *dType)) {
        return COMM_RC_PROTOCOL;
    }

    switch (*dType) {
        case COMM_CHAR_DATATYPE_STRING:
            *width = 1;
            rc = parseUInt16( count); 
            break;

        case COMM_CHAR_DATATYPE_CHAR:
        case COMM_CHAR_DATATYPE_BOOL:
            *width = 1;
            *count = 1;
            break;

        case COMM_CHAR_DATATYPE_UNSIGNED_INT:
        case COMM_CHAR_DATATYPE_SIGNED_INT:
            rc = parseUInt8( width); 
            *count = 1;
            break;

        case COMM_CHAR_DATATYPE_UNSIGNED_ARRAY:
        case COMM_CHAR_DATATYPE_SIGNED_ARRAY:
            rc = parseUInt8( width);
            if( rc != COMM_RC_OK) { break; }
            rc = expectChar(COMM_CHAR_DELIM) ? COMM_RC_OK : COMM_RC_PROTOCOL;
            if( rc != COMM_RC_OK) { break; }
            rc = parseUInt16( count);
    }

    if( rc == COMM_RC_OK) {
        rc = expectChar(COMM_CHAR_DELIM) ? COMM_RC_OK : COMM_RC_PROTOCOL;
    }

    return rc;
}

uint8_t Comm::nextData( void *data, char dType, uint8_t width, uint16_t count) {

    int ch;
    uint16_t i = 0;
    uint8_t rc = COMM_RC_NODATA;

    switch ( dType) {
        case COMM_CHAR_DATATYPE_STRING:
            while( count--) {
                ch = getChar();
                if( ch == EOD) {
                    break;
                }
                ((char*)data)[i++] = (char)ch;
            }
            ((char*)data)[i++] = '\0';
            rc = COMM_RC_OK;
            break;

        case COMM_CHAR_DATATYPE_CHAR:
            *((char*)data) = (char)getChar();
            rc = COMM_RC_OK;
            break;

        case COMM_CHAR_DATATYPE_BOOL:
            *((bool*)data) = ((char)getChar() == '0') ? false : true;
            rc = COMM_RC_OK;
            break;

        case COMM_CHAR_DATATYPE_UNSIGNED_INT:
            switch( width) {
                case 1:
                    parseUInt8( ((uint8_t*)data));
                    break;
                case 2:
                    parseUInt16( ((uint16_t*)data));
                    break;
                case 4:
                    parseUInt32( ((uint32_t*)data));
                    break;
            }
            rc = COMM_RC_OK;
            break;

        case COMM_CHAR_DATATYPE_SIGNED_INT:
            switch( width) {
                case 1:
                    parseInt8( ((int8_t*)data));
                    break;
                case 2:
                    parseInt16( ((int16_t*)data));
                    break;
                case 4:
                    parseInt32( ((int32_t*)data));
                    break;
            }
            rc = COMM_RC_OK;
            break;

        case COMM_CHAR_DATATYPE_UNSIGNED_ARRAY:
            while( count--) {
                switch( width) {
                case 1:
                    parseUInt8( &((uint8_t*)data)[i]);
                    break;
                case 2:
                    parseUInt16( &((uint16_t*)data)[i]);
                    break;
                case 4:
                    parseUInt32( &((uint32_t*)data)[i]);
                    break;
                }
                i++;

                optionalChar( COMM_CHAR_SEPERATOR);
            }
            rc = COMM_RC_OK;
            break;

        case COMM_CHAR_DATATYPE_SIGNED_ARRAY:
            while( count--) {
                switch( width) {
                case 1:
                    parseInt8( &((int8_t*)data)[i]);
                    break;
                case 2:
                    parseInt16( &((int16_t*)data)[i]);
                    break;
                case 4:
                    parseInt32( &((int32_t*)data)[i]);
                    break;
                }
                i++;

                optionalChar( COMM_CHAR_SEPERATOR);
            }
            rc = COMM_RC_OK;
            break;
    }
    
    optionalChar( COMM_CHAR_NL);

    return rc;
}

/* PRIVATE */

boolean Comm::searchOpen() {

    while( available()) {
        if( getChar() == COMM_CHAR_OPEN) {
            return true;
        }
    }

    return false;
}

boolean Comm::expectChar( char ch) {

    int next = getChar();

    if( next == EOD) {
        return false;
    }

    return (next == ch);
}

boolean Comm::optionalChar( char ch) {

    int next = getChar();

    if( next == EOD) {
        return false;
    }

    if( next == ch) {
        return true;
    } else {
        pushBack( next);
        return false;
    }
}

int Comm::getChar() {

    int ch;

    if( pushbackAvailable) {
        pushbackAvailable = false;
        ch = pushback;
    } else {
#ifdef ARDUINO
        /* Do not use read() 
         * It doesn't obey timeout.
         */
        char c;
        if( inOut->readBytes( &c, 1) == 0) {
            ch = EOF;
        } else {
            ch = c;
        }
#else
        ch = inOut->read();
#endif
    }

    return ch;
}

boolean Comm::available() {

    return (pushbackAvailable || inOut->available());
}

boolean Comm::verifyType( char dType) {

    return dType == COMM_CHAR_DATATYPE_STRING
        || dType == COMM_CHAR_DATATYPE_CHAR
        || dType == COMM_CHAR_DATATYPE_BOOL
        || dType == COMM_CHAR_DATATYPE_UNSIGNED_INT
        || dType == COMM_CHAR_DATATYPE_SIGNED_INT
        || dType == COMM_CHAR_DATATYPE_UNSIGNED_ARRAY
        || dType == COMM_CHAR_DATATYPE_SIGNED_ARRAY;
}

uint8_t Comm::parseInt8( int8_t *p) {

    int32_t v = parseInt();

    if( (v >= INT8_MIN) && (v <= INT8_MAX)) {
        *p = (int8_t)v;
        return COMM_RC_OK;
    } else {
        return COMM_RC_PROTOCOL;
    }
}

uint8_t Comm::parseUInt8( uint8_t *p) {

    uint32_t v  = parseUInt();

    if( v <= UINT8_MAX) {
        *p = (uint8_t)v;
        return COMM_RC_OK;
    } else {
        return COMM_RC_PROTOCOL;
    }
}

uint8_t Comm::parseInt16( int16_t *p) {

    int32_t v = parseInt();

    if( (v >= INT16_MIN) && (v <= INT16_MAX)) {
        *p = (int16_t)v;
        return COMM_RC_OK;
    } else {
        return COMM_RC_PROTOCOL;
    }
}

uint8_t Comm::parseUInt16( uint16_t *p) {

    uint32_t v = parseUInt();

    if( v <= UINT16_MAX) {
        *p = (uint16_t)v;
        return COMM_RC_OK;
    } else {
        return COMM_RC_PROTOCOL;
    }
}

uint8_t Comm::parseInt32( int32_t *p) {

    *p = parseInt();

    return COMM_RC_OK;
}

uint8_t Comm::parseUInt32( uint32_t *p) {

    *p = parseUInt();

    return COMM_RC_OK;
}

uint32_t Comm::parseUInt() {

    int ch;
    uint32_t v = 0;

    while( true ) {
        ch = getChar();
        if( ch == EOD) {
            break;
        }
        if( ch >= '0' && ch <= '9') {
            v *= 10;
            v += ch - '0';
        } else {
            pushBack( ch);
            break;
        }
    }

    return v;
}

int32_t Comm::parseInt() {

    int ch;
    int32_t v = 0;
    bool negative = false;

    if( optionalChar( '-')) {
        negative = true;
    }

    while( true ) {
        ch = getChar();
        if( ch == EOD) {
            break;
        }
        if( ch >= '0' && ch <= '9') {
            v *= 10;
            v += ch - '0';
        } else {
            pushBack( ch);
            break;
        }
    }

    return negative ? -v : v;
}

uint8_t Comm::parseName( nameType_t *name) {

    int ch;

    *name = 0;

    for( uint8_t i=0; i<2; i++) {
        ch = getChar();
        if( ch == EOD) {
            return COMM_RC_NODATA;
        }
        if( !isalnum( ch)) {
            return COMM_RC_PROTOCOL;
        }

        if( i == 0) {
            *name = ch << 8;
        } else {
            *name += ch;
        }
    }

    return COMM_RC_OK;
}
