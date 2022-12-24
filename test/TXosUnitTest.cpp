
#include "InputImpl.h"
#include "DisplayImpl.h"
#include "OutputImpl.h"
#include "PortsImpl.h"
#include "BuzzerImpl.h"

#include "EEPROM.h"

#include "time.h"

#include "UtModules.h"

EEPROMClass EEPROM(4096);

InputImpl *inputImpl;
OutputImpl *outputImpl;
DisplayImpl *displayImpl;
PortsImpl *portsImpl;
BuzzerImpl *buzzerImpl;

extern Controls controls;

SWITCH_CONFIGURATION

long millis() {

    return (long)clock() * 1000 / CLOCKS_PER_SEC;
}

int main() {
    
    portsImpl = new PortsImpl();
    buzzerImpl = new BuzzerImpl();
    displayImpl = new DisplayImpl();
    outputImpl = new OutputImpl( 9);
    inputImpl =  new InputImpl( PORT_ANALOG_INPUT_COUNT, PORT_TRIM_INPUT_COUNT, PORT_AUX_INPUT_COUNT,
                                PORT_SWITCH_INPUT_COUNT, switchConfiguration);

    UnitTest *modules = new UtModules();

    modules->run();

    return 0;
}