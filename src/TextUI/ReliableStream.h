/*
  TextUI. A simple text based UI.

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

#ifndef _ReliableStream_h_
#define _ReliableStream_h_

#include "Arduino.h"

#include "Stream.h"


const uint8_t MAX_PAYLOAD_SIZE = 16;
const uint8_t ALLOWED_OURSTANDING = 3;
const uint16_t MINBUFFER_SIZE = 32;
const uint16_t MAXBUFFER_SIZE = 1024;
const uint8_t SEQUENCE_COUNT = 16; // Number of valid sequences 0..15

const uint8_t DATA_PACKET = 0x00;
const uint8_t ACK_PACKET  = 0x10;
const uint8_t SYNC_PACKET = 0xf0;

/**
 * @brief Send and receive buffer.
 * 
 */
typedef struct buffer_t {

    uint16_t size;
    uint8_t *buffer;
    uint16_t head;
    uint16_t tail;

} buffer_t;

/**
 * @brief A temporary packet object used to keep packet state over
 * multiple invocations of the readPacket method.
 * 
 */
typedef struct packet_t {

    /**
     * @brief Packet state.
     *
     * 1     => h is valid
     * 2     => h and s are valid
     * >2    => (state-2) bytes of payload are valid
     */
    uint8_t state;

    uint8_t h; // header: packet type and ack sequence
    uint8_t s; // size and packet sequence number
    uint8_t payload[MAX_PAYLOAD_SIZE];
    
} packet_t;

/**
 * @brief A stream providing reliable communication on top of an 
 * unreliable stream like a serial line without handshake.
 * 
 * Packet Format 
 * ============== 
 *
 *     Byte 1: tttt aaaa
 *       tttt: Packet type
 *          0: Data packet
 *          1: Acknowledge packet
 *         ff: Sync packet (aaaa always 0)
 *       aaaa: Acknowlege sequence no.
 *       
 *     Byte 2: ssss qqqq
 *       ssss: Payload size (1 - 16, 0 means payload size of 1 byte! )
 *       qqqq: Packet sequence no.
 *       
 *     Byte 3-18: Payload
 *
 */
class ReliableStream : public Stream
{
private:
    Stream &stream;

    uint8_t sendSeq;
    uint8_t recvSeq;
    uint8_t ackdSeq;

    packet_t packet;

    buffer_t sendBuffer;
    buffer_t recvBuffer;

    void makeBuffer( buffer_t &b, uint16_t sz);

    inline bool isBufferEmpty( buffer_t& b) {
        return b.head == b.tail;
    }

    inline uint8_t getByte( buffer_t& b) {
        uint8_t c = b.buffer[b.tail];
        b.tail = (b.tail + 1) % b.size;
        return c;
    }

    inline uint8_t peekByte( buffer_t& b) {
        return b.buffer[b.tail];
    }

    inline void putByte( buffer_t& b, uint8_t c) {
        b.buffer[b.head] = c;
        b.head = (b.head + 1) % b.size;
    }

    inline uint16_t spaceAvailable( buffer_t& b) {
        if (b.head >= b.tail) {
            return b.size - 1 - b.head + b.tail;
        } else {
            return b.tail - b.head - 1;
        }
    }

    inline uint16_t bytesAvailable( buffer_t& b) {
        return (b.size + b.head - b.tail) % b.size;
    }

    inline uint8_t outstanding() {
        if( sendSeq >= ackdSeq) {
            return sendSeq - ackdSeq;
        } else {
            return SEQUENCE_COUNT - (ackdSeq - sendSeq);
        }
    }

    inline bool validAck( uint8_t ack) {

        uint8_t miss;

        if( ack >= ackdSeq) {
            miss = ack - ackdSeq;
        } else {
            miss = SEQUENCE_COUNT - (ackdSeq - ack);
        }

        return (miss <= outstanding());
    }

    void reset();
    int available_intern(void);
    int availableForWrite_intern(void);
    void sendPacket( uint8_t t, uint16_t sz, buffer_t& b );
    /* Returns true if a complete packet was received */
    bool recvPacket( buffer_t& b );
    bool recvData( buffer_t& b );

public:
    ReliableStream( Stream& str, uint16_t sendBufferSz, uint16_t recvBufferSz);

    /**
     * @brief Should be called periodically to complete outstanding send/receive operations.
     * 
     */
    void handleComm();

    /**
     * @brief Inherited from Stream
     * 
     * @return int Number of available characters to read.
     */
    virtual int available(void);
    /**
     * @brief Inherited from Stream
     * 
     * @return int Next character without removing it, or -1.
     */
    virtual int peek(void);
    /**
     * @brief Inherited from Stream
     * 
     * @return int Next character, or -1.
     */
    virtual int read(void);

    /**
     * @brief Inherited from Stream/Print
     * 
     * @return int Free buffer space for write.
     */
    virtual int availableForWrite(void);
    /**
     * @brief Inherited from Stream/Print
     */
    virtual void flush(void);
    /**
     * @brief Inherited from Stream/Print
     * 
     * @return size_t Number of characters written, usually 1.
     */
    virtual size_t write(uint8_t ch);
};

#endif
