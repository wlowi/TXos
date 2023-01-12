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

#ifndef _UtModules_
#define _UtModules_

#include "UnitTest.h"

class UtModules : public UnitTest {

    private:

    public:
        void run();

        void UtCalibrateSticks();
        void UtCalibrateTrim();
        void UtAnalogTrim();
        void UtSwitchedChannels();
        void UtAnalogSwitch();
        void UtChannelRange();
        void UtChannelReverse();

        void UtAssignInput();
        void UtChannelDelay();

        void verify( channel_t start, uint8_t count, channelValue_t in, channelValue_t expected);
        void dumpControls( Controls &controls);
};

#endif