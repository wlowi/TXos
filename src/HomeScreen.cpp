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

#include "HomeScreen.h"
#include "ModuleManager.h"
#include "Model.h"
#include "ModelSelect.h"
#include "Buzzer.h"

extern ModuleManager moduleManager;
extern ModelSelect modelSelect;
extern Buzzer buzzer;

const buzzerCmd_t SoundClear[] = {
    BUZZER_PLAY(1),
    BUZZER_PAUSE(1),
    BUZZER_REPEAT(0, 2),
    BUZZER_STOP()
};

const buzzerCmd_t SoundAlarm[] = {
    BUZZER_PAUSE(20),
    BUZZER_PLAY(1),
    BUZZER_PAUSE(1),
    BUZZER_REPEAT(1, 5),
    BUZZER_PAUSE(20),
    BUZZER_STOP()
};

const char* messageText[] = {
    TEXT_MSG_NONE,
    TEXT_MSG_BAD_SYSCONFIG,
    TEXT_MSG_LOW_BATT,
    TEXT_MSG_MODEL_LOAD_FAILED,
    TEXT_MSG_CONFIG_SIZE,
    TEXT_MSG_MODEL_IMP_FAILED
};

HomeScreen::HomeScreen() : TextUIScreen() {

    message1 = message2 = 0;
    post1 = post2 = 0;
    arg1 = arg2 = 0;

    lastPhase = 255;
    lastVcc = 0;
    lastTime = 0;
    engineSave = false;
}

void HomeScreen::printDebug(uint16_t t) {

    lcd->setCursor(0, 7);
    lcd->setFg(255, 165, 0);
    lcd->printUInt(t, 6);
}

void HomeScreen::postMessage(uint8_t line, uint8_t msg, uint8_t arg) {

    if (msg < TEXT_MSG_count) {
        if (line == 0) {
            post1 = msg;
            arg1 = arg;
        }
        else {
            post2 = msg;
            arg2 = arg;
        }
    }
}

void HomeScreen::postMessage(uint8_t line, uint8_t msg) {

    postMessage( line, msg, 0);
}

/* From TextUIScreen */
void HomeScreen::handleEvent(TextUI* ui, Event* e) {

    // LOGV("HomeScreen::handleEvent: %d\n", e->getType());

    if (lcd == nullptr) {
        lcd = ui->getDisplay();
        screenWidth = lcd->getColumns();
    }

    // TextUILcd* lcd = ui->getDisplay();

    if (refresh == REFRESH_FULL) {
        lcd->normalColors();
        lcd->clear();

        lcd->printStr(TEXT_TXOS, 5);
        lcd->printStr(TXOS_VERSION);

        lcd->setCursor(1, 0);
        lcd->printUInt(modelSelect.getModelID(), 2);
        lcd->printStr(": ");
        Model* model = (Model*)moduleManager.getModuleByType(MODULE_SET_MODEL, MODULE_MODEL_TYPE);
        lcd->printStr(model->getModelName(), MODEL_NAME_LEN);
    }

    if (engineCut && (refresh == REFRESH_FULL || engineCut->isSave() != engineSave)) {
        engineSave = engineCut->isSave();

        lcd->setFg(0, 255, 0);
        lcd->setCursor(2, 10);
        lcd->printStr(engineSave ? TEXT_THR : TEXT_MSG_NONE, 3);
    }

    if (phases && (refresh == REFRESH_FULL || phases->getPhase() != lastPhase)) {
        lastPhase = phases->getPhase();

        lcd->normalColors();
        lcd->setCursor(3, 3);
        lcd->printStr(phases->getPhaseName(), TEXT_PHASE_NAME_length);
    }

    if (timer && (refresh == REFRESH_FULL || timer->timeSec() != lastTime)) {

        lastTime = timer->timeSec();

        lcd->normalColors();
        lcd->setCursor(5, 0);
        lcd->printStr(timer->timeStr(), 5);
    }

    if (vccMonitor && (refresh == REFRESH_FULL || vccMonitor->getVcc() != lastVcc)) {

        lastVcc = vccMonitor->getVcc();

        if (vccMonitor->belowAlert()) {
            lcd->setFg(255, 0, 0);
            postMessage(0, MSG_LOW_BATT);
            buzzer.playPermanent(SoundAlarm);
        }
        else if (vccMonitor->belowWarn()) {
            lcd->setFg(255, 165, 0);
        }
        else {
            lcd->setFg(0, 255, 0);
        }
        lcd->setCursor(5, 7);
        lcd->printFixFloat2(lastVcc, 5);
        lcd->printStr("V");
    }

    if (refresh == REFRESH_FULL || post1 != message1) {
        message1 = post1;
        lcd->setFg(255, 165, 0);
        lcd->setCursor(6, 0);
        lcd->printStr(messageText[message1], screenWidth);
        if( arg1 ) {
            lcd->setCursor(6, screenWidth-1);
            lcd->printUInt( arg1, 1);
        }
    }

    if (refresh == REFRESH_FULL || post2 != message2) {
        message2 = post2;
        lcd->setFg(255, 165, 0);
        lcd->setCursor(7, 0);
        lcd->printStr(messageText[message2], screenWidth);
        if( arg2 ) {
            lcd->setCursor(7, screenWidth-1);
            lcd->printUInt( arg2, 1);
        }
    }

    refresh = REFRESH_OK;

    if (e->getType() == EVENT_TYPE_KEY) {
        switch (e->getKey()) {

        case KEY_ENTER:
            ui->pushScreen(moduleManager.getModelMenu());
            break;

        case KEY_CLEAR:
            if (post1 || post2) {
                /* Clear messages and switch off alarm */
                post1 = post2 = 0;
                arg1 = arg2 = 0;
                buzzer.off();
            }
            else if (timer) {
                timer->reset();
                buzzer.play(SoundClear);
            }
        }
    }
}

void HomeScreen::activate(TextUI* ui) {

    if (lcd == nullptr) {
        lcd = ui->getDisplay();
        screenWidth = lcd->getColumns();

        vccMonitor = (VccMonitor*)moduleManager.getModuleByType(MODULE_SET_SYSTEM, MODULE_VCC_MONITOR_TYPE);
        phases = (Phases*)moduleManager.getModuleByType(MODULE_SET_MODEL, MODULE_PHASES_TYPE);
        timer = (Timer*)moduleManager.getModuleByType(MODULE_SET_MODEL, MODULE_TIMER_TYPE);
        engineCut = (EngineCut*)moduleManager.getModuleByType(MODULE_SET_MODEL, MODULE_ENGINE_CUT_TYPE);
    }

    refresh = REFRESH_FULL;
}