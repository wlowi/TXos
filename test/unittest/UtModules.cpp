
#include "Controls.h"
#include "InputImpl.h"

#include "ModuleManager.h"

#include "CalibrateSticks.h"
#include "CalibrateTrim.h"
#include "SwitchedChannels.h"

#include "AnalogSwitch.h"
#include "ChannelRange.h"
#include "ChannelReverse.h"
#include "AssignInput.h"

#include "UtModules.h"

extern InputImpl *inputImpl;
extern Controls controls;
extern ModuleManager moduleManager;

CalibrateSticks calibrateSticks;
CalibrateTrim calibrateTrim;
SwitchedChannels switchedChannels;

void UtModules::run() {

    std::cout << "*** UnitTest: UtModules" << std::endl;

    controls.init();

    std::cout << "*** Module: CalibrateSticks" << std::endl;

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
    

    std::cout << "*** Module: CalibrateTrim" << std::endl;

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

    std::cout << "*** Module: SwitchedChannels" << std::endl;

    switchedChannels.setDefaults();

    ASSERT_TEXT_T( switchedChannels.getName(), TEXT_MODULE_SWITCHED_CHANNELS, "switchedChannels.getName()");
    ASSERT_UINT8_T( switchedChannels.getConfigType(), MODULE_SWITCHED_CHANNELS_TYPE , "switchedChannels.getConfigType()");
    ASSERT_UINT8_T( switchedChannels.getRowCount(), 5 * SWITCHED_CHANNELS, "switchedChannels.getRowCount()");

    switchedChannels_t *switchedChannelsCFG = (switchedChannels_t*)switchedChannels.getConfig();

    std::cout << "*** Module: AnalogSwitch" << std::endl;

    AnalogSwitch analogSwitch;
    AssignInput assignInput;

    analogSwitch.setDefaults();

    ASSERT_TEXT_T( analogSwitch.getName(), TEXT_MODULE_ANALOG_SWITCH, "analogSwitch.getName()");
    ASSERT_UINT8_T( analogSwitch.getConfigType(), MODULE_ANALOG_SWITCH_TYPE , "analogSwitch.getConfigType()");
    ASSERT_UINT8_T( analogSwitch.getRowCount(), 2 * CHANNEL_SWITCHES, "analogSwitch.getRowCount()");

    analogSwitch_t *cfg = (analogSwitch_t*)analogSwitch.getConfig();
    cfg->source[0] = CHANNEL_THROTTLE;
    cfg->trigger[0] = -80;

    controls.GetControlValues();
    assignInput.run( controls);

    analogSwitch.run( controls);

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
