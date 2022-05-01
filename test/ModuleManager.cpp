
#include "ModuleManager.h"

ModuleManager::ModuleManager() {

    first = last = NULL;
}

void ModuleManager::Add( Module *module) {

    module->next = NULL;

    if( first == NULL) {
        first = last = module;
    } else {
        last->next = module;
        last = module;
    }
}

void ModuleManager::RunModules( channelSet_t &channels) {

    Module *current = first;

    while( current != NULL) {
        current->run( channels);
        current = current->next;
    }
}
