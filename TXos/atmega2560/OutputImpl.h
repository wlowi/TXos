
#ifndef _OutputImpl_h_
#define _OutputImpl_h_

#include "TXos.h"
#include "Controls.h"

#define PPM_PORT            5

/*
 *      0  400     1000  1500  2000             22000
 *  ____     _______             _________ ... ____
 *      |   |       |     |     |
 *      |___|       |     |     |
 *
 *      |   |
 *    PPM_SPACE
 *
 *          |   PPM_MID   |
 *
 *
 *  100%  ==   1.1  -  1.9 mSec
 *  125%  ==   1.0  -  2.0 mSec
 *  150%  ==   0.9  -  2.1 mSec
 *
 *  9 ch * 2.1 mSec = 18.9 mSec    22.0 - 18.9 = 3.1 mSec
 *
 */


#define PPM_FRAME_usec      ((timingUsec_t)22000)
#define PPM_SPACE_usec      ((timingUsec_t)  400)
#define PPM_MID_usec        ((timingUsec_t) 1100)
#define PPM_RANGE100_usec   ((timingUsec_t)  400)
/* 150 % */
#define PPM_RANGEMAX_usec   ((timingUsec_t)  600)

#define PPM_MIN_usec        (PPM_MID_usec - PPM_RANGEMAX_usec)
#define PPM_MAX_usec        (PPM_MID_usec + PPM_RANGEMAX_usec)

/* Initial high for sync */
#define PPM_INIT_usec       ((timingUsec_t) 8000)

#define OTHER_PPMSET( s)    (((s) +1) % 2)

typedef struct ppmSet_t {
    
    timingUsec_t channel[ PPM_CHANNELS ];

} ppmSet_t;

class OutputImpl
{
    public:
        ppmSet_t ppmSet[2];
        uint8_t currentSet;

    private:
    	  /* Count how may times the user mode code was unable to compute 
     	   * and set all channels within one PPM frame.
     	   * This should always be 0.
     	   */
    	  uint16_t ppmOverrun;
    	  bool channelSetDone;
    	
    public:
        OutputImpl();

    	  void switchSet();

	      bool acceptChannels();
        void SetChannelValue( int channel, int value);
        
        timingUsec_t getInFrameTime();
        timingUsec_t getMaxFrameTime();
        uint16_t getOverrunCounter();
        
    private:
    	  void init();
};

#endif
