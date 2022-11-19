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
    The base class for all modules.
 */

#ifndef _Module_h_
#define _Module_h_

#include "TXos.h"
#include "TableEditable.h"
#include "Controls.h"

typedef uint8_t moduleType_t;

typedef uint8_t moduleSize_t;

#define MODULE_INVALID_TYPE             ((moduleType_t)0)

/* List all available modules here */

#define MODULE_MODEL_SELECT_TYPE        ((moduleType_t)1)
#define MODULE_MODEL_TYPE               ((moduleType_t)2)
#define MODULE_SERVO_MONITOR_TYPE       ((moduleType_t)3)
#define MODULE_SWITCH_MONITOR_TYPE      ((moduleType_t)4)
#define MODULE_ENGINE_CUT_TYPE          ((moduleType_t)10)
#define MODULE_SERVO_REVERSE_TYPE       ((moduleType_t)20)
#define MODULE_SERVO_SUBTRIM_TYPE       ((moduleType_t)21)
#define MODULE_SERVO_LIMIT_TYPE         ((moduleType_t)22)

class Module : public TableEditable {

    private:
        const char *moduleName;
        moduleType_t moduleType;

        Module *menuNext = nullptr;
        Module *runlistNext = nullptr;

    public:
        Module( moduleType_t type, const char *name);
        friend class ModuleManager;
        friend class Menu;
        
        /* */
        virtual void run( Controls &controls) = 0;

        /* Set default values for all configuration data of a module. */
        virtual void setDefaults() = 0;

        /* Get a pointer to the modules configuration data. */
        virtual uint8_t *getConfig() = 0;

        /* Returns the size of the modules configuration data. */
        virtual moduleSize_t getConfigSize() = 0;

        /* Get the modules configuration type identifier.  */
        moduleType_t getConfigType() const;

        /* From Interface TableEditable */
        const char *getName() final;
};

#endif
