/*
 * Dispatcher.cpp
 */

#include <util/atomic.h>

#include "Dispatcher.h"

static volatile uint16_t millis;
static volatile uint16_t sleepMillis;
static volatile uint16_t next;

Dispatcher dispatcher;

/* Compare timer 0 interrupt.
 * Once per msec.
 */
ISR(TIMER0_COMPA_vect)
{    
    if( millis < 59999) {
        millis++;
    } else {
        millis = 0;
    }
    
    sleepMillis++;

    TCNT0 = 0;
    
    if( dispatcher.periodicCb != NULL
        && dispatcher.periodicMSec > 0
        && millis == next) {

        next += dispatcher.periodicMSec;
        next %= 60000;

        dispatcher.dueFlags |= PERIODIC_DUE;
    }
}

// int Dispatchable::getValue() = 0;

Dispatcher::Dispatcher() {
        
    periodicCb = NULL;
    adcCompleteCb = NULL;

    periodicMSec = 0;
    dueFlags = 0;

    init();
}

void Dispatcher::addPeriodicCb( DispatcherTimerCb *cb, unsigned int msec) {
    
    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {
    
        dispatcher.periodicCb = cb;
        dispatcher.periodicMSec = msec;
        next = millis + msec;
        next %= 60000;
    }
}

void Dispatcher::addAdcCompleteCb( DispatcherCallback *cb) {
    
    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {
    
        dispatcher.adcCompleteCb = cb;
    }
}

void Dispatcher::adcComplete() {
    
    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {

        dispatcher.dueFlags |= ADC_COMPLETE_DUE;
    }
}

void Dispatcher::init() {

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE) {
    
        millis = 0;
        sleepMillis = 0;

        /* 8bit Timer 0 */
        TCCR0B = _BV(CS01) | _BV(CS00); // Prescale /64 = 250000Hz
        OCR0A = 250; // Interrupt at 250 = 1msec.
        TIMSK0 = _BV(OCIE0A);
    }
}

uint16_t Dispatcher::getMillis() {
    
    return millis;
}

/* Set CPU to sleep and wait for an interrupt to happen */
void Dispatcher::sleep() {
    
    set_sleep_mode( SLEEP_MODE_IDLE);
    cli();
    sleep_enable();
    sei();
    sleep_cpu();
    sleep_disable();

    while( dispatcher.dueFlags != 0) {

        /* When we come to here an interrupt has happened */
        if( dispatcher.dueFlags & PERIODIC_DUE) {
            
            cli();
            dispatcher.dueFlags &= ~PERIODIC_DUE;
            sei();
            
            if( dispatcher.periodicCb) {
                dispatcher.periodicCb( millis);
            }
        }
    
        if( dispatcher.dueFlags & ADC_COMPLETE_DUE) {
            
            cli();
            dispatcher.dueFlags &= ~ADC_COMPLETE_DUE;
            sei();
            
            if( dispatcher.adcCompleteCb) {
                dispatcher.adcCompleteCb();
            }
        }
    }
}

void delay( uint16_t msec)
{
    cli();
    sleepMillis = 0;
    sei();

    msec++;
    
    while( sleepMillis < msec) {
        Dispatcher::sleep();
    }
}

/* Sleep for some microseconds.
 * This is the original Arduino function.
 */ 
void delayMicroseconds( uint16_t us)
{
   	// call = 4 cycles + 2 to 4 cycles to init us(2 for constant delay, 4 for variable)

	// calling avrlib's delay_us() function with low values (e.g. 1 or
	// 2 microseconds) gives delays longer than desired.
	//delay_us(us);
#if F_CPU >= 24000000L
	// for the 24 MHz clock for the aventurous ones, trying to overclock

	// zero delay fix
	if (!us) return; //  = 3 cycles, (4 when true)

	// the following loop takes a 1/6 of a microsecond (4 cycles)
	// per iteration, so execute it six times for each microsecond of
	// delay requested.
	us *= 6; // x6 us, = 7 cycles

	// account for the time taken in the preceeding commands.
	// we just burned 22 (24) cycles above, remove 5, (5*4=20)
	// us is at least 6 so we can substract 5
	us -= 5; //=2 cycles

#elif F_CPU >= 20000000L
	// for the 20 MHz clock on rare Arduino boards

	// for a one-microsecond delay, simply return.  the overhead
	// of the function call takes 18 (20) cycles, which is 1us
	__asm__ __volatile__ (
		"nop" "\n\t"
		"nop" "\n\t"
		"nop" "\n\t"
		"nop"); //just waiting 4 cycles
	if (us <= 1) return; //  = 3 cycles, (4 when true)

	// the following loop takes a 1/5 of a microsecond (4 cycles)
	// per iteration, so execute it five times for each microsecond of
	// delay requested.
	us = (us << 2) + us; // x5 us, = 7 cycles

	// account for the time taken in the preceeding commands.
	// we just burned 26 (28) cycles above, remove 7, (7*4=28)
	// us is at least 10 so we can substract 7
	us -= 7; // 2 cycles

#elif F_CPU >= 16000000L
	// for the 16 MHz clock on most Arduino boards

	// for a one-microsecond delay, simply return.  the overhead
	// of the function call takes 14 (16) cycles, which is 1us
	if (us <= 1) return; //  = 3 cycles, (4 when true)

	// the following loop takes 1/4 of a microsecond (4 cycles)
	// per iteration, so execute it four times for each microsecond of
	// delay requested.
	us <<= 2; // x4 us, = 4 cycles

	// account for the time taken in the preceeding commands.
	// we just burned 19 (21) cycles above, remove 5, (5*4=20)
	// us is at least 8 so we can substract 5
	us -= 5; // = 2 cycles,

#elif F_CPU >= 12000000L
	// for the 12 MHz clock if somebody is working with USB

	// for a 1 microsecond delay, simply return.  the overhead
	// of the function call takes 14 (16) cycles, which is 1.5us
	if (us <= 1) return; //  = 3 cycles, (4 when true)

	// the following loop takes 1/3 of a microsecond (4 cycles)
	// per iteration, so execute it three times for each microsecond of
	// delay requested.
	us = (us << 1) + us; // x3 us, = 5 cycles

	// account for the time taken in the preceeding commands.
	// we just burned 20 (22) cycles above, remove 5, (5*4=20)
	// us is at least 6 so we can substract 5
	us -= 5; //2 cycles

#elif F_CPU >= 8000000L
	// for the 8 MHz internal clock

	// for a 1 and 2 microsecond delay, simply return.  the overhead
	// of the function call takes 14 (16) cycles, which is 2us
	if (us <= 2) return; //  = 3 cycles, (4 when true)

	// the following loop takes 1/2 of a microsecond (4 cycles)
	// per iteration, so execute it twice for each microsecond of
	// delay requested.
	us <<= 1; //x2 us, = 2 cycles

	// account for the time taken in the preceeding commands.
	// we just burned 17 (19) cycles above, remove 4, (4*4=16)
	// us is at least 6 so we can substract 4
	us -= 4; // = 2 cycles

#else
	// for the 1 MHz internal clock (default settings for common Atmega microcontrollers)

	// the overhead of the function calls is 14 (16) cycles
	if (us <= 16) return; //= 3 cycles, (4 when true)
	if (us <= 25) return; //= 3 cycles, (4 when true), (must be at least 25 if we want to substract 22)

	// compensate for the time taken by the preceeding and next commands (about 22 cycles)
	us -= 22; // = 2 cycles
	// the following loop takes 4 microseconds (4 cycles)
	// per iteration, so execute it us/4 times
	// us is at least 4, divided by 4 gives us 1 (no zero delay bug)
	us >>= 2; // us div 4, = 4 cycles
	

#endif

	// busy wait
	__asm__ __volatile__ (
		"1: sbiw %0,1" "\n\t" // 2 cycles
		"brne 1b" : "=w" (us) : "0" (us) // 2 cycles
	);
	// return = 4 cycles
}

