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
#include "EngineCut.h"
#include "MotorStart.h"

extern const char* const PhaseNames[TEXT_PHASES_count];

extern ModuleManager moduleManager;

/* The import/export dictionary.
 * See ImportExport.h
 */
DICTROW( r1, COMM_DATATYPE_UINT8, COMM_FIELD_PHASE, motorStart_t, phase)
DICTROW( r2, COMM_DATATYPE_UINT8, COMM_FIELD_SWITCH, motorStart_t, activateSw)
DICTROW( r3, COMM_DATATYPE_INT8, COMM_FIELD_PERCENT, motorStart_t, trigger_pct)
DICTROW( r4, COMM_DATATYPE_INT16, COMM_FIELD_TIME, motorStart_t, leadingTime)
DICTROW( r5, COMM_DATATYPE_INT16, COMM_FIELD_DELAY, motorStart_t, delayTime)
DICT( MotorStart, COMM_SUBPACKET_MOTOR_START, &r1, &r2, &r3, &r4, &r5)

MotorStart::MotorStart() : Module( MODULE_MOTOR_START_TYPE, TEXT_MODULE_MOTOR_START, COMM_SUBPACKET_MOTOR_START) {

    setDefaults();
}

bool MotorStart::isActive() {

    return !(state == STATE_INACTIVE || state == STATE_DEACTIVATING);
}

/* From Module */

COMM_RC_t MotorStart::exportConfig( ImportExport *exporter, uint8_t *config) const {

    return exporter->runExport( DICT_ptr(MotorStart), DICTROW_ptr(MotorStart), config, sizeof(motorStart_t));
}

COMM_RC_t MotorStart::importConfig( ImportExport *importer, uint8_t *config) const {

    return importer->runImport( DICT_ptr(MotorStart), DICTROW_ptr(MotorStart), config, sizeof(motorStart_t));
}

void MotorStart::run( Controls &controls) {

    #define SCALING_F (10)

    int16_t targetPosition10;  // scaled by 10 for increased precision
    int16_t stepPosition10;    // same here
    int16_t delay_msec;

    switchState_t sw;
    percent_t thrPct;
    EngineCut *engineCut = (EngineCut*)moduleManager.getModuleByType(MODULE_SET_MODEL, MODULE_ENGINE_CUT_TYPE);
    Phases *phases = (Phases*)moduleManager.getModuleByType(MODULE_SET_MODEL, MODULE_PHASES_TYPE);

    if( inModule) {
        // Do not accidentaly activate while configuring the module.
        state = STATE_INACTIVE;
        return;
    }

    switch( state) {
        case STATE_INACTIVE:
            // If engine cut is off
            //   and in correct phase
            //   and switch in correct state
            if( engineCut && engineCut->isSave()) {
                break;
            }
            if( phases && phases->getPhaseType() != CFG->phase) {
                break;
            }
            if( IS_SWITCH_USED(CFG->activateSw)) {
                sw = controls.switchGet(GET_SWITCH(CFG->activateSw));
                if( sw == GET_SWITCH_STATE(CFG->activateSw )) {
                    state = STATE_ACTIVATED;
                    LOG("MotorStart:run(): ACTIVATED\n");
                }
            }
            break;

        case STATE_ACTIVATED:
            // If engine cut tuned on again, cancel activation
            if( engineCut && engineCut->isSave()) {
                state = STATE_DEACTIVATING;
                LOG("MotorStart:run(): DEACTIVATING\n");
                break;
            }
            thrPct = CHANNEL_TO_PCT(controls.logicalGet(CHANNEL_THROTTLE));
            if( thrPct >= CFG->trigger_pct) {
                state = STATE_TRIGGERED;
                LOG("MotorStart:run(): TRIGGERED\n");
                endTime_msec = 0;
            }
            controls.logicalSet( CHANNEL_THROTTLE, PCT_TO_CHANNEL(engineCut->getEngineCutPct()));
            break;

        case STATE_TRIGGERED:
            // If engine cut tuned on again, cancel activation
            if( engineCut && engineCut->isSave()) {
                state = STATE_DEACTIVATING;
                LOG("MotorStart:run(): DEACTIVATING\n");
                break;
            }
            if( endTime_msec == 0) {
                // Set new leading end time
                endTime_msec = millis() + CFG->leadingTime*100;
            } else if( millis() > endTime_msec) {
                // leading time exceeded
                state = STATE_START;
                LOG("MotorStart:run(): START\n");
                endTime_msec = 0;
            }
            controls.logicalSet( CHANNEL_THROTTLE, PCT_TO_CHANNEL(engineCut->getEngineCutPct()));
            break;

        case STATE_START:
            // If engine cut tuned on again, cancel activation
            if( engineCut && engineCut->isSave()) {
                state = STATE_DEACTIVATING;
                LOG("MotorStart:run(): DEACTIVATING\n");
                break;
            }
            if( endTime_msec == 0) {
                // Set new delay end time
                endTime_msec = millis() + CFG->delayTime*100;
            } else if( millis() > endTime_msec) {
                // delay time exceeded
                state = STATE_DEACTIVATING;
                LOG("MotorStart:run(): DEACTIVATING\n");
                endTime_msec = 0;
            }

            /* The target position in 1/10 % scaled to 1/100 % */
            targetPosition10 = controls.logicalGet( CHANNEL_THROTTLE) * SCALING_F;
            if( CFG->delayTime > 0 && targetPosition10 > lastChannelValue10 ) {
                // posDelay_sec is a scaled float in 1/10 sec resolution. Convert to msec.
                delay_msec = CFG->delayTime * 100;
                stepPosition10 = (int16_t)((int32_t)((CHANNELVALUE_MAX - CHANNELVALUE_MIN) * SCALING_F) * PPM_FRAME_TIME_usec / 1000 / delay_msec);
                lastChannelValue10 += stepPosition10;
                if( lastChannelValue10 > targetPosition10) { // Do not exceed targeted value.
                    lastChannelValue10 = targetPosition10;
                }
            } else {
                lastChannelValue10 = targetPosition10;
            }
            controls.logicalSet( CHANNEL_THROTTLE, (channelValue_t)(lastChannelValue10 / SCALING_F));
            break;

        case STATE_DEACTIVATING:
            if( IS_SWITCH_USED(CFG->activateSw)) {
                sw = controls.switchGet(GET_SWITCH(CFG->activateSw));
                if( sw != GET_SWITCH_STATE(CFG->activateSw )) {
                    state = STATE_INACTIVE;
                    LOG("MotorStart:run(): INACTIVE\n");
                }
            }
            break;
    }

    lastChannelValue10 = controls.logicalGet(CHANNEL_THROTTLE) * SCALING_F;
}

void MotorStart::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        CFG->phase = 0;
        INIT_SWITCH( CFG->activateSw);
        CFG->trigger_pct = 0;
        CFG->leadingTime = 20; // 2.0 Seconds
        CFG->delayTime = 20; // 2.0 Seconds
    )

    inModule = false;
    state = STATE_INACTIVE;
}

/* From TableEditable */
uint8_t MotorStart::getRowCount() {

    return 5;
}

const char *MotorStart::getRowName( uint8_t row) {

    switch( row) {
        case 0:
            return TEXT_PHASE;
        case 1:
            return TEXT_ACTIVE;
        case 2:
            return TEXT_TRIGGER;
        case 3:
            return TEXT_LEADING;
        case 4:
            return TEXT_DELAY;
    }

    return TEXT_MSG_NONE;
}

uint8_t MotorStart::getColCount( uint8_t row) {

    return 1;
}

void MotorStart::getValue( uint8_t row, uint8_t col, Cell *cell) {

    switch( row) {
        case 0:
            cell->setList(6, PhaseNames, TEXT_PHASES_count, CFG->phase);;
            break;
        case 1:
            cell->setSwitchState( 8, CFG->activateSw);
            break;
        case 2:
            cell->setInt8( 9, CFG->trigger_pct, 0, PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
            break;
        case 3:
            cell->setFloat1( 9, CFG->leadingTime, 4, 0, LEADING_MAX_SEC * 10 /* 10.0 sec */);
            break;
        case 4:
            cell->setFloat1( 9, CFG->delayTime, 4, 0, DELAY_MAX_SEC * 10 /* 10.0 sec */);
            break;
    }
}

void MotorStart::setValue( uint8_t row, uint8_t col, Cell *cell) {

    switch( row) {
        case 0:
            CFG->phase = cell->getList();
            break;
        case 1:
            CFG->activateSw = cell->getSwitchState();
            break;
        case 2:
            CFG->trigger_pct = cell->getInt8();
            break;
        case 3:
            CFG->leadingTime = cell->getFloat1();
            break;
        case 4:
            CFG->delayTime = cell->getFloat1();
            break;
    }
}
