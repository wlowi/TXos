
#ifndef _Mixer_h_
#define _Mixer_h_

#include "Module.h"

typedef struct mixer_t {

    switch_t mixSw[MIXER];

    /* source and target are mix channel numbers 
     * To get the real channel number they need to be 
     * mapped via real_channel = mixChannelMap[ source ]
     */
    channel_t source[MIXER];
    channel_t target[MIXER];

    percent_t mixPct[MIXER];
    percent_t mixOffset[MIXER];

} mixer_t;

class Mixer : public Module {

    NON_PHASED_CONFIG( mixer_t)

    private:
        char mixerName[5];

        channelValue_t mixValue( channelValue_t v, uint8_t mix);
        void limitChannels( Controls &controls);

    public:
        Mixer();

        /* From Module */
        void run( Controls &controls) final;
        void setDefaults() final;

        /* From TableEditable */
        uint8_t getRowCount() final;
        const char *getRowName( uint8_t row) final;
        uint8_t getColCount( uint8_t row) final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;
};
#endif