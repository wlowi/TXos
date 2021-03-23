/*
 * ModelSelector.h
 */
 
#ifndef _ModelSelector_h
#define _ModelSelector_h

#include "Arduino.h"
#include "ProcessChain.h"

class ModelSelector : public ProcessModule {
private:
    uint8_t model;

public:
    ModelSelector();
    void init();

    void setConfiguration( uint8_t type, uint8_t length, uint8_t *mem);
    uint8_t getConfiguration( uint8_t *mem, uint8_t ptr);
    
    void process( channelSet_t *channels);
    
    uint8_t getModel();
    void setModel( uint8_t m);
};

#endif
