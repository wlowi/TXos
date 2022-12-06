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

#include "BuzzerImpl.h"

BuzzerImpl::BuzzerImpl() = default;

void BuzzerImpl::init( Ports &p) {

}

void BuzzerImpl::off() {

    LOG("BuzzerImpl::off(): called\n");
}

void BuzzerImpl::play( const buzzerCmd_t sound[]) {

    LOG("BuzzerImpl::play(): called\n");
}

void BuzzerImpl::playPermanent( const buzzerCmd_t sound[]) {

    LOG("BuzzerImpl::playPermanent(): called\n");
}