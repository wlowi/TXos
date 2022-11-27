
#include "DisplayImpl.h"

static volatile byte oldVal;

static volatile unsigned int buttonDown_msec;
static volatile boolean buttonIsDown;
static volatile byte button;
static volatile int enc;

#define ROTARYENC_CLK    ((byte)0b00000001)
#define ROTARYENC_DIR    ((byte)0b00000010)
#define ROTARYENC_SWITCH ((byte)0b00000100)

#define ROTARYENC_ENC_MASK    (ROTARYENC_CLK | ROTARYENC_DIR)
#define ROTARYENC_SWITCH_MASK (ROTARYENC_SWITCH)

/* Button short and long pressed */
#define ROTARYENC_BUTTON_SHORT 1
#define ROTARYENC_BUTTON_LONG  2

#define ROTARYENC_BUTTON_SHORT_MSEC  10
#define ROTARYENC_BUTTON_LONG_MSEC  500

#define PIN_CLK     A12
#define PIN_DIR     A13
#define PIN_SWITCH  A14

/* PK0 A8  PCINT16
 * PK1 A9  PCINT17
 * PK2 A10 PCINT18
 * PK3 A11 PCINT19
 * PK4 A12 PCINT20
 * PK5 A13 PCINT21
 * PK6 A14 PCINT22
 * PK7 A15 PCINT23
 */
#define ROTARYENC_PCINT_MASK  (_BV(PCINT20) | _BV(PCINT21) | _BV(PCINT22))

ISR( PCINT2_vect)
{
    byte butVal;
    byte encVal;

    unsigned long now = millis();

    encVal =  digitalRead( PIN_CLK) ? ROTARYENC_CLK : 0;
    encVal |= digitalRead( PIN_DIR) ? ROTARYENC_DIR : 0;
    butVal = digitalRead( PIN_SWITCH) ? ROTARYENC_SWITCH : 0;

    if( (oldVal & ROTARYENC_SWITCH) && !butVal) { /* Button down */
        buttonDown_msec = now;
        buttonIsDown = true;
        
        oldVal &= ~ROTARYENC_SWITCH_MASK;
        oldVal |= butVal;
        
    } else if ( !(oldVal & ROTARYENC_SWITCH) && butVal) { /* Button up */
        unsigned int down_msec;
        buttonIsDown = false;

        down_msec = (now >= buttonDown_msec)
                  ? now - buttonDown_msec
                  : UINT16_MAX - buttonDown_msec + now;
           
        if( down_msec >= ROTARYENC_BUTTON_LONG_MSEC) {
            button = ROTARYENC_BUTTON_LONG;

        } else if( down_msec >= ROTARYENC_BUTTON_SHORT_MSEC) {
            button = ROTARYENC_BUTTON_SHORT;
        }

        oldVal &= ~ROTARYENC_SWITCH_MASK;
        oldVal |= butVal;
    }

    /* This avoids change of encoder value while button is pressed */
    if( !buttonIsDown) {

        switch( (oldVal & ROTARYENC_ENC_MASK) ^ encVal) {
        case ROTARYENC_CLK:
            if( encVal == 0 || encVal == 3) enc--; else enc++;
            break;
        case ROTARYENC_DIR:
            if( encVal == 0 || encVal == 3) enc++; else enc--;
            break;
        }

        oldVal &= ~ROTARYENC_ENC_MASK;
        oldVal |= encVal;
    }
}

DisplayImpl::DisplayImpl() {
    
    lcd = new LcdWidget();
    
    init();
}

void DisplayImpl::init()
{
    cli();

    pinMode( PIN_CLK, INPUT);
    pinMode( PIN_DIR, INPUT);
    pinMode( PIN_SWITCH, INPUT);

    /* Enable pull-up */
    digitalWrite( PIN_CLK, HIGH);
    digitalWrite( PIN_DIR, HIGH);
    digitalWrite( PIN_SWITCH, HIGH);
    
    PCMSK2 |= ROTARYENC_PCINT_MASK;
    PCICR |= _BV(PCIE2);
    
    enc = button = buttonDown_msec = 0;
    buttonIsDown = false;

    oldVal  = digitalRead( PIN_CLK) ? ROTARYENC_CLK    : 0;
    oldVal |= digitalRead( PIN_DIR) ? ROTARYENC_DIR    : 0;
    oldVal |= digitalRead( PIN_SWITCH) ? ROTARYENC_SWITCH : 0;

    sei();
}

LcdWidget *DisplayImpl::getLCD( void) {

    return lcd;
}

Event *DisplayImpl::getEvent() {

    cli();
    
    if( button == ROTARYENC_BUTTON_SHORT) {
      event.key = KEY_ENTER;
      event.count = 1;
      button = 0;
    } else if( button == ROTARYENC_BUTTON_LONG) {
      event.key = KEY_CLEAR;
      event.count = 1;
      button = 0;
    } else if( enc > 0) {
      event.key = KEY_UP;
      event.count = (enc >> 2);
      enc = 0;
    } else if( enc < 0) {
      event.key = KEY_DOWN;
      event.count = -(enc >> 2);
      enc = 0;
    } else {
      event.key = KEY_NONE;
      event.count = 0;
    }

    sei();
    
    return &event;
}
