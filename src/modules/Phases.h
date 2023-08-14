/*
  TXos. A remote control transmitter OS.

  MIT License

  Copyright (c) 2023 wlowi

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#ifndef _Phases_h_
#define _Phases_h_

#include "Module.h"

#define NO_CONFIG()                                 \
public:                                             \
    void switchPhase(phase_t ph) { /* no op */ }    \
    moduleSize_t getConfigSize() { return 0; }      \
    uint8_t *getConfig() { return nullptr; }


#define PHASED_CONFIG( c_t )                        \
private:                                            \
    c_t configuration[ PHASES ];                    \
    c_t *cfgPtr;                                    \
    phase_t phase;                                  \
public:                                             \
    void switchPhase(phase_t ph);                   \
    moduleSize_t getConfigSize() {                  \
        return (moduleSize_t)sizeof( configuration);\
    }                                               \
    uint8_t *getConfig() {                          \
        return (uint8_t*)&configuration;            \
    }

#define INIT_PHASED_CONFIGURATION( block )          \
    for( phase_t phase=0; phase<PHASES; phase++) {  \
        cfgPtr = &configuration[phase];             \
        block                                       \
    }                                               \
    cfgPtr = &configuration[0];


#define NON_PHASED_CONFIG( c_t )                    \
private:                                            \
    c_t configuration;                              \
    c_t *cfgPtr;                                    \
public:                                             \
    void switchPhase(phase_t ph) { /* no op */ }    \
    moduleSize_t getConfigSize() {                  \
        return (moduleSize_t)sizeof( configuration);\
    }                                               \
    uint8_t *getConfig() {                          \
        return (uint8_t*)&configuration;            \
    }

#define INIT_NON_PHASED_CONFIGURATION( block )      \
    cfgPtr = &configuration;                        \
    block


#define CFG cfgPtr


#define SWITCH_PHASE( p)                \
do {                                    \
    if( p < PHASES) {                   \
        phase = p;                      \
        cfgPtr = &configuration[p];     \
    }                                   \
} while( false)


typedef struct phases_t {

    switch_t sw;
    phase_t phaseName[PHASES]; 

} phases_t;

class Phases : public Module {

    NON_PHASED_CONFIG( phases_t )

    private:
        phase_t phase;

        char phaseText[5];

    public:
        Phases();

        phase_t getPhase();
        const char *getPhaseName();

        /* From Module */
        void run( Controls &controls) final;
        void setDefaults() final;
        void exportConfig( Exporter *exporter, uint8_t *config, moduleSize_t configSz) const;

        /* From TableEditable */
        uint8_t getRowCount() final;
        const char *getRowName( uint8_t row) final;
        uint8_t getColCount( uint8_t row) final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;
};

#endif