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

#include "stdio.h"

#include "EmuSerial.h"

EmuSerial::EmuSerial() {

}

void EmuSerial::send(const char* text) {

    printf("EmuSerial: send: %s\n", text);

    while( *text) {
        recvCriticalSection.Enter();

        while( *text) {
            if( recvInPtr < (recvOutPtr-1) 
                || ((recvInPtr >= recvOutPtr) && ((recvInPtr != EMUSERIAL_BUFFER_SIZE-1) || (recvOutPtr != 0))))
            {
                recvBuffer[recvInPtr++] = *text;
                text++;
                if( recvInPtr >= EMUSERIAL_BUFFER_SIZE ) {
                    recvInPtr = 0;
                }
            } else {
                printf("EmuSerial: send buffer full\n");
                break;
            }
        }

        recvCriticalSection.Leave();
    }
}

int EmuSerial::receive() {

    int ch = -1; 

    sendCriticalSection.Enter();

    if( sendInPtr != sendOutPtr ) {
        ch = sendBuffer[sendOutPtr++];
        if( sendOutPtr >= EMUSERIAL_BUFFER_SIZE ) {
            sendOutPtr = 0;
        }
    }

    sendCriticalSection.Leave();

    return ch;
}

/* Interface: Stream */

size_t EmuSerial::write(const char* text) {

    size_t cnt = 0;

    printf("EmuSerial: write: %s\n", text);

    while( *text) {
        sendCriticalSection.Enter();

        while( *text) {
            if( sendInPtr < (sendOutPtr-1) 
                || ((sendInPtr >= sendOutPtr) && ((sendInPtr != EMUSERIAL_BUFFER_SIZE-1) || (sendOutPtr != 0))))
            {
                sendBuffer[sendInPtr++] = *text;
                text++;
                cnt++;
                if( sendInPtr >= EMUSERIAL_BUFFER_SIZE ) {
                    sendInPtr = 0;
                }
            } else {
                printf("EmuSerial: write buffer full\n");
                break;
            }
        }

        sendCriticalSection.Leave();
    }

    return cnt;
}

int EmuSerial::read() {

    int ch = -1; 

    recvCriticalSection.Enter();

    if( recvInPtr != recvOutPtr ) {
        ch = recvBuffer[recvOutPtr++];
        if( recvOutPtr >= EMUSERIAL_BUFFER_SIZE ) {
            recvOutPtr = 0;
        }
    }

    recvCriticalSection.Leave();

    return ch;
}

int EmuSerial::available() {

    int available;;

    recvCriticalSection.Enter();

    available = recvInPtr - recvOutPtr;

    if( recvInPtr < recvOutPtr ) {
        available += EMUSERIAL_BUFFER_SIZE;
    }

    recvCriticalSection.Leave();

    return available;
}