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

/*
    Interface code to the actual implementation of PPM generation.
 */

#ifndef _Output_h_
#define _Output_h_

#include "TXos.h"
#include "Controls.h"

class Output {

    public:
        Output();

        /* Returns true if the PPM generator is ready to accept the next channel set */
        bool acceptChannels() const;
        void setChannels( channelSet_t &channels) const;
};

#endif
