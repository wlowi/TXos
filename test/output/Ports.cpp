
#include "Ports.h"
#include "PortsImpl.h"

extern PortsImpl *portsImpl;

void Ports::init() const {

    portsImpl->portInit( PORT_BIND_RELAIS, OUTPUT);
    portsImpl->portInit( PORT_HF_RELAIS, OUTPUT);
    portsImpl->portInit( PORT_BUZZER, OUTPUT);

    buzzerOff();
    bindOff();
    hfOn();
}

void Ports::hfOn() const {

    /* HF is on when relais is off */
    portsImpl->portSet( PORT_HF_RELAIS, LOW);
}

void Ports::hfOff() const {

    portsImpl->portSet( PORT_HF_RELAIS, HIGH);
}

void Ports::bindOn() const {

    portsImpl->portSet( PORT_BIND_RELAIS, HIGH);
}

void Ports::bindOff() const {

    portsImpl->portSet( PORT_BIND_RELAIS, LOW);
}

void Ports::buzzerOn() const {

    portsImpl->portSet( PORT_BUZZER, HIGH);
}

void Ports::buzzerOff() const {

    portsImpl->portSet( PORT_BUZZER, LOW);
}
