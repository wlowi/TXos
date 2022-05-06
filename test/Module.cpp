
#include "Module.h"

Module::Module( moduleType_t type) {

    moduleType = type;
    next = NULL;
}

moduleType_t Module::getConfigType() {

    return moduleType;
}
