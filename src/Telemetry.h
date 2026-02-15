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

#ifndef _Telemetry_h_
#define _Telemetry_h_

#include "Comm.h"
#include "TXos.h"

typedef long ValueConversion_cb(long);

typedef struct SensorDefinition_t {
    uint8_t sensor_id;            // Sensor id
    uint8_t offset;               // data start offset in frame payload
    uint8_t len;                  // data length in bytes
    uint8_t precision;            // number of decimal digits to shift raw value
    uint8_t unit_id;              // unit
    ValueConversion_cb *callback; // convert value if precision is 0xff;
} SensorDefinition_t;

typedef struct TelemetryFrame_t {
    uint8_t type;
    uint8_t sensorCount;
    const SensorDefinition_t *sensors; // Array of sensors
} TelemetryFrame_t;

#define TFRAME_ENDMARKER 0x00

/* */

enum SensorState { SENSOR_UNUSED = 0, SENSOR_VALID, SENSOR_EXPIRED };

#define TELEMETRY_SCREENS 2
#define TELEMETRY_LINES 8

typedef struct TelemetryLine {
    SensorState state;
    uint8_t sensor_id;
    int32_t value;
    uint8_t precision;
    uint8_t unit_id;
} TelemetryLine;

typedef struct TelemetryScreen {
    TelemetryLine lines[TELEMETRY_LINES];
} TelemetryScreen;

/* */

class Telemetry {

  private:
    long lastUpdate;
    TelemetryScreen screens[TELEMETRY_SCREENS];

    const TelemetryFrame_t *findFrameDescriptionForType(uint8_t fType);
    int32_t parseValue(const SensorDefinition_t *sensorDef, uint8_t *payload);
    void setValue(uint8_t sensor_id, int32_t value, uint8_t precision,
                  uint8_t unit_id);

  public:
    Telemetry();

    void parseTelemetryFrame(uint8_t fType, uint8_t *payload);
    TelemetryScreen *getTelemetryScreen(uint8_t screen_no);
    const char *readableUnit(uint8_t unit_id);
    const char *readableName(uint8_t sensor_id);
};

#endif
