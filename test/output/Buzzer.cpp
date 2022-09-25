
#include "Buzzer.h"
#include "BuzzerImpl.h"

extern BuzzerImpl *buzzerImpl;

Buzzer::Buzzer( Ports &p) : ports( p) {

} 

void Buzzer::init() {

  buzzerImpl->init( ports);
}

void Buzzer::off() {

  buzzerImpl->off();
}

void Buzzer::play( const buzzerCmd_t sound[]) {

  buzzerImpl->play( sound);
}
