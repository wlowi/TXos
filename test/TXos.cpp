/*
 * TXos - RC Transmitter OS
 *
 * Output
 * ======
 * 
 * D5   PPM Out
 * 
 * Switches
 * ========
 * 
 * D6   Digital 1
 * D7   Digital 2
 * D8   Digital 3
 * D9   Digital 4
 * D10  Digital 5
 * D11  Digital 6
 * 
 * Analog Input
 * ============
 * 
 * A0   Analog 1
 * A1   Analog 2
 * A2   Analog 3
 * A3   Analog 4
 * A4   Analog 5
 * A5   Analog 6
 * A7   Vcc Monitor
 * 
 * Rotary Encoder
 * ==============
 * 
 * A10  Rotary Encoder A
 * A11  Rotary Encoder B
 * A12  Rotary Encoder Switch
 * 
 * Display
 * =======
 * MOSI
 * SCK
 * D
 * D
 * 
 */

#include "TXos.h"

#include "EEPROM.h"

#include "Controls.h"
#include "Output.h"
#include "UserInterface.h"

#include "Module.h"
#include "ModuleManager.h"
#include "ConfigBlock.h"
#include "SystemConfig.h"

#include "ModelSelect.h"
#include "Model.h"
#include "EngineCut.h"
#include "ServoReverse.h"
#include "ServoSubtrim.h"
#include "ServoLimit.h"

#if defined( ARDUINO )
#include "InputImpl.h"
#include "OutputImpl.h"
#include "DisplayImpl.h"
#endif

const char *ChannelNames[CHANNELS] = {
    TEXT_CONTROL_CHANNEL_1,
    TEXT_CONTROL_CHANNEL_2,
    TEXT_CONTROL_CHANNEL_3,
    TEXT_CONTROL_CHANNEL_4,
    TEXT_CONTROL_CHANNEL_5,
    TEXT_CONTROL_CHANNEL_6,
    TEXT_CONTROL_CHANNEL_7,
    TEXT_CONTROL_CHANNEL_8,
    TEXT_CONTROL_CHANNEL_9
};

channelSet_t channels;

#if defined( ARDUINO )
InputImpl *inputImpl;
OutputImpl *outputImpl;
DisplayImpl *displayImpl;
#endif

Controls controls;
Output output;
UserInterface userInterface;
ConfigBlock configBlock;
SystemConfig systemConfig( configBlock);
ModuleManager moduleManager( configBlock);

void setup( void) {

#if defined( ARDUINO )
    inputImpl = new InputImpl( 6, 6);
    outputImpl = new OutputImpl( CHANNELS);
    displayImpl = new DisplayImpl();
#endif

    userInterface.init();

    systemConfig.load();

    /* The order of modules is important.
     * It defines the order of execution in RunModules().
     */
    moduleManager.Add( new ModelSelect());
    moduleManager.Add( new Model());
    moduleManager.Add( new EngineCut());
    moduleManager.Add( new ServoReverse());
    moduleManager.Add( new ServoSubtrim());
    moduleManager.Add( new ServoLimit());

    moduleManager.load( systemConfig.getModelID());

    controls.init();
    output.init();
}

void loop( void) {

#if defined( ARDUINO )
    digitalWrite( LED_BUILTIN, HIGH);
    delay( 11);
    digitalWrite( LED_BUILTIN, LOW);
    delay( 11);
#endif

    controls.GetControlValues( channels);
    moduleManager.RunModules( channels);
    output.setChannels( channels);

    userInterface.handle();
}
