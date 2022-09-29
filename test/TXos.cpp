/*
 * TXos - RC Transmitter OS
 *
 * 
 * ==============================
 * Port usage
 * ==============================
 * 
 * PPM Output
 * ==========
 * 
 * PE3 D5    PPM Out
 * 
 * Switches
 * ========
 * 
 * PA0 D22   Digital 1
 * PA1 D23   Digital 2
 * PA2 D24   Digital 3
 * PA3 D25   Digital 4
 * PA4 D26   Digital 5
 * PA5 D27   Digital 6
 * PA6 D28
 * PA7 D29
 * 
 * Analog Input
 * ============
 * 
 * PF0 A0    Analog 1
 * PF1 A1    Analog 2
 * PF2 A2    Analog 3
 * PF3 A3    Analog 4
 * PF4 A4    Analog 5
 * PF5 A5    Analog 6
 * PF6 A6
 * PF7 A7
 * 
 * PK0 A8  PCINT16     Trim 1
 * PK1 A9  PCINT17     Trim 2
 * PK2 A10 PCINT18     Trim 3
 * PK3 A11 PCINT19     Trim 4
 * PK4 A12 PCINT20     ( Rotary Encoder A )
 * PK5 A13 PCINT21     ( Rotary Encoder B )
 * PK6 A14 PCINT22     ( Rotary Encoder Switch )
 * PK7 A15 PCINT23     Vcc Monitor
 * 
 * Rotary Encoder
 * ==============
 * 
 * PK2 A12 PCINT20     Rotary Encoder A
 * PK3 A13 PCINT21     Rotary Encoder B
 * PK4 A14 PCINT22     Rotary Encoder Switch
 * 
 * Buzzer
 * ======
 * PC6 D31             Buzzer
 * 
 * Relais
 * ======
 * PE5 D2              HF on
 * PE4 D3              Bind
 * 
 * Display
 * =======
 *                      +5V    1  
 *                      GND    2
 * PB4 D10 PCINT4       CS     3
 *     RESET            RESET  4
 * PH6 D9               A0/DC  5
 * PB2 MOSI             DATA   6
 * PB1 SCK              SCK    7
 *                      +3.3V  8
 * 
 * 
 * ==============================
 * Timer usage
 * ==============================
 * 
 * Timer 0   8 bit       Arduino micros() millis() delay()...
 * Timer 1  16 bit       PPM generation    OutputImpl.cpp
 * Timer 2   8 bit 
 * Timer 3  16 bit       Buzzer Sound      BuzzerImpl.cpp
 * Timer 4  16 bit 
 * Timer 5  16 bit 
 * 
 */

#include "TXos.h"

#include "EEPROM.h"

#include "Controls.h"
#include "Output.h"
#include "Ports.h"
#include "Buzzer.h"
#include "UserInterface.h"

#include "Module.h"
#include "ModuleManager.h"
#include "ConfigBlock.h"
#include "SystemConfig.h"

#include "ModelSelect.h"
#include "Model.h"
#include "ServoMonitor.h"
#include "SwitchMonitor.h"
#include "EngineCut.h"
#include "ServoReverse.h"
#include "ServoSubtrim.h"
#include "ServoLimit.h"

#if defined( ARDUINO )
#include "InputImpl.h"
#include "OutputImpl.h"
#include "PortsImpl.h"
#include "BuzzerImpl.h"
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

const buzzerCmd_t SoundWelcome[] = {
  BUZZER_PLAY( 2),
  BUZZER_PAUSE( 2),
  BUZZER_REPEAT( 0, 3),
  BUZZER_STOP()
};

#if defined( ARDUINO )

const uint8_t AnalogPins[] = {
  PORT_ANALOG_INPUT
};

const uint8_t SwitchPins[] = {
  PORT_SWITCH_INPUT
};

InputImpl *inputImpl;
OutputImpl *outputImpl;
PortsImpl *portsImpl;
BuzzerImpl *buzzerImpl;
DisplayImpl *displayImpl;

#undef ENABLE_MEMDEBUG
#undef ENABLE_TIMING
#undef ENABLE_BDEBUG

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
Ports ports;
Buzzer buzzer( ports);
UserInterface userInterface;
ConfigBlock configBlock;
SystemConfig systemConfig( configBlock);
ModuleManager moduleManager( configBlock);


#ifdef ENABLE_BDEBUG
uint8_t bdebugi = 0;
char bdebug[ BDEBUG_LEN ];
#endif


void setup( void) {

#if defined( ARDUINO )

    inputImpl = new InputImpl( PORT_ANALOG_INPUT_COUNT, AnalogPins,
                               PORT_SWITCH_INPUT_COUNT, SwitchPins);
                               
    outputImpl = new OutputImpl();
    portsImpl = new PortsImpl();
    buzzerImpl = new BuzzerImpl();
    displayImpl = new DisplayImpl();

#if defined( ENABLE_MEMDEBUG ) || defined( ENABLE_TIMING ) || defined( ENABLE_BDEBUG )
    Serial.begin(19200);
#endif

#ifdef ENABLE_BDEBUG
  bdebugi = 0;
#endif

#endif
    ports.init();
    buzzer.init();
    userInterface.init();

    systemConfig.load();

    /* The order of modules is important.
     * It defines the order of execution in RunModules().
     */
    moduleManager.add( new ModelSelect());
    moduleManager.add( new Model());
    moduleManager.add( new ServoMonitor( controls));
    moduleManager.add( new SwitchMonitor( controls));
    moduleManager.add( new EngineCut());
    moduleManager.add( new ServoReverse());
    moduleManager.add( new ServoSubtrim());
    moduleManager.add( new ServoLimit());

    moduleManager.load( systemConfig.getModelID());

    controls.init( SWITCH_CONFIGURATION);

#if defined( ARDUINO )
#ifdef ENABLE_MEMDEBUG
    MEMDEBUG_INIT();
#endif
#endif

    buzzer.play( SoundWelcome);
}

void loop( void) {
#ifdef ENABLE_TIMING
    unsigned long tstamp;
#endif

    if( output.acceptChannels() ) {
#ifdef ENABLE_TIMING
        tstamp = micros();
#endif
        controls.GetControlValues();
        moduleManager.runModules( controls);
        output.setChannels( controls);
#ifdef ENABLE_TIMING
        tstamp = micros() - tstamp;
        Serial.print("M:");
        Serial.println(tstamp);
#endif
        
#if defined( ARDUINO )
#ifdef ENABLE_MEMDEBUG
        MEMDEBUG_CHECK();
        Serial.println(gapFree);
#endif
#endif     
    }
    
#ifdef ENABLE_TIMING
    tstamp = micros();
#endif
    userInterface.handle();
#ifdef ENABLE_TIMING
    tstamp = micros() - tstamp;
    Serial.print("U:");
    Serial.println(tstamp);
#endif
}
