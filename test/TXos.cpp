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
 *            +5V    1  
 *            GND    2
 * D10        CS     3
 * RESET             4
 * D9         A0/DC  5
 * MOSI              6
 * SCK               7
 *            +3.3V  8
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

const uint8_t AnalogPins[] = {
  A0,A1,A2,A3,A4,A5
};

const uint8_t SwitchPins[] = {
  6,7,8,9,10,11
};

InputImpl *inputImpl;
OutputImpl *outputImpl;
DisplayImpl *displayImpl;

#undef ENABLE_MEMDEBUG

#ifdef ENABLE_MEMDEBUG

/* ATMega 328 p
 *
 *    0 -   31 Register File (32)
 *   32 -   95 Standard IO Memory (64)
 *   96 -  255 Extended IO Memory (160)
 *  256 - 2303 SRAM (2048)
 */

/* Start of free memory in SRAM */
#define freeStart memdebug[0]
/* Stackpointer */
#define stackLow memdebug[1]
/* Gap size or initial free bytes */
#define gapSize memdebug[2]
/* Free bytes determined by free space check */
#define gapFree memdebug[3]

/* Initialize every byte within the gap between
 * freeStart and stackLow with bit pattern 01010101
 */
#define MEMDEBUG_INIT()                       \
do {                                          \
    uint16_t i;                               \
    cli();                                    \
    gapFree = 0;                              \
    freeStart = (size_t)malloc( 1);           \
    stackLow = SPH << 8 | SPL;                \
    gapSize = stackLow - freeStart - 2;       \
    for( i=0; i<gapSize; i++) {               \
        *(byte*)(freeStart+i) = 0x55;         \
    }                                         \
    sei();                                    \
} while( false)

/* Check how many bytes are still free.
 * (Unchanged pattern 01010101)
 */
#define MEMDEBUG_CHECK()                      \
do {                                          \
    uint16_t i;                               \
    gapFree = 0;                              \
    for( i=0; i<gapSize; i++) {               \
        if( *(byte*)(freeStart+i) != 0x55) {  \
            break;                            \
        } else {                              \
            gapFree++;                        \
        }                                     \
    }                                         \
} while( false)

size_t memdebug[4];

#endif

#endif

Controls controls;
Output output;
UserInterface userInterface;
ConfigBlock configBlock;
SystemConfig systemConfig( configBlock);
ModuleManager moduleManager( configBlock);

void setup( void) {

#if defined( ARDUINO )

    inputImpl = new InputImpl( 6, AnalogPins,
                               6, SwitchPins);
    outputImpl = new OutputImpl();
    displayImpl = new DisplayImpl();

#ifdef ENABLE_MEMDEBUG
    Serial.begin(19200);
#endif

#endif

    userInterface.init();

    systemConfig.load();

    /* The order of modules is important.
     * It defines the order of execution in RunModules().
     */
    moduleManager.add( new ModelSelect());
    moduleManager.add( new Model());
    moduleManager.add( new EngineCut());
    moduleManager.add( new ServoReverse());
    moduleManager.add( new ServoSubtrim());
    moduleManager.add( new ServoLimit());

    moduleManager.load( systemConfig.getModelID());

#if defined( ARDUINO )
#ifdef ENABLE_MEMDEBUG
    MEMDEBUG_INIT();
#endif
#endif
}

void loop( void) {

    if( output.acceptChannels() ) {
        controls.GetControlValues( channels);
        moduleManager.runModules( channels);
        output.setChannels( channels);
        
#if defined( ARDUINO )
#ifdef ENABLE_MEMDEBUG
        MEMDEBUG_CHECK();
        Serial.println(gapFree);
#endif
#endif     
    }
    
    userInterface.handle();
}
