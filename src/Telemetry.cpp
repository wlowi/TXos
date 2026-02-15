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

#include "Telemetry.h"

#if HF_MODULE == HF_SPEKTRUM_SERIAL
#include "Telemetry_SPEKTRUM.h"
#elif HF_MODULE == HF_CRSF_SERIAL
#include "Telemetry_CRSF.h"
#endif

/*
 * const TelemetryFrame_t TELEMETRY_FRAMES[]
 */

Telemetry::Telemetry() : lastUpdate(0) {

    /* clear telemetry screens */
    for (uint8_t s = 0; s < TELEMETRY_SCREENS; s++) {
        for (uint8_t l = 0; l < TELEMETRY_LINES; l++) {
            screens[s].lines[l].state = SENSOR_UNUSED;
        }
    }

    screens[0].lines[0].sensor_id = BATT_VOLTAGE_INDEX;
    screens[0].lines[1].sensor_id = TX_POWER_INDEX;
    screens[0].lines[2].sensor_id = RX_RSSI1_INDEX;
    screens[0].lines[3].sensor_id = RX_QUALITY_INDEX;
    screens[0].lines[4].sensor_id = TX_RSSI_INDEX;
    screens[0].lines[5].sensor_id = TX_QUALITY_INDEX;

    screens[0].lines[0].state = SENSOR_EXPIRED;
    screens[0].lines[1].state = SENSOR_EXPIRED;
    screens[0].lines[2].state = SENSOR_EXPIRED;
    screens[0].lines[3].state = SENSOR_EXPIRED;
    screens[0].lines[4].state = SENSOR_EXPIRED;
    screens[0].lines[5].state = SENSOR_EXPIRED;
}

void Telemetry::parseTelemetryFrame(uint8_t fType, uint8_t *payload) {

    const TelemetryFrame_t *frameDefinition =
        findFrameDescriptionForType(fType);
    const SensorDefinition_t *sensorDefinition;
    int32_t value;

    if (frameDefinition) {
        for (uint8_t i = 0; i < frameDefinition->sensorCount; i++) {
            sensorDefinition = &(frameDefinition->sensors[i]);

            value = parseValue(sensorDefinition, payload);

            if (sensorDefinition->precision == 0xff &&
                sensorDefinition->callback) {
                value = sensorDefinition->callback(value);
            }

            setValue(sensorDefinition->sensor_id, value,
                     sensorDefinition->precision, sensorDefinition->unit_id);
        }
    }
}

const TelemetryFrame_t *Telemetry::findFrameDescriptionForType(uint8_t fType) {

    const TelemetryFrame_t *fd;

    for (fd = TELEMETRY_FRAMES; fd != nullptr; fd++) {
        if (fd->type == TFRAME_ENDMARKER) {
            fd = nullptr;
            break;
        } else if (fd->type == fType) {
            break;
        }
    }

    return fd;
}

int32_t Telemetry::parseValue(const SensorDefinition_t *sensorDef,
                              uint8_t *payload) {

    int32_t value = 0;
    uint8_t offs = sensorDef->offset;

    for (uint8_t l = 0; l < sensorDef->len; l++) {
        value <<= 8;
        value += payload[offs++];
    }

    return value;
}

void Telemetry::setValue(uint8_t sensor_id, int32_t value, uint8_t precision,
                         uint8_t unit_id) {

    for (uint8_t s = 0; s < TELEMETRY_SCREENS; s++) {
        for (uint8_t l = 0; l < TELEMETRY_LINES; l++) {
            if (screens[s].lines[l].state != SENSOR_UNUSED &&
                screens[s].lines[l].sensor_id == sensor_id) {
                screens[s].lines[l].value = value;
                screens[s].lines[l].precision = precision;
                screens[s].lines[l].unit_id = unit_id;
                screens[s].lines[l].state = SENSOR_VALID;
            }
        }
    }
}

TelemetryScreen *Telemetry::getTelemetryScreen(uint8_t screen_no) {

    if (screen_no < TELEMETRY_SCREENS) {
        return &screens[screen_no];
    } else {
        return &screens[0];
    }
}

const char *Telemetry::readableUnit(uint8_t unit_id) {

    if (unit_id < TELEMETRY_UNITS_COUNT) {
        return TELEMETRY_UNIT_NAMES[unit_id];
    } else {
        return "???";
    }
}

const char *Telemetry::readableName(uint8_t sensor_id) {

    if (sensor_id < TELEMETRY_SENSORS_COUNT) {
        return TELEMETRY_SENSOR_NAMES[sensor_id];
    } else {
        return "???";
    }
}
