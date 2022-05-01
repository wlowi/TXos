/*
 * PPM.h
 * 
 * Generate a PPM signal.
 * 
 * Configuration for PPM module.
 * 
 * History:
 * 
 *   wolfix   31.Dec.2019  Creation
 */
 
#ifndef _PPM_h
#define _PPM_h

#include "Arduino.h"
#include "TXos.h"


/* Numer of supported channels. */
#define PPM_CHANNELS      ((channel_t)9)

/* PPM channel timing in micro seconds.
 */
typedef uint16_t ppmTiming_t;

/* Timings for all channels */
typedef struct timingSet_t {
    
    ppmTiming_t channel[ PPM_CHANNELS ];

} timingSet_t;


/* PPM frame format
 * 
 *   |<------------- PPM_FRAME 22.5 milliseconds ------------>|
 *   |<-- one channel -->|
 * 
 *        ---------------     -----------   .  .  .   ---------
 *   |   |               |   |
 *    ---                 ---
 * 
 *   |<->|<-- PPM_MAX -->|
 * 
 *   Space
 *  0.3 msec
 *
 *  Timings are in micro seconds. 
 */
#define PPM_FRAME_usec      ((ppmTiming_t)22500)
#define PPM_SPACE_usec      ((ppmTiming_t)  300)
#define PPM_MID_usec        ((ppmTiming_t) 1200)
#define PPM_RANGE_usec      ((ppmTiming_t)  500)

#define PPM_MIN_usec        (PPM_MID_usec - PPM_RANGE_usec)
#define PPM_MAX_usec        (PPM_MID_usec + PPM_RANGE_usec)

/* Initial high for sync */
#define PPM_INIT_usec       ((ppmTiming_t) 8000)

#define PPM_PORT 5


class PPM
{
public:
    volatile timingSet_t *modifiableSet;
    volatile timingSet_t *activeSet;
    channel_t outputChannel;
    
    /* PPM generation state machine state.
     * Used in the interrupt routine.
     */
    uint8_t outputState;

    /* inFrameTime is used to compute the remaining time until frame end */
    ppmTiming_t inFrameTime_usec;

    /* Count how may times the user mode code was unable to compute 
     * and set all channels within one PPM frame.
     * This should be 0.
     */
    uint16_t ppmOverrun;

    /* Flag indicating that all channels have been set.
     */
    volatile boolean channelSetDone;
    
public:
    PPM();
    
    void init();

    static ppmTiming_t getInFrameTime();

    static void switchSet();
    static void setChannel( channel_t channel, value_t usec);
    static uint16_t getOverrunCounter();
};

#endif
