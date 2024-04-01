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

#include "ReliableStream.h"

/*
 * Protocol:
 * =========
 * 
 * Packet Format:
 * ==============
 * 
 * Byte 1: tttt aaaa
 *   tttt: Packet type
 *      0: Data packet
 *      1: Acknowledge packet
 *     ff: Sync packet
 *   aaaa: Acknowlege sequence no.
 * 
 * Byte 2: ssss qqqq
 *   ssss: Payload size (1 - 16, 0 means payload size of 1 byte! )
 *   qqqq: Packet sequence no.
 * 
 * Byte 3-18: Payload
 * 
 */

ReliableStream::ReliableStream( Stream &str, uint16_t sendBufferSz, uint16_t recvBufferSz) : stream(str)
{
    makeBuffer( sendBuffer, sendBufferSz);
    makeBuffer( recvBuffer, recvBufferSz);

    str.setTimeout( 10);

    reset();
}

int ReliableStream::available(void)
{
    if( available_intern() == 0) {
        handleComm();
    }

    return available_intern();
}

int ReliableStream::peek(void)
{
    if( isBufferEmpty( recvBuffer)) {
        handleComm();
        if( isBufferEmpty( recvBuffer)) {
            return -1;
        }
    }

    return peekByte( recvBuffer);
}

int ReliableStream::read(void)
{
    if( isBufferEmpty( recvBuffer)) {
        handleComm();
        if( isBufferEmpty( recvBuffer)) {
            return -1;
        }
    }

    return getByte( recvBuffer);
}

int ReliableStream::availableForWrite(void)
{
    if (availableForWrite_intern() == 0) {
        handleComm();
    }
    
    return availableForWrite_intern();
}

void ReliableStream::flush(void)
{
    handleComm();
}

size_t ReliableStream::write(uint8_t ch)
{
    while( availableForWrite_intern() == 0) {
        handleComm();
    }

    putByte( sendBuffer, ch);

    return 1;
}

void ReliableStream::handleComm()
{
    uint16_t bytes;

    while( stream.available()) {
        if( !recvPacket( recvBuffer)) { // incomplete packet
            break;
        }
    }

    while( (bytes = bytesAvailable( sendBuffer)) ) {
        if( outstanding() >= ALLOWED_OURSTANDING) {
            break;
        }

        sendPacket( DATA_PACKET, bytes, sendBuffer);
    }
}

/* ** PRIVATE ** */

void ReliableStream::reset()
{
    sendSeq = recvSeq = ackdSeq = 0;

    sendBuffer.head = sendBuffer.tail = 0;
    recvBuffer.head = recvBuffer.tail = 0;

    packet.state = 0;
}

int ReliableStream::available_intern(void)
{
    return bytesAvailable( recvBuffer);
}

int ReliableStream::availableForWrite_intern(void)
{
    return spaceAvailable( sendBuffer);
}

void ReliableStream::makeBuffer( buffer_t& b, uint16_t sz)
{
    if( sz < MINBUFFER_SIZE) sz = MINBUFFER_SIZE;
    if( sz > MAXBUFFER_SIZE) sz = MAXBUFFER_SIZE;

    b.size = sz;
    b.buffer = new uint8_t[sz];
    b.head = b.tail = 0;
}

void ReliableStream::sendPacket( uint8_t t, uint16_t sz, buffer_t& b ) {

    switch( t) {
        case DATA_PACKET:
            if( sz == 0) { return; }
            if( sz > MAX_PAYLOAD_SIZE) { sz = MAX_PAYLOAD_SIZE; }

            stream.write( DATA_PACKET | (recvSeq & 0x0f));
            stream.write( (((sz-1) & 0x0f) << 4) | (sendSeq & 0x0f));
            sendSeq = (sendSeq +1) % SEQUENCE_COUNT;
            while( sz-- ) {
                stream.write( getByte( b));
            }
            break;

        case ACK_PACKET:
            stream.write( ACK_PACKET | (recvSeq & 0x0f));
            break;

        case SYNC_PACKET:
            stream.write( SYNC_PACKET);
            break;
    }
}

/* Returns true if a complete packet was received */
bool ReliableStream::recvPacket( buffer_t& b ) {

    int h;
    uint8_t ack;

    if( packet.state != 0 ) {
        return recvData( b);
    }

    h = stream.read();
    if( h == -1) { return false; }

    switch( h & 0xf0) {
        case DATA_PACKET:
            ack = h & 0x0f;
            if( validAck( ack) ) {
                ackdSeq = ack;
                packet.h = (uint8_t)h;
                packet.state = 1;

                return recvData( b);

            } else {
                sendPacket( SYNC_PACKET, 0, sendBuffer);
                reset();
            }
            break;

        case ACK_PACKET:
            ack = h & 0x0f;
            if( validAck( ack) ) {
                ackdSeq = ack;
            } else {
                sendPacket( SYNC_PACKET, 0, sendBuffer);
                reset();                
            }
            break;

        case SYNC_PACKET:
            reset();
    }

    return true;
}

/* Returns true if a complete packet was received */
bool ReliableStream::recvData( buffer_t& b ) {

    int d;
    uint8_t i;
    uint8_t sz = 0;

    /* Get number of payload bytes if available */
    if( packet.state > 1) {
        sz = ((packet.s & 0xf0) >> 4) +1;
    }

    while( (d = stream.read()) != -1) {

        if( packet.state == 1) {
            packet.s = (uint8_t)d;
            packet.state = 2;
            sz = ((packet.s & 0xf0) >> 4) +1;

        } else if( packet.state >= 2 && packet.state <= sz) {
            packet.payload[packet.state-2] = (uint8_t)d;
            packet.state++;

        } else { // package complete
            packet.payload[packet.state-2] = (uint8_t)d;

            if( spaceAvailable(b) >= sz) {
                for( i=0; i<sz; i++) {
                    putByte( b, packet.payload[i]);
                }
            }

            recvSeq = packet.s & 0x0f;           
            packet.state = 0;

            sendPacket( ACK_PACKET, 0, sendBuffer);

            return true;
        }
    }

    return false;
}
