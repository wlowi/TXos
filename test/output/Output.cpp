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

#include "Output.h"
#include "OutputImpl.h"

extern OutputImpl *outputImpl;

Output::Output() = default;

/* Returns true if the PPM generator is ready to accept the next channel set */
bool Output::acceptChannels() const {
  
  return outputImpl->acceptChannels();  
}

void Output::setChannels( Controls &controls) const {

    for( channel_t ch = 0; ch<PPM_CHANNELS; ch++) {
        outputImpl->SetChannelValue( ch, controls.outputGet(ch));
    }
}
