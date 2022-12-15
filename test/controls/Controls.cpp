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

void Controls::init() {

    inputImpl->init();
}

void Controls::GetControlValues() {

    channel_t ch;

    /* Clean values */
    for( ch = 0; ch < ANALOG_CHANNELS; ch++) {
        controlSet.inputChannel[ch] = CHANNELVALUE_MID;
    }

    for( ch = 0; ch < LOGICAL_CHANNELS; ch++) {
        controlSet.logicalChannel[ch] = CHANNELVALUE_MID;
    }

    for( ch = 0; ch < PPM_CHANNELS; ch++) {
        controlSet.outChannel[ch] = CHANNELVALUE_MID;
    }

    /* Read analog inputs */
    for( ch = 0; ch < PORT_ANALOG_INPUT_COUNT; ch++) {
        controlSet.stickADCChannel[ch] = inputImpl->GetStickValue( ch);
    }

    for( ch = 0; ch < PORT_TRIM_INPUT_COUNT; ch++) {
        controlSet.trimADCChannel[ch] = inputImpl->GetTrimValue( ch);
    }

    for( ch = 0; ch < PORT_AUX_INPUT_COUNT; ch++) {
        controlSet.auxADCChannel[ch] = inputImpl->GetAuxValue( ch);
    }

    /* Read switch inputs */
    for( switch_t sw = 0; sw < SWITCHES; sw++) {
        switchState_t state;

        if( sw < inputImpl->GetSwitches()) {
            state = inputImpl->GetSwitchValue( sw);
        } else if (switchConfGet(sw) == SW_CONF_FIXED_ON) {
            state = SW_STATE_1;
        } else {
            state = SW_STATE_0;
        }

        switchSet( sw, state);
    }
}

channelValue_t Controls::stickADCGet( channel_t ch) {

    return controlSet.stickADCChannel[ch];
}

channelValue_t Controls::trimADCGet( channel_t ch) {

    return controlSet.trimADCChannel[ch];
}

channelValue_t Controls::auxADCGet( channel_t ch) {

    return controlSet.auxADCChannel[ch];
}

void Controls::rangeSet( channel_t ch, percent_t value) {

    controlSet.range_pct[ch] = value;
}

percent_t Controls::rangeGet( channel_t ch) {

    return controlSet.range_pct[ch];
}

void Controls::inputSet( channel_t ch, channelValue_t value) {

    if( value > CHANNELVALUE_MAX_LIMIT) {
        value = CHANNELVALUE_MAX_LIMIT;
    }

    if( value < CHANNELVALUE_MIN_LIMIT) {
        value = CHANNELVALUE_MIN_LIMIT;
    }

    controlSet.inputChannel[ch] = value;
}

channelValue_t Controls::inputGet( channel_t ch) {

    return controlSet.inputChannel[ch];
}

void Controls::trimSet( channel_t ch, channelValue_t value) {

    if( value > CHANNELVALUE_MAX_LIMIT) {
        value = CHANNELVALUE_MAX_LIMIT;
    }

    if( value < CHANNELVALUE_MIN_LIMIT) {
        value = CHANNELVALUE_MIN_LIMIT;
    }

    controlSet.trimChannel[ch] = value;
}

channelValue_t Controls::trimGet( channel_t ch) {

    return controlSet.trimChannel[ch];
}

void Controls::logicalSet( channel_t ch, channelValue_t value) {

    if( value > CHANNELVALUE_MAX_LIMIT) {
        value = CHANNELVALUE_MAX_LIMIT;
    }

    if( value < CHANNELVALUE_MIN_LIMIT) {
        value = CHANNELVALUE_MIN_LIMIT;
    }

    controlSet.logicalChannel[ch] = value;
}

channelValue_t Controls::logicalGet( channel_t ch) {

    return controlSet.logicalChannel[ch];
}
void Controls::outputSet( channel_t ch, channelValue_t value) {

    if( value > CHANNELVALUE_MAX_LIMIT) {
        value = CHANNELVALUE_MAX_LIMIT;
    }

    if( value < CHANNELVALUE_MIN_LIMIT) {
        value = CHANNELVALUE_MIN_LIMIT;
    }

    controlSet.outChannel[ch] = value;
}

channelValue_t Controls::outputGet( channel_t ch) {

    return controlSet.outChannel[ch];
}

void Controls::switchSet( switch_t sw, switchState_t value) {

    uint8_t swn = GET_SWITCH( sw);

    if( swn < SWITCHES) {
        SET_SWITCH_STATE( controlSet.switches[swn], value);
    }
}

switchState_t Controls::switchGet( switch_t sw) {

    uint8_t swn = GET_SWITCH( sw);

    if( swn < SWITCHES) {
        return GET_SWITCH_STATE( controlSet.switches[swn]);
    } else {
        return SW_STATE_DONTCARE;
    }
}

switchConf_t Controls::switchConfGet( switch_t sw) {

    uint8_t swn = GET_SWITCH( sw);

    return inputImpl->GetSwitchConf( swn);
}

void Controls::copySwitchName( char *b, switch_t sw) {

    uint8_t swn = GET_SWITCH( sw);
    const char *swType;
    
    /* b must be of length TEXT_SW_NAME_length +1 */

    if( IS_SWITCH_UNUSED( sw)) {
        strcpy( b, TEXT_SW_TYPE_UNUSED);
    } else {
        b[0] = '1' + swn;
        switch (switchConfGet(sw)) {
        case SW_CONF_2STATE:
            swType = TEXT_SW_TYPE_2_STATE;
            break;

        case SW_CONF_3STATE:
            swType = TEXT_SW_TYPE_3_STATE;
            break;

        case SW_CONF_CHANNEL:
            swType = TEXT_SW_TYPE_CHANNEL;
            break;

        case SW_CONF_FIXED_ON:
            swType = TEXT_SW_TYPE_FIXED_ON;
            break;

        case SW_CONF_UNUSED:
        default:
            swType = TEXT_SW_TYPE_UNUSED;
            break;
        }
        strcpy( &b[1], swType);
    }
}

void Controls::copySwitchNameAndState( char *b, switch_t sw) {

    size_t len;

    /* b must be of length TEXT_SW_NAME_STATE_length +1 */

    copySwitchName( b, sw);

    if( IS_SWITCH_USED( sw)) {
        len = strlen( b);
        b[len++] = ':';
        b[len++] = '0' + GET_SWITCH_STATE( sw);
        b[len] = '\n';
    }
}

bool Controls::evalSwitches( switch_t trigger) {

    return IS_SWITCH_USED( trigger) 
            && GET_SWITCH_STATE( trigger) == GET_SWITCH_STATE( controlSet.switches[ GET_SWITCH(trigger)]);
}
