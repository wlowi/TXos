/*
 * Dispatcher.h
 */
 
#ifndef _Dispatcher_h
#define _Dispatcher_h

#include "Arduino.h"

void delay( uint16_t msec);
void delayMicroseconds( uint16_t us);

#define PERIODIC_DUE     1
#define ADC_COMPLETE_DUE 2

/* Callback prototypes */
typedef void (DispatcherTimerCb)( uint16_t millis);
typedef void (DispatcherCallback)();

class Dispatcher
{
public:
    Dispatcher();

    static void init();
    static void sleep();
    
    static void adcComplete();

    static void addAdcCompleteCb( DispatcherCallback *cb);
    static void addPeriodicCb( DispatcherTimerCb *cb, unsigned int msec);

    static uint16_t getMillis();
    
    DispatcherTimerCb *periodicCb;
    DispatcherCallback *adcCompleteCb;

    volatile uint8_t dueFlags;
    volatile uint16_t periodicMSec;
};
 
 #endif
 
