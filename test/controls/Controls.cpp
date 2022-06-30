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
    Interface code to the actual implementation of input controls.
 */

#include "Controls.h"
#include "InputImpl.h"

extern InputImpl *inputImpl;

Controls::Controls() = default;

void Controls::GetControlValues( channelSet_t &channels) const {

    for( channel_t ch = 0; ch < CHANNELS; ch++) {
        channels.analogChannel[ch] = CHANNELVALUE_MID;
    }

    for( channel_t ch = 0; ch < inputImpl->GetChannels(); ch++) {
        channels.analogChannel[ch] = inputImpl->GetChannelValue( ch);
    }

    channels.switches = 0;
    for( switch_t sw = 0; sw < inputImpl->GetSwitches(); sw++) {
        SWITCH_SET( channels.switches, sw, inputImpl->GetSwitchValue( sw));
    }
}