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

#ifndef _Comm_h_
#define _Comm_h_

#include "TXos.h"

/*
 * Commm Packet Format
 * ===================
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
 *       " = String ' = Character
 *       ? = boolean
 *       +w: = unsigned numeric
 *       -w: = signed numeric
 *       %w: = unsigned numeric array
 *       #w: = signed numeric array
 *       where "w" is the width in bytes (1,2,4)
 * ddd = data type dependent data
 * [ ] = Sub block
 * S   = Sub type (Block dependent)
 * xxx = numeric identifier (mandatory)
 * CCC = Checksum from "{" to "}" (including)
 * 
 * 
 * Comm Packet Types
 * =================
 * 
 * Command Packet
 * --------------
 * Block Type = C
 * Identifier = -
 * C (String) Command
 * 
 * Info Packet
 * -----------
 * Block Type = I
 * Identifier = -
 * V (String) Version
 * M (String) Max number of models
 * 
 * Error Packet
 * ------------
 * Block Type = E
 * Identifier = -
 * M (String) Error message
 * 
 * System Configuration
 * --------------------
 * Block Type = S
 * Identifier = -
 * 
 * Model Configuration
 * -------------------
 * Block Type = M
 * Identifier = Model number
 * 
 * 
 * API
 * ====
 * 
 * open( T)
 * open( T, iii)
 * openSub( S, xxx)
 * close()
 * 
 * addString( N, ddd)
 * addChar( N, ddd)
 * addBool( N, ddd)
 * addInt8( N, ddd)
 * addUInt8( N, ddd)
 * addInt16( N, ddd)
 * addUInt16( N, ddd)
 * addInt32( N, ddd)
 * addUInt32( N, ddd)
 * addIntArr( N, ddd, sz, cnt)
 * addUIntArr( N, ddd, sz, cnt)
 * 
 * write()
 * writePart()
 * 
 */

const uint16_t COMM_TEXT_MAXLEN = 256;

#define COMM_PACKET_COMMAND           'C'
#define COMM_PACKET_INFO              'I'
#define COMM_PACKET_ERROR             'E'
#define COMM_PACKET_MODEL_CONFIG      'M'
#define COMM_PACKET_SYSTEM_CONFIG     'S'

#define COMM_DATATYPE_STRING          '"'
#define COMM_DATATYPE_CHAR            '\''
#define COMM_DATATYPE_BOOL            '?'
#define COMM_DATATYPE_UNSIGNED_INT    '-'
#define COMM_DATATYPE_SIGNED_INT      '+'
#define COMM_DATATYPE_UNSIGNED_ARRAY  '%'
#define COMM_DATATYPE_SIGNED_ARRAY    '#'
#define COMM_DATATYPE_DELIM           ':'

class Comm {

    private:
        char textBuffer[ COMM_TEXT_MAXLEN ];
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
        Comm();

        /* Send API */
        void open( char bType);
        void open( char bType, uint8_t bId);
        void openSub( char sType, uint8_t sId);
        void close();
        void addString( char fName, const char *text);
        void addChar( char fName, char ch);
        void addBool( char fName, bool b);
        void addInt8( char fName, int8_t d);
        void addUInt8( char fName, uint8_t d);
        void addInt16( char fName, int16_t d);
        void addUInt16( char fName, uint16_t d);
        void addInt32( char fName, int32_t d);
        void addUInt32( char fName, uint32_t d);
        void addIntArr( char fName, const byte *arr, size_t sz, uint16_t cnt);
        void addUIntArr( char fName, const byte *arr, size_t sz, uint16_t cnt);

        void write();
        void writePart();

        /* Receive API */
        #define COMM_RC_NODATA
        #define COMM_RC_OK
        #define COMM_RC_FIELD
        #define COMM_RC_SUBSTART
        #define COMM_RC_SUBEND
        #define COMM_RC_END
        #define COMM_RC_END_CSFAIL
        #define COMM_RC_PROTOCOL

        /* Wait for a packet of given type
         *
         * returns COMM_RC_OK        if a valid packet header was received
         *                           bId is filled with package identifier or 0
         * returns COMM_RC_NODATA    otherwise 
         */
        uint8_t expect( char bType, uint8_t *bId);

        /* Read next field in packet
         *
         * returns COMM_RC_FIELD      if a valid field has been received
         *                            fName is filled with field name
         *                            dType is filled with field data type
         *                            width is filled with field data type width for numeric fields
         * returns COMM_RC_SUBSTART   if sub block start marker was received
         *                            fName is filled with sub block type
         * returns COMM_RC_SUBEND     if sub block end marker was received
         * returns COMM_RC_END        if packet end marker was receiven and checksum is ok
         * returns COMM_RC_END_CSFAIL if packet end marker was received but the checksum did not match
         * returns COMM_RC_PROTOCOL   in case of a packet format error (Garbage received)
         */
        uint8_t readNext( char *fName, char *dType, uint8_t width);

        /* Packet API */
        void sendCommand( char *cmd);
        void sendError( char *message);
        void sendInfo( char *version, uint8_t modelCnt);
};

#endif