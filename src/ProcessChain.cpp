/*
 * ProcessChain.cpp
 */

#include "ProcessChain.h"

ProcessModule::ProcessModule( uint8_t type) {
    
    moduleType = type;
}

uint8_t ProcessModule::getModuleType() {
    
    return moduleType;
}

ProcessModule *ProcessModule::nextModule() {
    
    return next;
}

ProcessChain::ProcessChain() {

    chain = NULL;
}

void ProcessChain::add( ProcessModule *module) {
    
    ProcessModule *tmp;

    module->next = NULL;

    if( chain == NULL) {
        chain = module;
    } else {
        tmp = chain;
        while( tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = module;
    }
}

ProcessModule *ProcessChain::findModule( uint8_t type) {
   
    ProcessModule *module;
    module = chain;
    
    while( module) {
        if( module->getModuleType() == type) {
            break;
        }
        module = module->next;
    } 
    
    return module;
}

ProcessModule *ProcessChain::firstModule() {

    return chain;
}

void ProcessChain::process( channelSet_t *channels) {
    
    ProcessModule *module;
    module = chain;
    
    while( module) {
        module->process( channels);
        module = module->next;
    }
}
