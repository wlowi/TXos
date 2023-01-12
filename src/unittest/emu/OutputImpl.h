/*
    TXos. A remote control transmitter OS.

    Copyright (C) 2022 Wolfgang Lohwasser

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#ifndef _OutputImpl_h_
#define _OutputImpl_h_

#include "Controls.h"

class OutputImpl {

        long lastFrameMs = 0;
        
    public:
        OutputImpl( int channels);

        bool acceptChannels();
        void SetChannelValue( int channel, int value);
        uint16_t getOverrunCounter();
        timingUsec_t getMaxFrameTime();
};

#endif