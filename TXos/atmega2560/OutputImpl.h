
#ifndef _OutputImpl_h_
#define _OutputImpl_h_

#include "TXos.h"

#define PPM_PORT            5

#define PPM_FRAME_usec      ((timingUsec_t)22500)
#define PPM_SPACE_usec      ((timingUsec_t)  300)
#define PPM_MID_usec        ((timingUsec_t) 1200)
#define PPM_RANGE_usec      ((timingUsec_t)  500)

#define PPM_MIN_usec        (PPM_MID_usec - PPM_RANGE_usec)
#define PPM_MAX_usec        (PPM_MID_usec + PPM_RANGE_usec)

/* Initial high for sync */
#define PPM_INIT_usec       ((timingUsec_t) 8000)

#define OTHER_PPMSET( s)    (((s) +1) % 2)

typedef struct ppmSet_t {
    
    timingUsec_t channel[ CHANNELS ];

} ppmSet_t;

class OutputImpl
{
    public:
        ppmSet_t ppmSet[2];
        uint8_t currentSet;

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
        uint16_t getOverrunCounter();
        
    private:
    	  void init();
};

#endif
