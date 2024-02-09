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

#ifndef _EmuSerial_h_
#define _EmuSerial_h_

#include <wx/thread.h>

#include "stddef.h"
#include "Stream.h"

/* The buffer size needs to be big enough to store data for a complete transfer.
 * Currently EmuSerial is called from main thread and SerialConsole is also
 * called from main thread. There is no way to switch between threads 
 * and comsume the buffer content.
 */
const int EMUSERIAL_BUFFER_SIZE = 10240;

class EmuSerial : public Stream {

private:
    char sendBuffer[EMUSERIAL_BUFFER_SIZE];
    char recvBuffer[EMUSERIAL_BUFFER_SIZE];

    /* send = emulation to SerialConsole */
    wxCriticalSection sendCriticalSection;
    int sendInPtr = 0;
    int sendOutPtr = 0;

    /* recv = SerialConsole to emulation */
    wxCriticalSection recvCriticalSection;
    int recvInPtr = 0;
    int recvOutPtr = 0;

public:
    EmuSerial();

    void send(const char *text);

    int receive();
    
    /* Interface: Stream */

    void setTimeout(unsigned long timeout) {}; // noop

    size_t write(const char* text);

    void flush() {}; // noop

    int read();

    int available();

    void close() {}; // noop 
};

#endif