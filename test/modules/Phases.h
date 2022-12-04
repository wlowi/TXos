/*
    TXos. A remote control transmitter OS.

    Copyright (C) 2022 Wolfgang Lohwasser

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

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

        /* From TableEditable */
        uint8_t getRowCount() final;
        const char *getRowName( uint8_t row) final;
        uint8_t getColCount( uint8_t row) final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;
};

#endif