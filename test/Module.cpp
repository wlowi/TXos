
#include "Module.h"

Module::Module( moduleType_t type, const char *name) {

    moduleType = type;
    moduleName = name;
    next = NULL;
}

moduleType_t Module::getConfigType() {

    return moduleType;
}

const char *Module::getName() {

    return moduleName;
}
