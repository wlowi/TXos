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

/*
    Maintain global system configuration.

    Configuration data is stored in configuration blocks in EEPROM.
    The very first block is reserved for the system configuration.
    Further blocks contain model specific configuration data.

    The ConfigBlock class performs the actual reading, writing and
    formatting of blocks.

 */

#ifndef _SystemConfig_h_
#define _SystemConfig_h_

#include "TXos.h"
#include "ConfigBlock.h"
#include "ModuleManager.h"

#define SYSTEMCONFIG_BLOCKID    ((configBlockID_t)0)

class SystemConfig {

    public:
        explicit SystemConfig();

        void load();
        void save();
};

#endif
