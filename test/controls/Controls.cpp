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

void Controls::init( switchSetConf_t conf) {

    for( switch_t sw = 0; sw < SWITCHES; sw++) {
        switchConf_t swConf = switchConfGet( conf, sw);

        if( swConf == SW_CONF_UNUSED) {
            swConf = SW_CONF_CONTROL;
        }
        
        LOGV( "Controls::init: Switch %d type %d\n", sw, swConf);

        switchConfSet( switchSetConf, sw, swConf);
    }

    inputImpl->init( switchSetConf);
}

void Controls::GetControlValues( channelSet_t &channels) const {

    for( channel_t ch = 0; ch < CHANNELS; ch++) {
        channels.analogChannel[ch] = CHANNELVALUE_MID;
    }

    for( channel_t ch = 0; ch < inputImpl->GetChannels(); ch++) {
        channels.analogChannel[ch] = inputImpl->GetChannelValue( ch);
    }

    channels.switchSetConf = switchSetConf;
    channels.switches = 0;
    for( switch_t sw = 0; sw < inputImpl->GetSwitches(); sw++) {
        switchSet( channels.switches, sw, inputImpl->GetSwitchValue( sw));
    }
}

void Controls::switchSet( switchSet_t &switches, switch_t sw, switchState_t value) {

    switches |= ((value & 0x03) << (sw << 1));
}

switchState_t Controls::switchGet( switchSet_t switches, switch_t sw) {

    return (switchState_t)((switches >> (sw << 1)) & 0x03);
}

void Controls::switchConfSet( switchSetConf_t &switchConf, switch_t sw, switchConf_t conf) {

    switchConf |= ((conf & 0x03) << (sw << 1));
}

switchConf_t Controls::switchConfGet( switchSetConf_t switchConf, switch_t sw) {

    return (switchConf_t)((switchConf >> (sw << 1)) & 0x03);
}

bool Controls::evalSwitches( const channelSet_t &channels, switchSet_t trigger) {

    bool state = false;

    for( switch_t sw = 0; sw < SWITCHES; sw++) {
        if( switchConfGet( channels.switchSetConf, sw) == SW_CONF_UNUSED) {
            continue;
        }

        if( switchGet( trigger, sw) == SW_STATE_DONTCARE) {
            continue;
        }

        if( switchGet( trigger, sw) != switchGet( channels.switches, sw) ) {
            state = false;
            break;
        } else {
            state = true;
        }
    }

    return state;
}