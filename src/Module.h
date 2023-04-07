/*
  TXos. A remote control transmitter OS.

  MIT License

  Copyright (c) 2023 wlowi

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

/*
    The base class for all modules.
 */

#ifndef _Module_h_
#define _Module_h_

#include "TXos.h"
#include "TextUI.h"
#include "Controls.h"

typedef uint8_t moduleType_t;

typedef uint8_t moduleSize_t;

#define MODULE_INVALID_TYPE             ((moduleType_t)0)

/* List all available modules here */

/* System modules */
// #define MODULE_SYSTEM_SETUP_TYPE        ((moduleType_t)1)   obsolete
#define MODULE_MODEL_SELECT_TYPE        ((moduleType_t)2)
#define MODULE_SERVO_MONITOR_TYPE       ((moduleType_t)3)
#define MODULE_SWITCH_MONITOR_TYPE      ((moduleType_t)4)
#define MODULE_CAL_STICKS_TYPE          ((moduleType_t)5)
#define MODULE_CAL_TRIM_TYPE            ((moduleType_t)6)
#define MODULE_BIND_TYPE                ((moduleType_t)7)
#define MODULE_RANGE_TEST_TYPE          ((moduleType_t)8)
#define MODULE_VCC_MONITOR_TYPE         ((moduleType_t)9)
#define MODULE_STATISTICS_TYPE          ((moduleType_t)10)
#define MODULE_MODE_ASSIGN_TYPE         ((moduleType_t)11)

/* Model specific modules */
#define MODULE_MODEL_TYPE               ((moduleType_t)50)
#define MODULE_ENGINE_CUT_TYPE          ((moduleType_t)51)
#define MODULE_DUAL_EXPO_TYPE           ((moduleType_t)52)
#define MODULE_TIMER_TYPE               ((moduleType_t)53)
#define MODULE_PHASES_TYPE              ((moduleType_t)54)
#define MODULE_SERVO_REVERSE_TYPE       ((moduleType_t)55)
#define MODULE_SERVO_SUBTRIM_TYPE       ((moduleType_t)56)
#define MODULE_SERVO_LIMIT_TYPE         ((moduleType_t)57)
#define MODULE_PHASES_TRIM_TYPE         ((moduleType_t)58)
#define MODULE_SWITCHED_CHANNELS_TYPE   ((moduleType_t)59)
#define MODULE_ASSIGN_INPUT_TYPE        ((moduleType_t)60)
#define MODULE_SERVO_REMAP_TYPE         ((moduleType_t)61)
#define MODULE_ANALOG_SWITCH_TYPE       ((moduleType_t)62)
#define MODULE_CHANNEL_RANGE_TYPE       ((moduleType_t)63)
#define MODULE_CHANNEL_REVERSE_TYPE     ((moduleType_t)64)
#define MODULE_MIXER_TYPE               ((moduleType_t)65)
#define MODULE_CHANNEL_DELAY_TYPE       ((moduleType_t)66)
#define MODULE_LOGIC_SWITCH_TYPE        ((moduleType_t)67)
#define MODULE_ANALOG_TRIM_TYPE         ((moduleType_t)68)

class Module : public TextUIScreen {

    private:
        const char *moduleName;
        moduleType_t moduleType;

        Module *runlistNext = nullptr;
        Module *setNext = nullptr;

    public:
        Module( moduleType_t type, const char *name);
        friend class ModuleManager;
        
        /* */
        virtual void run( Controls &controls) = 0;

        /* Called after model load */
        virtual void init() { /* noop */ }

        /* Called when module is entered from menu */
        virtual void moduleEnter() { /* noop */ }
        
        /* Called before leaving module */
        virtual void moduleExit() { /* noop */ }

        /* Called whenever a flight phase changes. 
         * The overrides are automatically defined in
         * Phases.h NO_CONFIG, PHASED_CONFIG an NON_PHASED_CONFIG macros.
         */
        virtual void switchPhase(phase_t ph) = 0;

        /* Set default values for all configuration data of a module. */
        virtual void setDefaults() = 0;

        /* Get a pointer to the modules configuration data. */
        virtual uint8_t *getConfig() = 0;

        /* Returns the size of the modules configuration data. */
        virtual moduleSize_t getConfigSize() = 0;

        /* Get the modules configuration type identifier.  */
        moduleType_t getConfigType() const;

        /* From Interface TextUIScreen */
        bool goBackItem() { return true; }
        void activate(TextUI *ui);
        void deactivate(TextUI *ui);
        
        bool isRowEditable(uint8_t row) { return true; }
        bool isColEditable(uint8_t row, uint8_t col) { return true; }
        const char *getMenuName() final;
	    const char *getHeader() { return getMenuName(); }
};

#include "Phases.h"

#endif
