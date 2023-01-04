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
 * PF0 D54 A0    Analog 1
 * PF1 D55 A1    Analog 2
 * PF2 D56 A2    Analog 3
 * PF3 D57 A3    Analog 4
 * PF4 D58 A4    Analog 5
 * PF5 D59 A5    Analog 6
 * PF6 D60 A6
 * PF7 D61 A7
 * 
 * PK0 D62 A8  PCINT16     Trim 1
 * PK1 D63 A9  PCINT17     Trim 2
 * PK2 D64 A10 PCINT18     Trim 3
 * PK3 D65 A11 PCINT19     Trim 4
 * PK4 D66 A12 PCINT20     ( Rotary Encoder A ).
 * PK5 D67 A13 PCINT21     ( Rotary Encoder B )
 * PK6 D68 A14 PCINT22     ( Rotary Encoder Switch )
 * PK7 D69 A15 PCINT23     Vcc Monitor
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
#include "SystemSetup.h"
#include "CalibrateSticks.h"
#include "CalibrateTrim.h"
#include "Bind.h"
#include "RangeTest.h"
#include "DualExpo.h"
#include "Phases.h"
#include "Timer.h"
#include "VccMonitor.h"
#include "PhasesTrim.h"
#include "SwitchedChannels.h"
#include "AssignInput.h"
#include "ServoRemap.h"
#include "AnalogSwitch.h"
#include "ChannelRange.h"
#include "ChannelReverse.h"
#include "Mixer.h"
#include "Statistics.h"
#include "ChannelDelay.h"
#include "LogicSwitch.h"
#include "ModeAssign.h"

#if defined( ARDUINO )

#include "avr/sleep.h"
#include "avr/wdt.h"

#include "InputImpl.h"
#include "OutputImpl.h"
#include "PortsImpl.h"
#include "BuzzerImpl.h"
#include "DisplayImpl.h"

#endif

const char* InputChannelNames[ANALOG_CHANNELS] = {
    TEXT_INPUT_CH_1,
    TEXT_INPUT_CH_2,
    TEXT_INPUT_CH_3,
    TEXT_INPUT_CH_4,
    TEXT_INPUT_CH_5,
    TEXT_INPUT_CH_6,
    TEXT_INPUT_CH_7,
    TEXT_INPUT_CH_8,
    TEXT_INPUT_CH_9
};

const char* LogicalChannelNames[LOGICAL_CHANNELS] = {
    TEXT_CONTROL_CH_1,
    TEXT_CONTROL_CH_2,
    TEXT_CONTROL_CH_3,
    TEXT_CONTROL_CH_4,
    TEXT_CONTROL_CH_5,
    TEXT_CONTROL_CH_6,
    TEXT_CONTROL_CH_7,
    TEXT_CONTROL_CH_8,
    TEXT_CONTROL_CH_9,
    TEXT_CONTROL_CH_10,
    TEXT_CONTROL_CH_11,
    TEXT_CONTROL_CH_12
};

const char* OutputChannelNames[PPM_CHANNELS] = {
    TEXT_OUT_CH_1,
    TEXT_OUT_CH_2,
    TEXT_OUT_CH_3,
    TEXT_OUT_CH_4,
    TEXT_OUT_CH_5,
    TEXT_OUT_CH_6,
    TEXT_OUT_CH_7,
    TEXT_OUT_CH_8,
    TEXT_OUT_CH_9
};

const char *PhaseNames[TEXT_PHASES_count] = {
    TEXT_PHASE_NORMAL,
    TEXT_PHASE_THERMAL,
    TEXT_PHASE_SPEED,
    TEXT_PHASE_TAKEOFF,
    TEXT_PHASE_LAND,
    TEXT_PHASE_ACRO,
    TEXT_PHASE_ACRO2,
    TEXT_PHASE_DIST,
    TEXT_PHASE_TOW
};

const char *LogicTypes[TEXT_LOGIC_SW_TYPE_count] = {
    TEXT_LOGIC_SW_TYPE1,
    TEXT_LOGIC_SW_TYPE2,
    TEXT_LOGIC_SW_TYPE3,
    TEXT_LOGIC_SW_TYPE4,
    TEXT_LOGIC_SW_TYPE5,
    TEXT_LOGIC_SW_TYPE6
};

/* Make sure the entries are in the same order as the 
 * defines in Models.h
 */
const char *WingMixNames[TEXT_WINGMIX_count] = {
    TEXT_WINGMIX_NORMAL,
    TEXT_WINGMIX_DELTA,
    TEXT_WINGMIX_VTAIL
};

/* Make sure the entries are in the same order as the 
 * defines in Models.h
 */
const char *MixNames[TEXT_MIX_count] = {
    TEXT_MIX_AIL_RUD,
    TEXT_MIX_AIL_FLP,
    TEXT_MIX_SPL_AIL,
    TEXT_MIX_SPL_FLP,
    TEXT_MIX_SPL_ELV,
    TEXT_MIX_FLP_AIL,
    TEXT_MIX_FLP_ELV,
    TEXT_MIX_ELV_AIL,
    TEXT_MIX_ELV_FLP,
    TEXT_MIX_RUD_ELV
};

const buzzerCmd_t SoundWelcome[] = {
  BUZZER_PLAY( 1),
  BUZZER_PAUSE( 1),
  BUZZER_REPEAT( 0, 3),
  BUZZER_STOP()
};

#if defined( ARDUINO )

/* All analog pins */
#define ANALOG_PIN_COUNT (PORT_ANALOG_INPUT_COUNT + PORT_TRIM_INPUT_COUNT + PORT_AUX_INPUT_COUNT)
const uint8_t AnalogPins[ANALOG_PIN_COUNT] = {
  PORT_ANALOG_INPUT,
  PORT_TRIM_INPUT,
  PORT_AUX_INPUT
};

/* All switch pins */
const uint8_t SwitchPins[PORT_SWITCH_INPUT_COUNT] = {
  PORT_SWITCH_INPUT
};

SWITCH_CONFIGURATION

InputImpl *inputImpl;
OutputImpl *outputImpl;
PortsImpl *portsImpl;
BuzzerImpl *buzzerImpl;
DisplayImpl *displayImpl;

#undef ENABLE_MEMDEBUG
#undef ENABLE_BDEBUG
#define ENABLE_STATISTICS_MODULE
#define ENABLE_SERIAL


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

#else

#define ENABLE_STATISTICS_MODULE

#endif

Controls controls;
Output output;
Ports ports;
Buzzer buzzer( ports);
UserInterface userInterface;
ConfigBlock configBlock;
SystemConfig systemConfig;
ModelSelect modelSelect;
ModuleManager moduleManager( configBlock);

#ifdef ENABLE_STATISTICS_MODULE
Statistics statistics;
uint16_t lastOverrun = UINT16_MAX;
uint16_t wdLastReset;
#endif

#ifdef ENABLE_BDEBUG
uint8_t bdebugi = 0;
char bdebug[ BDEBUG_LEN ];
#endif


void setup( void) {

#ifdef ENABLE_BDEBUG
  BDEBUG_CLEAR();
#endif

#if defined( ARDUINO )

    inputImpl = new InputImpl( PORT_ANALOG_INPUT_COUNT, PORT_TRIM_INPUT_COUNT, PORT_AUX_INPUT_COUNT,
                               AnalogPins,
                               PORT_SWITCH_INPUT_COUNT, switchConfiguration,
                               SwitchPins);

    outputImpl = new OutputImpl();
    portsImpl = new PortsImpl();
    buzzerImpl = new BuzzerImpl();
    displayImpl = new DisplayImpl();
    
#if defined( ENABLE_MEMDEBUG ) || defined( ENABLE_BDEBUG) || defined( ENABLE_SERIAL)
    Serial.begin(19200);
#endif
   
#endif

    ports.init();
    buzzer.init();

    /* The order of modules is important.
     * It defines the order with the menu.
     */
    
    /* System menu */
    
    moduleManager.addToSystemMenu( &modelSelect);
    ServoMonitor *servoMonitor = new ServoMonitor( controls);
    moduleManager.addToSystemMenu( servoMonitor);
    SwitchMonitor *switchMonitor = new SwitchMonitor( controls);
    moduleManager.addToSystemMenu( switchMonitor);
    Bind *bind = new Bind();
    moduleManager.addToSystemMenu( bind);
    RangeTest *rangeTest = new RangeTest();
    moduleManager.addToSystemMenu( rangeTest);
    ModeAssign *modeAssign = new ModeAssign();
    moduleManager.addToSystemMenu( modeAssign);
    CalibrateSticks *calibrateSticks = new CalibrateSticks();
    moduleManager.addToSystemMenu( calibrateSticks);
    CalibrateTrim *calibrateTrim = new CalibrateTrim();
    moduleManager.addToSystemMenu( calibrateTrim);
    VccMonitor *vccMonitor = new VccMonitor();
    moduleManager.addToSystemMenu( vccMonitor);
#ifdef ENABLE_STATISTICS_MODULE
    moduleManager.addToSystemMenu( &statistics);
#endif

    /* Model menu */

    moduleManager.addToModelMenu( new SystemSetup());
    Model *model = new Model();
    moduleManager.addToModelMenu( model);
    ChannelRange *channelRange = new ChannelRange();
    moduleManager.addToModelMenu( channelRange);
    ChannelReverse *channelReverse = new ChannelReverse();
    moduleManager.addToModelMenu( channelReverse);
    AnalogSwitch *analogSwitch = new AnalogSwitch();
    moduleManager.addToModelMenu( analogSwitch);
    SwitchedChannels *switchedChannels = new SwitchedChannels();
    moduleManager.addToModelMenu( switchedChannels);
    AssignInput *assignInput = new AssignInput();
    moduleManager.addToModelMenu( assignInput);
    ChannelDelay *channelDelay = new ChannelDelay();
    moduleManager.addToModelMenu( channelDelay);
    Phases *phases = new Phases();
    moduleManager.addToModelMenu( phases);
    PhasesTrim *phasesTrim = new PhasesTrim();
    moduleManager.addToModelMenu( phasesTrim);
    LogicSwitch *logicSwitch = new LogicSwitch();
    moduleManager.addToModelMenu( logicSwitch);
    DualExpo *dualExpo = new DualExpo();
    moduleManager.addToModelMenu( dualExpo);
    Mixer *mixer = new Mixer();
    moduleManager.addToModelMenu( mixer);
    EngineCut *engineCut = new EngineCut();
    moduleManager.addToModelMenu( engineCut);
    Timer *timer = new Timer();
    moduleManager.addToModelMenu( timer);

    ServoRemap *servoRemap = new ServoRemap();
    moduleManager.addToModelMenu( servoRemap);
    ServoReverse *servoReverse = new ServoReverse();
    moduleManager.addToModelMenu( servoReverse);
    ServoSubtrim *servoSubtrim = new ServoSubtrim();
    moduleManager.addToModelMenu( servoSubtrim);
    ServoLimit *servoLimit = new ServoLimit();
    moduleManager.addToModelMenu( servoLimit);

    /* The order of modules is important.
     * It defines the order of execution in RunModules().
     */
    moduleManager.addToRunList( calibrateSticks);
    moduleManager.addToRunList( calibrateTrim);
    moduleManager.addToRunList( switchedChannels);
    moduleManager.addToRunList( analogSwitch);
    moduleManager.addToRunList( channelRange);
    moduleManager.addToRunList( channelReverse);

    /* The following moduels act on logical channels */
    moduleManager.addToRunList( assignInput);
    moduleManager.addToRunList( channelDelay);
    moduleManager.addToRunList( phases);
    moduleManager.addToRunList( logicSwitch);
    moduleManager.addToRunList( dualExpo);
    moduleManager.addToRunList( model);
    moduleManager.addToRunList( mixer);
    moduleManager.addToRunList( phasesTrim);
    moduleManager.addToRunList( engineCut);

    /* The following modules act on output (servo) channels */
    moduleManager.addToRunList( servoRemap);
    moduleManager.addToRunList( servoReverse);
    moduleManager.addToRunList( servoSubtrim);
    moduleManager.addToRunList( servoLimit);

    /* The follow moduels do not impact channel values */
    moduleManager.addToRunList( servoMonitor);
    moduleManager.addToRunList( switchMonitor);
    moduleManager.addToRunList( timer);
    moduleManager.addToRunList( vccMonitor);

    userInterface.init();

    systemConfig.load();

    moduleManager.loadModel( modelSelect.getModelID());

    controls.init();

#if defined( ARDUINO )
#ifdef ENABLE_MEMDEBUG
    MEMDEBUG_INIT();
#endif

    wdt_enable( WDTO_1S );
    wdLastReset = millis();
#endif

    buzzer.play( SoundWelcome);

#ifdef ENABLE_BDEBUG
    Serial.print("D:");
    Serial.println(bdebug);
#endif
}

void loop( void) {

#ifdef ENABLE_STATISTICS_MODULE
    unsigned long now;
    uint16_t overrun;
#endif

#if defined( ARDUINO )
    set_sleep_mode( SLEEP_MODE_IDLE);
    cli();
    sleep_enable();
    sei();
    sleep_cpu();
    sleep_disable();

    wdt_reset();
#ifdef ENABLE_STATISTICS_MODULE
    now = millis();
    statistics.updateWdTimeout( now - wdLastReset);
    wdLastReset = now;
#endif
#endif

    if( output.acceptChannels() ) {

#ifdef ENABLE_STATISTICS_MODULE
        now = millis();
#endif

        controls.GetControlValues();
        moduleManager.runModules( controls);
        output.setChannels( controls);

#ifdef ENABLE_STATISTICS_MODULE
        statistics.updateModulesTime( (uint16_t)(millis() - now));
#endif

#ifdef ENABLE_BDEBUG
    Serial.print("d:");
    Serial.println(bdebug);
    BDEBUG_CLEAR();
#endif
        
#if defined( ARDUINO )
#ifdef ENABLE_MEMDEBUG
        MEMDEBUG_CHECK();
        Serial.println(gapFree);
#endif
#endif     
    }

#ifdef ENABLE_STATISTICS_MODULE
    now = millis();
#endif

    userInterface.handle();

#ifdef ENABLE_STATISTICS_MODULE
    now = millis() - now;
    statistics.updateUITime( (uint16_t)now);
    overrun = output.getOverrunCounter();
    statistics.updatePPMOverrun( overrun);
    statistics.updateFrameTime( output.getMaxFrameTime());
    
    if( overrun != lastOverrun && statistics.debugOverrun()) {
        lastOverrun = overrun;
        userInterface.printDebug( overrun);
    } else if( statistics.debugTiming()) {
        userInterface.printDebug( (uint16_t)now);
    }
    
#endif
}
