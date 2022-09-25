
#include "PortsImpl.h"

PortsImpl::PortsImpl() = default;

void PortsImpl::portInit( uint8_t p, uint8_t t) const {

  pinMode( p, t);
}

void PortsImpl::portSet( uint8_t p, uint8_t s) const {

  digitalWrite( p, s);  
}

uint8_t PortsImpl::portGet( uint8_t p) const {

  return digitalRead( p);
}
