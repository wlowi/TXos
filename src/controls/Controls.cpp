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

/*
    Interface code to the actual implementation of input controls.
 */

#include "Controls.h"
#include "InputImpl.h"

extern InputImpl *inputImpl;

Controls::Controls() = default;

void Controls::init() {

    //inputImpl->init();
}

void Controls::GetControlValues() {

    channel_t ch;

    /* Clean values */
    for( ch = 0; ch < INPUT_CHANNELS; ch++) {
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

    // LOGV("Controls::switchSet(): sw=%d state=%d\n", swn, value);

    if( swn < SWITCHES) {
        SET_SWITCH_STATE( controlSet.switches[swn], value);
    }
}

switchState_t Controls::switchGet( switch_t sw) {

    uint8_t swn = GET_SWITCH( sw);
    switchState_t state;

    if( swn < SWITCHES) {
        state = GET_SWITCH_STATE( controlSet.switches[swn]);
    } else {
        state = SW_STATE_DONTCARE;
    }

    // LOGV("Controls::switchGet(): sw=%d state=%d\n", swn, state);

    return state;
}

switchConf_t Controls::switchConfGet( switch_t sw) {

    uint8_t swn = GET_SWITCH( sw);

    return inputImpl->GetSwitchConf( swn);
}

switch_t Controls::getSwitchByType( switchConf_t type, uint8_t idx) {

    switch_t sw;
    
    // Set switch to unused
    INIT_SWITCH( sw);
    
    for( switch_t i = 0; i < SWITCHES; i++) {
        if(  switchConfGet(i) == type) {
            if(idx == 0) {
                sw = i;
                break;
            } else {
                idx--;
            }
        }
    }

    return sw;
}

void Controls::copySwitchName( char *b, switch_t sw) {

    uint8_t swn = GET_SWITCH( sw);
    size_t idx = 0;
    const char *swType;
    
    /* b must be of length TEXT_SW_NAME_length +1 */

    if( IS_SWITCH_UNUSED( sw)) {
        strcpy( b, TEXT_SW_TYPE_UNUSED);
    } else {

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

        case SW_CONF_LOGIC:
            swType = TEXT_SW_TYPE_LOGIC;
            break;

        case SW_CONF_PHASES:
            swType = TEXT_SW_TYPE_PHASE;
            break;

        case SW_CONF_PHASE_N:
            swType = TEXT_SW_TYPE_PHASE_N;
            break;

        case SW_CONF_UNUSED:
        default:
            swType = TEXT_SW_TYPE_UNUSED;
            break;
        }
        strcpy( &b[idx], swType);
        idx = strlen( b);

        switch (switchConfGet(sw)) {
        case SW_CONF_2STATE:
            b[idx++] = '1' + swn - MECHANICAL_SWITCHES_FIRST_IDX;
            break;

        case SW_CONF_3STATE:
            b[idx++] = '1' + swn - MECHANICAL_SWITCHES_FIRST_IDX;
            break;

        case SW_CONF_CHANNEL:
            b[idx++] = '1' + swn - CHANNEL_SWITCHES_FIRST_IDX;
            break;

        case SW_CONF_LOGIC:
            b[idx++] = '1' + swn - LOGIC_SWITCHES_FIRST_IDX;
            break;

        case SW_CONF_PHASE_N:
            b[idx++] = '1' + swn - PHASE_SWITCHES_FIRST_IDX;
            break;

        default:
            break;
        }

        b[idx] = '\0';
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
        b[len] = '\0';
    }
}

bool Controls::evalSwitches( switch_t trigger) {

    return IS_SWITCH_USED( trigger) 
            && GET_SWITCH_STATE( trigger) == GET_SWITCH_STATE( controlSet.switches[ GET_SWITCH(trigger)]);
}
