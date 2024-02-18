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

#ifdef EMULATION
  #include "Stream.h"
#endif

/*
 * Commm Packet Format
 * ===================
 * 
 * {T
 * Ntdddd
 * {S
 * ntddd
 * }
 * }CCC
 *
 * T   = Block type (2 characters, only letters and numbers)
 *
 * N   = field name (2 characters, only letters and numbers)
 * 
 * t   = field data type
 *       "w: = String
 *       ': = Character
 *       ?: = boolean
 *       +w: = unsigned numeric
 *       -w: = signed numeric
 *       %w:n: = unsigned numeric array
 *       #w:n: = signed numeric array
 *       where "w" is the width in bytes (1,2,4)
 *       where "n" is the array length
 * 
 * ddd = data type dependent data
 * 
 * S   = Sub type (2 characters, only letters and numbers)
 * 
 * CCC = Checksum from "{" to "}" (including)
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

/* comm packet types */
using nameType_t = uint16_t;

#define PACKET_TYPE( a, b) ((((nameType_t)a) << 8) +  b)
#define PACKET_NAME( p ) (char)(p >> 8),(char)p

/* field names */
#define FIELD_TYPE( a, b) ((((nameType_t)a) << 8) +  b)


#define COMM_PACKET_GET_SYSCONFIG         PACKET_TYPE('G','S')
#define COMM_PACKET_GET_MODELCONFIG       PACKET_TYPE('G','M')
#define COMM_PACKET_INFO                  PACKET_TYPE('I','N')
#define COMM_PACKET_ERROR                 PACKET_TYPE('E','R')
#define COMM_PACKET_MODELCONFIG           PACKET_TYPE('M','C')
#define COMM_PACKET_SYSCONFIG             PACKET_TYPE('S','C')

/* This marks modules that do not need import/export */
#define COMM_SUBPACKET_NONE               PACKET_TYPE('\0','\0')

#define COMM_SUBPACKET_ANALOG_SWITCH      PACKET_TYPE('A','S')
#define COMM_SUBPACKET_ANALOG_TRIM        PACKET_TYPE('A','T')
#define COMM_SUBPACKET_ASSIGN_INPUT       PACKET_TYPE('A','I')
#define COMM_SUBPACKET_CALIBRATE_STICKS   PACKET_TYPE('C','S')
#define COMM_SUBPACKET_CALIBRATE_TRIM     PACKET_TYPE('C','T')
#define COMM_SUBPACKET_CHANNEL_DELAY      PACKET_TYPE('C','D')
#define COMM_SUBPACKET_CHANNEL_RANGE      PACKET_TYPE('C','R')
#define COMM_SUBPACKET_CHANNEL_REVERSE    PACKET_TYPE('C','V')
#define COMM_SUBPACKET_DUAL_EXPO          PACKET_TYPE('D','X')
#define COMM_SUBPACKET_DUAL_EXPO_PHASE    PACKET_TYPE('D','P')
#define COMM_SUBPACKET_ENGINE_CUT         PACKET_TYPE('E','C')
#define COMM_SUBPACKET_LOGIC_SWITCH       PACKET_TYPE('L','S')
#define COMM_SUBPACKET_MIXER              PACKET_TYPE('M','X')
#define COMM_SUBPACKET_MODE_ASSIGN        PACKET_TYPE('M','A')
#define COMM_SUBPACKET_MODEL              PACKET_TYPE('M','O')
#define COMM_SUBPACKET_PHASES             PACKET_TYPE('P','H')
#define COMM_SUBPACKET_PHASES_TRIM        PACKET_TYPE('P','T')
#define COMM_SUBPACKET_PHASES_TRIM_PHASE  PACKET_TYPE('P','P')
#define COMM_SUBPACKET_SERVO_LIMIT        PACKET_TYPE('S','L')
#define COMM_SUBPACKET_SERVO_REMAP        PACKET_TYPE('S','M')
#define COMM_SUBPACKET_SERVO_REVERSE      PACKET_TYPE('S','V')
#define COMM_SUBPACKET_SERVO_SUBTRIM      PACKET_TYPE('S','T')
#define COMM_SUBPACKET_SWITCHED_CHANNELS  PACKET_TYPE('W','C')
#define COMM_SUBPACKET_TIMER              PACKET_TYPE('T','I')
#define COMM_SUBPACKET_VCC_MONITOR        PACKET_TYPE('V','M')

#define COMM_FIELD_ID                     FIELD_TYPE('I','D')
#define COMM_FIELD_CHANNEL                FIELD_TYPE('C','H')
#define COMM_FIELD_CHANNEL_ARRAY          FIELD_TYPE('C','A')
#define COMM_FIELD_CHANNEL_SOURCE         FIELD_TYPE('C','S')
#define COMM_FIELD_CHANNEL_TARGET         FIELD_TYPE('C','T')
#define COMM_FIELD_PERCENT                FIELD_TYPE('P','C')
#define COMM_FIELD_PERCENT_ARRAY          FIELD_TYPE('P','A')
#define COMM_FIELD_NEG_PERCENT_ARRAY      FIELD_TYPE('N','A')
#define COMM_FIELD_DELAY_ARRAY            FIELD_TYPE('D','A')
#define COMM_FIELD_NEG_DELAY_ARRAY        FIELD_TYPE('d','A')
#define COMM_FIELD_SWITCH                 FIELD_TYPE('W','W')
#define COMM_FIELD_SWITCH_ARRAY           FIELD_TYPE('W','A')
#define COMM_FIELD_STATEA_ARRAY           FIELD_TYPE('S','A')
#define COMM_FIELD_STATEB_ARRAY           FIELD_TYPE('T','A')
#define COMM_FIELD_STATEC_ARRAY           FIELD_TYPE('U','A')
#define COMM_FIELD_LOW_ARRAY              FIELD_TYPE('L','O')
#define COMM_FIELD_MID_ARRAY              FIELD_TYPE('M','I')
#define COMM_FIELD_HIGH_ARRAY             FIELD_TYPE('H','I')
#define COMM_FIELD_RATE_ARRAY             FIELD_TYPE('R','A')
#define COMM_FIELD_EXPO_ARRAY             FIELD_TYPE('X','A')
#define COMM_FIELD_BITS                   FIELD_TYPE('B','F')
#define COMM_FIELD_PHASE                  FIELD_TYPE('P','H')
#define COMM_FIELD_PHASE_NAMES            FIELD_TYPE('P','N')
#define COMM_FIELD_MODEL_NAME             FIELD_TYPE('M','N')
#define COMM_FIELD_MODEL_MIX              FIELD_TYPE('M','M')
#define COMM_FIELD_MODEL_SWITCH_QDIFF     FIELD_TYPE('W','Q')
#define COMM_FIELD_MODEL_QDIFF_PCT        FIELD_TYPE('Q','P')
#define COMM_FIELD_MODEL_SWITCH_MIX       FIELD_TYPE('W','M')
#define COMM_FIELD_MODEL_PERCENT_MIX      FIELD_TYPE('P','M')
#define COMM_FIELD_MODEL_OFFSET_MIX       FIELD_TYPE('O','M')
#define COMM_FIELD_TIME                   FIELD_TYPE('T','M')
#define COMM_FIELD_WARN                   FIELD_TYPE('V','W')
#define COMM_FIELD_ALERT                  FIELD_TYPE('V','A')
#define COMM_FIELD_ADJUST                 FIELD_TYPE('V','J')

#define COMM_CHAR_OPEN                      '{'
#define COMM_CHAR_CLOSE                     '}'
#define COMM_CHAR_SUBOPEN                   '{'
#define COMM_CHAR_SUBCLOSE                  '}'
#define COMM_CHAR_DELIM                     ':'
#define COMM_CHAR_SEPERATOR                 ','
#define COMM_CHAR_NL                        '\n'

#define COMM_CHAR_DATATYPE_STRING           '"'
#define COMM_CHAR_DATATYPE_CHAR             '\''
#define COMM_CHAR_DATATYPE_BOOL             '?'
#define COMM_CHAR_DATATYPE_UNSIGNED_INT     '+'
#define COMM_CHAR_DATATYPE_SIGNED_INT       '-'
#define COMM_CHAR_DATATYPE_UNSIGNED_ARRAY   '%'
#define COMM_CHAR_DATATYPE_SIGNED_ARRAY     '#'

#define EOD (-1)

class Comm {

    private:
        Stream *inOut;

        char textBuffer[ COMM_TEXT_MAXLEN ];
        uint16_t idx = 0;
        uint8_t subLevel = 0;
        uint16_t checksum = 0;
        int pushback;
        boolean pushbackAvailable = false;

        /* SEND */

        /* Make sure there is enough space in the buffer 
         * Returns true on success.
         */
        bool ensureSpace( uint16_t bytes) const;

        /* Put a single character into the buffer. */
        void bufferChar( char ch);
        /* Put a string into the buffer. */
        void bufferString( const char *s);
        /* Put a name into the buffer. A name is always 2 characters long. */
        void bufferName( nameType_t name);
        /* Terminate the buffer with newline and nul. */
        void terminateLine();
        /* Put a signed integer into the buffer */
        void bufferInt( int32_t v);
        /* Put a unsigned integer into the buffer */
        void bufferUInt( uint32_t v);

        /* RECEIVE */

        /* Read characters until open found.
         * Does not wait for timeout.
         * Returns false if no more characters available.
         */
        boolean searchOpen();

        /* Expect next char.
         * With timeout.
         */
        boolean expectChar( char ch);

        /* Expect an optional char.
         * With timeout.
         * If the next character is 'ch' it is consumed and true is returned.
         * If the next character is different it is pushed back and false is returned.
         */
        boolean optionalChar( char ch);

        /* Get next character. 
         */
        int getChar();

        /* Check if there is at least one character available. 
         */
        boolean available();

        void pushBack( int ch) {
            pushback = ch;
            pushbackAvailable = true;
        }

        /* 
         */
        boolean verifyType( char dType);

        uint8_t parseInt8( int8_t *p);
        uint8_t parseUInt8( uint8_t *p);
        uint8_t parseInt16( int16_t *p);
        uint8_t parseUInt16( uint16_t *p);
        uint8_t parseInt32( int32_t *p);
        uint8_t parseUInt32( uint32_t *p);

        uint32_t parseUInt();
        int32_t parseInt();

        uint8_t parseName( nameType_t *name);

    public:
       Comm( Stream *stream);


        /* Send API */

        #define COMM_DATATYPE_STRING    1
        #define COMM_DATATYPE_CHAR      2
        #define COMM_DATATYPE_BOOL      3
        #define COMM_DATATYPE_INT8      4
        #define COMM_DATATYPE_UINT8     5
        #define COMM_DATATYPE_INT16     6
        #define COMM_DATATYPE_UINT16    7
        #define COMM_DATATYPE_INT32     8
        #define COMM_DATATYPE_UINT32    9
        #define COMM_DATATYPE_INTARR    10
        #define COMM_DATATYPE_UINTARR   11

        void open( nameType_t bType);
        void openSub( nameType_t sType);
        void close();
        void addString( nameType_t fName, const char *text);
        void addChar( nameType_t fName, char ch);
        void addBool( nameType_t fName, bool b);
        void addInt8( nameType_t fName, int8_t d);
        void addUInt8( nameType_t fName, uint8_t d);
        void addInt16( nameType_t fName, int16_t d);
        void addUInt16( nameType_t fName, uint16_t d);
        void addInt32( nameType_t fName, int32_t d);
        void addUInt32( nameType_t fName, uint32_t d);
        void addIntArr( nameType_t fName, const byte *arr, size_t sz, uint16_t cnt);
        void addUIntArr( nameType_t fName, const byte *arr, size_t sz, uint16_t cnt);

        size_t write();
        size_t writePart();


        /* Receive API */

        #define COMM_RC_OK         0
        #define COMM_RC_NODATA     1
        #define COMM_RC_FIELD      2
        #define COMM_RC_SUBSTART   3
        #define COMM_RC_END        4
        #define COMM_RC_END_CSFAIL 5
        #define COMM_RC_PROTOCOL   6

        /* Remove everything from input buffer.
         */
        uint8_t drain();

        /* Wait for a packet
         *
         * returns COMM_RC_OK        if a valid packet header was received.
         * returns COMM_RC_NODATA    otherwise
         */
        uint8_t nextPacket( nameType_t *pType);

        /* Read next field in packet
         *
         * returns COMM_RC_OK         if a valid field has been received
         *                            fName is filled with field name
         *                            dType is filled with field data type
         *                            width is filled with field data type width for numeric fields
         * returns COMM_RC_SUBSTART   if sub block start marker was received
         *                            fName is filled with sub block type
         * returns COMM_RC_END        if packet end marker was receiven and checksum is ok
         * returns COMM_RC_NODATA     in case of timeout
         * returns COMM_RC_PROTOCOL   in case of a packet format error (Garbage received)
         */
        uint8_t nextField( nameType_t *fType, char *dType, uint8_t *width, uint16_t *count);

        /* Read next field in packet
         *
         * NOTE: For String data type *data will always be null terminated.
         *       That means for a 10 characters long string (count=10) 11 characters will be written
         *       to *data. Make sure the field is long enough!
         *
         * returns COMM_RC_OK         if valid data has been received
         *                            fName is filled with field name
         *                            dType is filled with field data type
         *                            width is filled with field data type width for numeric fields
         * returns COMM_RC_NODATA     in case of timeout
         * returns COMM_RC_PROTOCOL   in case of a packet format error (Garbage received)
         */
        uint8_t nextData( void *data, char dType, uint8_t width, uint16_t count);
};

#endif
