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

#include "ModuleManager.h"
#include "AssignInput.h"
#include "ChannelRange.h"

#include "Timer.h"
#include "Buzzer.h"

extern ModuleManager moduleManager;

extern Buzzer buzzer;

const buzzerCmd_t SoundMinute[] = {
  BUZZER_PLAY(2),
  BUZZER_STOP()
};

const buzzerCmd_t SoundSecond[] = {
  BUZZER_PLAY(1),
  BUZZER_STOP()
};

const buzzerCmd_t SoundZero[] = {
  BUZZER_PLAY(5),
  BUZZER_STOP()
};

/* The import/export dictionary.
 * See ImportExport.h
 */
DICTROW(r1, COMM_DATATYPE_UINT8, COMM_FIELD_SWITCH, flightTimer_t, swState)
DICTROW(r2, COMM_DATATYPE_UINT16, COMM_FIELD_TIME, flightTimer_t, time_sec)
DICTROW(r3, COMM_DATATYPE_BOOL, COMM_FIELD_THRDEP, flightTimer_t, throttleDep)
DICT(Timer, COMM_SUBPACKET_TIMER, &r1, &r2, &r3)

Timer::Timer() : Module(MODULE_TIMER_TYPE, TEXT_MODULE_TIMER, COMM_SUBPACKET_TIMER) {

    setDefaults();
}

void Timer::reset() {

#if defined(ARDUINO_ARCH_AVR)
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
#endif
        countdown_mSec = (uint32_t)CFG->time_sec * MSEC_PER_SEC;
        lastMillis = 0;
        running = false;
#if defined(ARDUINO_ARCH_AVR)
    }
#endif
    LOGV("Timer::reset: %d mSec\n", countdown_mSec);
}

uint16_t Timer::timeSec() {

    uint16_t t;

#if defined(ARDUINO_ARCH_AVR)
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
#endif
        t = (uint16_t)(countdown_mSec / MSEC_PER_SEC);
#if defined(ARDUINO_ARCH_AVR)
    }
#endif

    return t;
}

char* Timer::timeStr() {

    uint16_t t;
    uint8_t min;
    uint8_t sec;

#if defined(ARDUINO_ARCH_AVR)
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
#endif
        t = (uint16_t)(countdown_mSec / MSEC_PER_SEC);
#if defined(ARDUINO_ARCH_AVR)
    }
#endif

    min = t / 60;
    sec = t % 60;

    if (min < 10) {
        timeAsString[0] = ' ';
    }
    else {
        timeAsString[0] = '0' + min / 10;
    }

    timeAsString[1] = '0' + min % 10;
    timeAsString[2] = ':';
    timeAsString[3] = '0' + sec / 10;
    timeAsString[4] = '0' + sec % 10;
    timeAsString[5] = '\0';

    return timeAsString;
}

/* From Module */

COMM_RC_t Timer::exportConfig(ImportExport* exporter, uint8_t* config) const {

    return exporter->runExport(DICT_ptr(Timer), DICTROW_ptr(Timer), config, sizeof(flightTimer_t));
}

COMM_RC_t Timer::importConfig(ImportExport* importer, uint8_t* config) const {

    return importer->runImport(DICT_ptr(Timer), DICTROW_ptr(Timer), config, sizeof(flightTimer_t));
}

void Timer::run(Controls& controls) {

    unsigned long now;
    bool update = false;
    percent_t thrPct;

    // Get throttle value in percent. Range is usually -100% ... 100%
    percent_t thrVal = CHANNEL_TO_PCT(controls.logicalGet(CHANNEL_THROTTLE));

    // Determine min and max limit of throttle channel
    const AssignInput* assignInput = (AssignInput*)moduleManager.getModuleByType(MODULE_SET_MODEL, MODULE_ASSIGN_INPUT_TYPE);
    channel_t in = assignInput->getInputChannel(CHANNEL_THROTTLE);
    const ChannelRange* channelRange = (ChannelRange*)moduleManager.getModuleByType(MODULE_SET_MODEL, MODULE_CHANNEL_RANGE_TYPE);
    percent_t chMin = channelRange->getRangeMin(in); // default: -100
    percent_t chMax = channelRange->getRangeMax(in); // default: 100

    if (CFG->throttleDep) {
        // map throttle channel value to 0-100%
        if (thrVal < chMin) {
            thrVal = chMin;
        }
        if (thrVal > chMax) {
            thrVal = chMax;
        }
        thrPct = ((int16_t)thrVal - (int16_t)chMin) * (int16_t)100 / ((int16_t)chMax - (int16_t)chMin);
    }
    else {
        thrPct = 100; // Run at wall clock speed.
    }

    if (controls.evalSwitches(CFG->swState) && countdown_mSec > 0 && thrPct > 0) {
#if defined(ARDUINO_ARCH_AVR)
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
#endif
            now = millis();
            uint32_t diff = now - lastMillis;

            if (!running) {
                // First update after timer reset
                // or first update after engine was stopped.
                lastMillis = now;
                running = true;
            }
            else if (diff >= 100) {
                uint32_t dynDiff = diff * thrPct / 100;
                if (dynDiff >= countdown_mSec) {
                    countdown_mSec = 0;
                }
                else {
                    countdown_mSec -= dynDiff;
                }
                // Use this instead of just 'diff'. It reduces rounding error.
                lastMillis += dynDiff * 100 / thrPct;
                update = true;
            }
#if defined(ARDUINO_ARCH_AVR)
        }
#endif

        if (update) {

            uint16_t lastSec = countdown_sec;
            countdown_sec = countdown_mSec / MSEC_PER_SEC;

            if (countdown_mSec == 0) {
                LOG("SoundZero\n");
                buzzer.play(SoundZero);
            }
            else if (lastSec != countdown_sec) {
                if (countdown_sec < 10) {
                    LOG("SoundSecond\n");
                    buzzer.play(SoundSecond);
                }
                else if (((countdown_sec + 1) % 60) == 0) {
                    LOG("SoundMinute\n");
                    buzzer.play(SoundMinute);
                }
            }
        }
    }
    else {
        running = false;
    }
}

void Timer::init() {

    reset();
    LOGV("Timer::init: %d mSec\n", countdown_mSec);
}

void Timer::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        INIT_SWITCH(CFG->swState);
            CFG->time_sec = 0;
            CFG->throttleDep = false;
        )

        reset();
}

/* From TableEditable */

uint8_t Timer::getRowCount() {

    return 3;
}

const char* Timer::getRowName(uint8_t row) {

    if (row == 0) {
        return TEXT_SWITCH;
    } else if( row == 1) {
        return TEXT_TIME;
    } else {
        return TEXT_THRDEP;
    }
}

uint8_t Timer::getColCount(uint8_t row) {

    if (row == 0) {
        return 1;
    }
    else if( row == 1) {
        return 3;
    } else {
        return 1;
    }
}

void Timer::getValue(uint8_t row, uint8_t col, Cell* cell) {

    uint8_t min;
    uint8_t sec;

    if (row == 0) {
        cell->setSwitchState(7, CFG->swState);
    }
    else if (row == 1) {
        if (col == 0) {
            min = CFG->time_sec / 60;
            cell->setInt8(7, min, 2, 0, 59);
        }
        else if (col == 1) {
            cell->setLabel(9, ":", 1);
        }
        else if (col == 2) {
            sec = CFG->time_sec % 60;
            cell->setInt8(10, sec, 2, 0, 59);
        }
    }
    else if( row == 2) {
        cell->setBool(9, CFG->throttleDep);
    }
}

void Timer::setValue(uint8_t row, uint8_t col, Cell* cell) {

    uint8_t min;
    uint8_t sec;

    if (row == 0) {
        CFG->swState = cell->getSwitchState();
    }
    else if (row == 1) {

        min = CFG->time_sec / 60;
        sec = CFG->time_sec % 60;

        if (col == 0) {
            min = cell->getInt8();
        }
        else if (col == 2) {
            sec = cell->getInt8();
        }

        CFG->time_sec = (uint16_t)60 * min + sec;
    }
    else if( row == 2) {
        CFG->throttleDep = cell->getBool();
    }
}
