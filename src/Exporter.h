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

#ifndef _Exporter_h_
#define _Exporter_h_

#include "TXos.h"

/*
 *
 * {Tiii
 * ntdddd
 * [Sxxx
 * ntddd
 * ]
 * }CCC
 * 
 * T   = Block type 
 *       "S" = System block "M" = Model block "E" = Error message "C" = Command block
 * iii = numeric identifier (optional)
 * n   = field name
 * t   = field data type
 *       " = String ' = Character ? = boolean # = numeric % = numeric array
 * ddd = data type dependent data
 * [ ] = Sub block
 * S   = Sub type (Block dependent)
 * xxx = numeric identifier (mandatory)
 * CCC = Checksum from "{" to "}" (including)
 * 
 * open( T, iii)
 * openSub( S, xxx)
 * close()
 * 
 * addString( N, ddd)
 * addChar( N, ddd)
 * addBool( N, ddd)
 * addInt( N, ddd)
 * addUInt( N, ddd)
 * addIntArr( N, ddd, sz, cnt)
 * addUIntArr( N, ddd, sz, cnt)
 */

const uint16_t EXPORTER_TEXT_MAXLEN = 256;

class Exporter {

    private:
        char textBuffer[ EXPORTER_TEXT_MAXLEN ];
        uint16_t idx = 0; 
        uint8_t subLevel = 0;
        uint16_t checksum = 0;

        bool ensureSpace( uint16_t bytes) const;
        void bufferChar( char ch);
        void bufferString( const char *s);
        void terminateLine();
        void bufferInt( int32_t v);
        void bufferUInt( uint32_t v);

    public:
        Exporter();

        void open( char bType, uint8_t bId);
        void openSub( char sType, uint8_t sId);
        void close();
        void addString( char fName, const char *text);
        void addChar( char fName, char ch);
        void addBool( char fName, bool b);
        void addInt( char fName, int32_t d);
        void addUInt( char fName, uint32_t d);
        void addIntArr( char fName, const byte *arr, size_t sz, uint16_t cnt);
        void addUIntArr( char fName, const byte *arr, size_t sz, uint16_t cnt);

        void write();
        void writePart();
};

#endif