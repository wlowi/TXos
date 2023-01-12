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

#include "Controls.h"
#include "InputImpl.h"

#include "ModuleManager.h"

#include "CalibrateSticks.h"
#include "CalibrateTrim.h"
#include "AnalogTrim.h"
#include "SwitchedChannels.h"
#include "AnalogSwitch.h"
#include "ChannelRange.h"
#include "ChannelReverse.h"

#include "AssignInput.h"
#include "ChannelDelay.h"

#include "UtModules.h"

extern InputImpl *inputImpl;
extern Controls controls;
extern ModuleManager moduleManager;

CalibrateSticks calibrateSticks;
CalibrateTrim calibrateTrim;
AnalogTrim analogTrim;
SwitchedChannels switchedChannels;
AnalogSwitch analogSwitch;
ChannelRange channelRange;
ChannelReverse channelReverse;

AssignInput assignInput;
ChannelDelay channelDelay;

DECLARE_ASSERT_COUNTER

void UtModules::run() {

    std::cout << "*** UnitTest: UtModules" << std::endl;

    controls.init();

    UtCalibrateSticks();
    UtCalibrateTrim();
    UtAnalogTrim();
    UtSwitchedChannels();
    UtAnalogSwitch();
    UtChannelRange();
    UtChannelReverse();

    UtAssignInput();
    UtChannelDelay();
    // UtPhases
    // UtLogicSwitch
    // UtDualExpo
    // UtModel
    // UtMixer
    // UtPhasesTrim
    // UtEngineCut

    // UtServoRemap
    // UtServoReverse
    // UtServoSubtrim
    // UtServoLimit

    // dumpControls( controls);

    std::cout << std::endl << "*** UnitTest: END UtModules" << std::endl;
    std::cout << "OK     = " << okCount << std::endl;
    std::cout << "FAILED = " << failCount << std::endl;
}

void UtModules::UtCalibrateSticks() {

    std::cout << std::endl << "*** Module: CalibrateSticks" << std::endl;

    calibrateSticks.setDefaults();

    ASSERT_TEXT_T( calibrateSticks.getName(), TEXT_MODULE_CAL_STICKS, "calibrateSticks.getName()");
    ASSERT_UINT8_T( calibrateSticks.getConfigType(), MODULE_CAL_STICKS_TYPE , "calibrateSticks.getConfigType()");
    ASSERT_UINT8_T( calibrateSticks.getRowCount(), PORT_ANALOG_INPUT_COUNT +1, "calibrateSticks.getRowCount()");

    calibrateSticks_t *calibrateSticksCFG = (calibrateSticks_t*)calibrateSticks.getConfig();

    /* Set min, mid and max values of ADC output values. 
     * Assume ADC range is from 300 to 700 with mid position 500 */
    for( channel_t ch=0; ch<PORT_ANALOG_INPUT_COUNT; ch++) {
        calibrateSticksCFG->minPos[ch] = 300;
        calibrateSticksCFG->midPos[ch] = 500;
        calibrateSticksCFG->maxPos[ch] = 700;
    }

    moduleManager.addToRunList( &calibrateSticks);

    verify( 0, PORT_ANALOG_INPUT_COUNT, 300, -1250);
    verify( 0, PORT_ANALOG_INPUT_COUNT, 500, 0);
    verify( 0, PORT_ANALOG_INPUT_COUNT, 700, 1250);
}

void UtModules::UtCalibrateTrim() {

    std::cout << std::endl << "*** Module: CalibrateTrim" << std::endl;

    calibrateTrim.setDefaults();

    ASSERT_TEXT_T( calibrateTrim.getName(), TEXT_MODULE_CAL_TRIM, "calibrateTrim.getName()");
    ASSERT_UINT8_T( calibrateTrim.getConfigType(), MODULE_CAL_TRIM_TYPE , "calibrateTrim.getConfigType()");
    ASSERT_UINT8_T( calibrateTrim.getRowCount(), PORT_TRIM_INPUT_COUNT +1, "calibrateTrim.getRowCount()");

    calibrateTrim_t *calibrateTrimCFG = (calibrateTrim_t*)calibrateTrim.getConfig();

    /* Set min, mid and max values of ADC output values. 
     * Assume ADC range is from 300 to 700 with mid position 500 */
    for( channel_t ch=0; ch<PORT_TRIM_INPUT_COUNT; ch++) {
        calibrateTrimCFG->minPos[ch] = 300;
        calibrateTrimCFG->midPos[ch] = 500;
        calibrateTrimCFG->maxPos[ch] = 700;
    }

    moduleManager.addToRunList( &calibrateTrim);
}

void UtModules::UtAnalogTrim() {

    std::cout << std::endl << "*** Module: AnalogTrim" << std::endl;

    analogTrim.setDefaults();

    ASSERT_TEXT_T( analogTrim.getName(), TEXT_MODULE_ANALOG_TRIM, "analogTrim.getName()");
    ASSERT_UINT8_T( analogTrim.getConfigType(), MODULE_ANALOG_TRIM_TYPE , "analogTrim.getConfigType()");
    ASSERT_UINT8_T( analogTrim.getRowCount(), PORT_TRIM_INPUT_COUNT, "analogTrim.getRowCount()");

    // analogTrim_t *analogTrimCFG = (analogTrim_t*)analogTrim.getConfig();

    moduleManager.addToRunList( &analogTrim);

    std::cout << "Trim value 500" << std::endl;

    for( channel_t ch=0; ch<PORT_TRIM_INPUT_COUNT; ch++) {
        inputImpl->unittestSetTrimValue( ch, 500);
    }

    verify( 0, PORT_ANALOG_INPUT_COUNT, 300, -1250);
    verify( 0, PORT_ANALOG_INPUT_COUNT, 500, 0);
    verify( 0, PORT_ANALOG_INPUT_COUNT, 700, 1250);

    std::cout << "Trim value 300" << std::endl;

    for( channel_t ch=0; ch<PORT_TRIM_INPUT_COUNT; ch++) {
        inputImpl->unittestSetTrimValue( ch, 300);
    }

    verify( 0, PORT_TRIM_INPUT_COUNT, 300, -1250);
    verify( 0, PORT_TRIM_INPUT_COUNT, 500, -250);
    verify( 0, PORT_TRIM_INPUT_COUNT, 700, 1000);

    std::cout << "Trim value 700" << std::endl;

    for( channel_t ch=0; ch<PORT_TRIM_INPUT_COUNT; ch++) {
        inputImpl->unittestSetTrimValue( ch, 700);
    }

    verify( 0, PORT_TRIM_INPUT_COUNT, 300, -1000);
    verify( 0, PORT_TRIM_INPUT_COUNT, 500, 250);
    verify( 0, PORT_TRIM_INPUT_COUNT, 700, 1250);

    std::cout << "Reset trim value to 500" << std::endl;

    for( channel_t ch=0; ch<PORT_TRIM_INPUT_COUNT; ch++) {
        inputImpl->unittestSetTrimValue( ch, 500);
    }
}

void UtModules::UtSwitchedChannels() {

    std::cout << std::endl << "*** Module: SwitchedChannels" << std::endl;

    switchedChannels.setDefaults();

    ASSERT_TEXT_T( switchedChannels.getName(), TEXT_MODULE_SWITCHED_CHANNELS, "switchedChannels.getName()");
    ASSERT_UINT8_T( switchedChannels.getConfigType(), MODULE_SWITCHED_CHANNELS_TYPE , "switchedChannels.getConfigType()");
    ASSERT_UINT8_T( switchedChannels.getRowCount(), 4 * SWITCHED_CHANNELS, "switchedChannels.getRowCount()");

    switchedChannels_t *switchedChannelsCFG = (switchedChannels_t*)switchedChannels.getConfig();
    moduleManager.addToRunList( &switchedChannels);

    switchedChannelsCFG->sw[0] = 1; // 2 state 
    switchedChannelsCFG->sw[1] = 2; // 3 state
    switchedChannelsCFG->sw[2] = 3; // 3 state 
        
    for( uint8_t i=0; i<SWITCHED_CHANNELS; i++) {
        switchedChannelsCFG->state0_pct[i] = -80;
        switchedChannelsCFG->state1_pct[i] = 0;
        switchedChannelsCFG->state2_pct[i] = 80;
    }

    inputImpl->unittestSetSwitchValues( SW_STATE_0, SW_STATE_0, SW_STATE_0, SW_STATE_0);
    controls.GetControlValues();
    moduleManager.runModules( controls);

    ASSERT_INT16_T( controls.inputGet( 6), -800, "Sw state 0");
    ASSERT_INT16_T( controls.inputGet( 7), -800, "Sw state 0");
    ASSERT_INT16_T( controls.inputGet( 8), -800, "Sw state 0");

    inputImpl->unittestSetSwitchValues( SW_STATE_0, SW_STATE_1, SW_STATE_1, SW_STATE_1);
    controls.GetControlValues();
    moduleManager.runModules( controls);

    ASSERT_INT16_T( controls.inputGet( 6), 0, "Sw state 1");
    ASSERT_INT16_T( controls.inputGet( 7), 0, "Sw state 1");
    ASSERT_INT16_T( controls.inputGet( 8), 0, "Sw state 1");

    inputImpl->unittestSetSwitchValues( SW_STATE_0, SW_STATE_1, SW_STATE_2, SW_STATE_2);
    controls.GetControlValues();
    moduleManager.runModules( controls);

    ASSERT_INT16_T( controls.inputGet( 6), 0, "Sw state 0");
    ASSERT_INT16_T( controls.inputGet( 7), 800, "Sw state 0");
    ASSERT_INT16_T( controls.inputGet( 8), 800, "Sw state 0");
}

void UtModules::UtAnalogSwitch() {

    std::cout << std::endl << "*** Module: AnalogSwitch" << std::endl;

    analogSwitch.setDefaults();

    ASSERT_TEXT_T( analogSwitch.getName(), TEXT_MODULE_ANALOG_SWITCH, "analogSwitch.getName()");
    ASSERT_UINT8_T( analogSwitch.getConfigType(), MODULE_ANALOG_SWITCH_TYPE , "analogSwitch.getConfigType()");
    ASSERT_UINT8_T( analogSwitch.getRowCount(), 2 * CHANNEL_SWITCHES, "analogSwitch.getRowCount()");

    analogSwitch_t *analogSwitchCFG = (analogSwitch_t*)analogSwitch.getConfig();
    moduleManager.addToRunList( &analogSwitch);

    analogSwitchCFG->source[0] = 0;
    analogSwitchCFG->trigger_pct[0] = -80;

    analogSwitchCFG->source[1] = 0;
    analogSwitchCFG->trigger_pct[1] = 0;

    // -81%
    inputImpl->unittestSetStickValue( 0, 500 - (200 * 81 / 100)); // ! this is range 300 - 700 for -125% to 125% 
    controls.GetControlValues();
    moduleManager.runModules( controls);
    
    ASSERT_INT8_T( controls.switchGet( CHANNEL_SWITCHES_FIRST_IDX), SW_STATE_0, "Switch state");
    ASSERT_INT8_T( controls.switchGet( CHANNEL_SWITCHES_FIRST_IDX +1), SW_STATE_0, "Switch state");

    // -79%
    inputImpl->unittestSetStickValue( 0, 500 - (200 * 79 / 100)); // ! this is range 300 - 700 for -125% to 125% 
    controls.GetControlValues();
    moduleManager.runModules( controls);
    
    ASSERT_INT8_T( controls.switchGet( CHANNEL_SWITCHES_FIRST_IDX), SW_STATE_1, "Switch state");
    ASSERT_INT8_T( controls.switchGet( CHANNEL_SWITCHES_FIRST_IDX +1), SW_STATE_0, "Switch state");

    // -1%
    inputImpl->unittestSetStickValue( 0, 500 - (200 * 1 / 100)); // ! this is range 300 - 700 for -125% to 125% 
    controls.GetControlValues();
    moduleManager.runModules( controls);
    
    ASSERT_INT8_T( controls.switchGet( CHANNEL_SWITCHES_FIRST_IDX), SW_STATE_1, "Switch state");
    ASSERT_INT8_T( controls.switchGet( CHANNEL_SWITCHES_FIRST_IDX +1), SW_STATE_0, "Switch state");

    // +1%
    inputImpl->unittestSetStickValue( 0, 500 + (200 * 1 / 100)); // ! this is range 300 - 700 for -125% to 125% 
    controls.GetControlValues();
    moduleManager.runModules( controls);
    
    ASSERT_INT8_T( controls.switchGet( CHANNEL_SWITCHES_FIRST_IDX), SW_STATE_1, "Switch state");
    ASSERT_INT8_T( controls.switchGet( CHANNEL_SWITCHES_FIRST_IDX +1), SW_STATE_1, "Switch state");
}

void UtModules::UtChannelRange() {

    std::cout << std::endl << "*** Module: ChannelRange" << std::endl;

    channelRange.setDefaults();

    ASSERT_TEXT_T( channelRange.getName(), TEXT_MODULE_CHANNEL_RANGE, "channelRange.getName()");
    ASSERT_UINT8_T( channelRange.getConfigType(), MODULE_CHANNEL_RANGE_TYPE , "channelRange.getConfigType()");
    ASSERT_UINT8_T( channelRange.getRowCount(), PORT_ANALOG_INPUT_COUNT, "channelRange.getRowCount()");

    channelRange_t *channelRangeCFG = (channelRange_t*)channelRange.getConfig();
    moduleManager.addToRunList( &channelRange);

    /* After setDefaults() all channels scaled to +/- 100%*/
    verify( 0, PORT_ANALOG_INPUT_COUNT, 300, -1000);
    verify( 0, PORT_ANALOG_INPUT_COUNT, 700, 1000);

    /* Unlimit to 125% */
    channelRangeCFG->range_pct[0] = 125;
    verify( 0, 1, 300, -1250);
    verify( 0, 1, 700, 1250);

    /* Limit to 50% */
    channelRangeCFG->range_pct[0] = 50;
    verify( 0, 1, 300, -500);
    verify( 0, 1, 700, 500);

    /* Reset to +/- 100% */
    std::cout << "Reset channel range to +/- 100%" << std::endl;
    channelRangeCFG->range_pct[0] = 100;
}

void UtModules::UtChannelReverse() {

    std::cout << std::endl << "*** Module: ChannelReverse" << std::endl;

    channelReverse.setDefaults();

    ASSERT_TEXT_T( channelReverse.getName(), TEXT_MODULE_CHANNEL_REVERSE, "channelReverse.getName()");
    ASSERT_UINT8_T( channelReverse.getConfigType(), MODULE_CHANNEL_REVERSE_TYPE , "channelReverse.getConfigType()");
    ASSERT_UINT8_T( channelReverse.getRowCount(), PORT_ANALOG_INPUT_COUNT, "channelReverse.getRowCount()");

    channelReverse_t *channelReverseCFG = (channelReverse_t*)channelReverse.getConfig();
    moduleManager.addToRunList( &channelReverse);

    /* Default unreversed */
    verify( 0, 1, 300, -1000);
    verify( 0, 1, 700, 1000);

    /* Reverse all */
    channelReverseCFG->revBits = 0xffff;
    verify( 0, 1, 300, 1000);
    verify( 0, 1, 700, -1000);

    /* Back to normal */
    channelReverseCFG->revBits = 0;
    verify( 0, 1, 300, -1000);
    verify( 0, 1, 700, 1000);
}

void UtModules::UtAssignInput() {

    std::cout << std::endl << "*** Module: AssignInput" << std::endl;

    assignInput.setDefaults();

    ASSERT_TEXT_T( assignInput.getName(), TEXT_MODULE_ASSIGN_INPUT, "assignInput.getName()");
    ASSERT_UINT8_T( assignInput.getConfigType(), MODULE_ASSIGN_INPUT_TYPE , "assignInput.getConfigType()");
    ASSERT_UINT8_T( assignInput.getRowCount(), MIX_CHANNELS, "assignInput.getRowCount()");

    assignInput_t *assignInputCFG = (assignInput_t*)assignInput.getConfig();
    moduleManager.addToRunList( &assignInput);


    for( channel_t ch=0; ch<MIX_CHANNELS; ch++) {
        assignInputCFG->source[ch] = ch % 6; // use only the first 6 analog channels
    }

    /* Set analog channels 0 - 5 */
    inputImpl->unittestSetStickValue( 0, 500 + (200 * 10 / 100)); // ! this is range 300 - 700 for -125% to 125% 
    inputImpl->unittestSetStickValue( 1, 500 + (200 * 20 / 100)); // set to 10% ... 60%
    inputImpl->unittestSetStickValue( 2, 500 + (200 * 30 / 100));
    inputImpl->unittestSetStickValue( 3, 500 + (200 * 40 / 100));
    inputImpl->unittestSetStickValue( 4, 500 + (200 * 50 / 100));
    inputImpl->unittestSetStickValue( 5, 500 + (200 * 60 / 100));

    controls.GetControlValues();
    moduleManager.runModules( controls);

    for( channel_t ch=0; ch<MIX_CHANNELS; ch++) {
        //                                                       100,200,300,400,500,600,100,200,...
        ASSERT_INT16_T( controls.logicalGet( ch), 100 + (ch % 6) * 100, "verify assignment");
    }


    /* Now change assignment by shifting input channels by 1 */
    for( channel_t ch=0; ch<MIX_CHANNELS; ch++) {
        assignInputCFG->source[ch] = (ch + 1) % 6; // use only the first 6 analog channels
    }

    controls.GetControlValues();
    moduleManager.runModules( controls);

    for( channel_t ch=0; ch<MIX_CHANNELS; ch++) {
        //                                                       100,200,300,400,500,600,100,200,...
        ASSERT_INT16_T( controls.logicalGet( ch), 100 + ((ch + 1) % 6) * 100, "verify shifted assignment");
    }

    /* set back to default */
    assignInput.setDefaults();
}

void UtModules::UtChannelDelay() {

    std::cout << std::endl << "*** Module: ChannelDelay" << std::endl;

    channelDelay.setDefaults();

    ASSERT_TEXT_T( channelDelay.getName(), TEXT_MODULE_CHANNEL_DELAY, "channelDelay.getName()");
    ASSERT_UINT8_T( channelDelay.getConfigType(), MODULE_CHANNEL_DELAY_TYPE , "channelDelay.getConfigType()");
    ASSERT_UINT8_T( channelDelay.getRowCount(), MIX_CHANNELS, "channelDelay.getRowCount()");

    // channelDelay_t *channelDelayCFG = (channelDelay_t*)channelDelay.getConfig();
    moduleManager.addToRunList( &channelDelay);
}

void UtModules::verify( channel_t start, uint8_t count, channelValue_t in, channelValue_t expected) {

    for( channel_t ch=start; ch<start+count; ch++) {
        inputImpl->unittestSetStickValue( ch, in);
    }

    controls.GetControlValues();
    moduleManager.runModules( controls);

    for( channel_t ch=start; ch<start+count; ch++) {
        ASSERT_INT16_T( controls.inputGet( ch), expected, "verify()");
    }
}

void UtModules::dumpControls( Controls &controls) {

    /* Raw analog input channels */
    for( uint8_t i = 0; i<PORT_ANALOG_INPUT_COUNT; i++) {
        printf("  adc stick[%d] %d\n", i, controls.stickADCGet(i));
    }
    for( uint8_t i = 0; i<PORT_ANALOG_INPUT_COUNT; i++) {
        printf("  adc range[%d] %d\n", i, controls.rangeGet(i));
    }
    for( uint8_t i = 0; i<PORT_TRIM_INPUT_COUNT; i++) {
        printf("  adc trim[%d] %d\n", i, controls.trimADCGet(i));
    }
    for( uint8_t i = 0; i<PORT_AUX_INPUT_COUNT; i++) {
        printf("  adc aux[%d] %d\n", i, controls.auxADCGet(i));
    }

    /* Calibrated analog input channels */
    for( uint8_t i = 0; i<ANALOG_CHANNELS; i++) {
        printf("  calibrated input[%d] %d\n", i, controls.inputGet(i));
    }
    for( uint8_t i = 0; i<PORT_TRIM_INPUT_COUNT; i++) {
        printf("  calibrated trim[%d] %d\n", i, controls.trimGet(i));
    }

    /* Mixed channels */
    for( uint8_t i = 0; i<LOGICAL_CHANNELS; i++) {
        printf("  logical channel[%d] %d\n", i, controls.logicalGet(i));
    }

    /* Calibrated and mixed analog channels */
    for( uint8_t i = 0; i<PPM_CHANNELS; i++) {
        printf("  output[%d] %d\n", i, controls.outputGet(i));
    }

    for( uint8_t i = 0; i<SWITCHES; i++) {
        printf("  switch[%d] %d\n", i, controls.switchGet(i));
    }
}
