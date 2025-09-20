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

#include "BuzzerImpl.h"

/* System dependent implementations of Buzzer.
 * For more documentation see Buzzer.h
 *
 * This implementation uses Atmel Timer 3 compare A interrupt.
 */

extern BuzzerImpl* buzzerImpl;

static hw_timer_t* buzzerTimer = NULL;

static portMUX_TYPE buzzerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR buzzerTimerISR() {

    buzzerImpl->processNext();
}

BuzzerImpl::BuzzerImpl() : ports(NULL) {

}

void BuzzerImpl::init(Ports& p) {

    ports = &p;
    sound[0] = BUZZER_STOP();

    off();

    /* Timer runns at 10kHz */
    buzzerTimer = timerBegin( 10000);

    if (buzzerTimer) {
        timerStop(buzzerTimer);
        timerAttachInterrupt(buzzerTimer, &buzzerTimerISR);
    }
}

/*
 * Cancel sound sequence.
 */
void BuzzerImpl::off() {

    portENTER_CRITICAL(&buzzerMux);

    stopSound();
    alarm = false;
    soundAlarm = NULL;

    portEXIT_CRITICAL(&buzzerMux);
}

/*
 * The last command in array s must be BUZZER_STOP.
 */
void BuzzerImpl::play(const buzzerCmd_t s[]) {

    uint8_t i;

    portENTER_CRITICAL(&buzzerMux);

    stopSound();

    for (i = 0; (i < BUZZER_SOUND_LEN - 1) && (BUZZER_CMD(s[i]) != BUZZER_STOP_CMD); i++) {
        sound[i] = s[i];
    }
    sound[i] = BUZZER_STOP();

    portEXIT_CRITICAL(&buzzerMux);

    processNext();
}

void BuzzerImpl::playPermanent(const buzzerCmd_t s[]) {

    if (alarm && s == soundAlarm) {
        return;
    }

    portENTER_CRITICAL(&buzzerMux);

    stopSound();
    alarm = true;
    soundAlarm = s;

    portEXIT_CRITICAL(&buzzerMux);

    play(s);
}

/* PRIVATE */

/*
 * Cancel sound sequence.
 */
void BuzzerImpl::stopSound() {

    if (buzzerTimer) {
        timerStop(buzzerTimer);
    }

    sndIdx = 0;

    ports->buzzerOff();
}

/*
 * Process next buzzer sequence command.
 */
void BuzzerImpl::processNext() {

    bool notDone;
    uint8_t i;

    if (sndIdx >= BUZZER_SOUND_LEN) {
        stopSound();
        return;
    }

    portENTER_CRITICAL(&buzzerMux);

    timerStop(buzzerTimer);

    do {
        notDone = false;

        switch (BUZZER_CMD(sound[sndIdx])) {
        case BUZZER_STOP_CMD:
            stopSound();
            if (alarm) {
                /* Re-enable alarm */
                for (i = 0; (i < BUZZER_SOUND_LEN - 1) && (BUZZER_CMD(soundAlarm[i]) != BUZZER_STOP_CMD); i++) {
                    sound[i] = soundAlarm[i];
                }
                sound[i] = BUZZER_STOP();
                notDone = true;
            }
            break;

        case BUZZER_PLAY_CMD:
            ports->buzzerOn();
            scheduleInterrupt(BUZZER_TIME(sound[sndIdx]));
            sndIdx++;
            break;

        case BUZZER_PAUSE_CMD:
            ports->buzzerOff();
            scheduleInterrupt(BUZZER_TIME(sound[sndIdx]));
            sndIdx++;
            break;

        case BUZZER_REPEAT_CMD:
            if (BUZZER_COUNT(sound[sndIdx]) == 0) {
                /* Repeat forever */
                sndIdx = BUZZER_STEP(sound[sndIdx]);
            }
            else if (BUZZER_COUNT(sound[sndIdx]) == 1) {
                /* Done repeating */
                sndIdx++;
            }
            else {
                /* Decrease count by one.
                 * This works because the counter is implemented as the 3 LSB.
                 */
                sound[sndIdx]--;
                sndIdx = BUZZER_STEP(sound[sndIdx]);
            }
            notDone = true;
            break;
        }
    } while (notDone);

    portEXIT_CRITICAL(&buzzerMux);
}

/*
 *  Schedule next timer interrupt.
 *   t is in 100msec units
 */
void BuzzerImpl::scheduleInterrupt(uint8_t t) {

    if (buzzerTimer) {
        timerWrite(buzzerTimer, 0);
        timerAlarm(buzzerTimer, (uint64_t)t * 1000, false, 0);
        timerStart(buzzerTimer);
    }
}
