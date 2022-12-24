
#include "UtModules.h"
#include "Controls.h"

#include "AnalogSwitch.h"

void UtModules::run() {

    std::cout << "*** UnitTest: UtModules" << std::endl;

    std::cout << "*** Module: AnalogSwitch" << std::endl;

    AnalogSwitch analogSwitch;
    analogSwitch.setDefaults();

    ASSERT_TEXT_T( analogSwitch.getName(), TEXT_MODULE_ANALOG_SWITCH, "analogSwitch.getName()");
    ASSERT_UINT8_T( analogSwitch.getConfigType(), MODULE_ANALOG_SWITCH_TYPE , "analogSwitch.getConfigType()");
    ASSERT_UINT8_T( analogSwitch.getRowCount(), 2 * CHANNEL_SWITCHES, "analogSwitch.getRowCount()");
}