/*
 * TXos - RC Transmitter OS
 *
 * D0   RX            - USB
 * D1   TX            - USB
 * D2   Clk           - Rotary Encoder
 * D3   Dir           - Rotary Encoder
 * D4   Switch        - Rotary Encoder
 * D5   PPM Out
 * D6   Digital 1
 * D7   Digital 2
 * D8   Digital 3
 * D9   Digital 4
 * D10  Digital 5
 * D11  MOSI          - ICSP
 * D12  MISO          - ICSP
 * D13  SCK           - ICSP
 * 
 * A0   Analog 1
 * A1   Analog 2
 * A2   Analog 3
 * A3   Analog 4
 * A4   SDA           - TWI (LCD)
 * A5   SCL           - TWI (LCD)
 * A6   Analog 5
 * A7   Vcc Monitor
 * 
 */

#include "Dispatcher.h"

#include "PPM.h"

#include "AnalogInput.h"
#include "DigitalInput.h"

#include "Serial.h"

#include "ConfigBlock.h"
#include "SystemConfig.h"
#include "ModelConfig.h"

#include "TXos.h"

/* Modules */
#include "ModelSelector.h"
#include "ModeSelector.h"
#include "ServoReverse.h"
#include "ServoSubTrim.h"
#include "ServoLimit.h"


/* Serial interface (USB) */
Serial serial( 19200);


ConfigBlock configBlockService;

SystemConfig systemConfig( &configBlockService);
ModelConfig modelConfig( &configBlockService);

ModelSelector modelSelector;


ppmTiming_t t1_max, t2_max;

/* ***** foreward definitions ***** */

static void setup();
static void loop();

static void setupPorts();
static void blink( uint8_t howOften, uint16_t howLongMSec);
static void sos( uint8_t err);

static void adcCompleteCb();

/* ***** ***** */

int main()
{
    setup();

    for( ;; ) {
        loop();
    }
}

static void setup()
{
    uint8_t rc;
    uint8_t model;

    cli();

    setupPorts();

    systemConfig.addModule( &modelSelector);
    systemConfig.addModule( new ModeSelector());

    modelConfig.addModule( new ServoReverse());
    modelConfig.addModule( new ServoSubTrim());
    modelConfig.addModule( new ServoLimit());

    rc = systemConfig.read( CONFIG_SYSTEM_BLOCK);
    if( rc != CONFIG_RC_OK) {
        sos( 1);
        systemConfig.write( CONFIG_SYSTEM_BLOCK);
    }

    model = modelSelector.getModel();

    rc = modelConfig.read( model);
    if( rc != CONFIG_RC_OK) {
        sos( 2);
        modelConfig.write( model);
    }
    
    Dispatcher::addAdcCompleteCb( adcCompleteCb);
    
    t1_max = t2_max = 0;

    sei();
    
    blink( 2, 100);
}

/* Error handler when an undefined pure virtual function gets called.
 */
extern "C" void __cxa_pure_virtual() {
    
    panic();
}

void panic() {
    
    while( 1) {
        blink( 1, 500);
    }
}

static void loop()
{
    Dispatcher::sleep();
}

/* ***** private static ***** */


static void setupPorts()
{
    /* Port 13 (LED) to output PB5 */
    DDRB |= _BV(DDB5);
    PORTB = 0;
}

/* Blink onboard LED on port 13.
 */
static void blink( uint8_t howOften, uint16_t howLongMSec)
{
    while( howOften-- > 0) {
        PORTB |= _BV(PB5);
        delay( howLongMSec);
        PORTB &= ~_BV(PB5);
        delay( howLongMSec);
    }
}

/* Flash on board LED SOS followed by an error code.
 */
static void sos( uint8_t err)
{
    blink( 3, 60);  // S ...
    blink( 3, 150); // O ---
    blink( 3, 60);  // S ...

    delay( 400);
    blink( err, 500);
}

/* This callback is called when all ADC conversions have been done.
 * It is a user function and runns in parallel with PPM generation.
 */
static void adcCompleteCb() {

    //ppmTiming_t t1, t2;
    channel_t ch;
    channelSet_t channels;

    /* Measure how long it takes to get here from start of frame
     * This is the ADC conversion time plus the time the dispatcher
     * needs to call us.
     */
    //t1 = PPM::getInFrameTime();

    for( ch = 0; ch < CHANNELS; ch++) {
        channels.analogChannel[ ch ] = AnalogInput::getChannelValue( ch);
    }
    
    channels.switches = DigitalInput::getSwitches();

    systemConfig.process( &channels);
    modelConfig.process( &channels);

    for( ch = 0; ch < CHANNELS; ch++) {
        PPM::setChannel( ch, channels.analogChannel[ ch ]);
    }
     
    /*
    serial.printu(PPM::getOverrunCounter());
    serial.printc('\n');
    */

    /* Measure how long it takes to get here from start of frame.
     */
    //t2 = PPM::getInFrameTime();

/*
    if( t1 > t1_max || t2 > t2_max) {
        t1_max = t1;
        t2_max = t2;
        serial.printu(t1);
        serial.printc(' ');
        serial.printu(t2);
        serial.printc('\n');
    }
*/
}
