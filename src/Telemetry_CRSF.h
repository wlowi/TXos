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

#ifndef _Telemetry_CRSF_h_
#define _Telemetry_CRSF_h_

#include "Telemetry.h"

/*
 * The following structures and enums are used in Telemetry.cpp.
 * Do not change their names.
 *
 * TELEMETRY_SENSORS
 * TELEMETRY_SENSOR_NAMES
 * TELEMETRY_UNITS
 * TELEMETRY_UNIT_NAMES
 * TELEMETRY_FRAMES
 */

#define TFRAME_BATTERY 0x08
#define TFRAME_LINK 0x14
#define TFRAME_RADIO 0x3A

enum TELEMETRY_SENSORS {
    RX_RSSI1_INDEX = 0,
    RX_RSSI2_INDEX,
    RX_QUALITY_INDEX,
    RX_SNR_INDEX,
    RX_ANTENNA_INDEX,
    RF_MODE_INDEX,
    TX_POWER_INDEX,
    TX_RSSI_INDEX,
    TX_QUALITY_INDEX,
    TX_SNR_INDEX,
    RX_RSSI_PERC_INDEX,
    RX_RF_POWER_INDEX,
    TX_RSSI_PERC_INDEX,
    TX_RF_POWER_INDEX,
    TX_FPS_INDEX,
    BATT_VOLTAGE_INDEX,
    BATT_CURRENT_INDEX,
    BATT_CAPACITY_INDEX,
    BATT_REMAINING_INDEX,
    GPS_LATITUDE_INDEX,
    GPS_LONGITUDE_INDEX,
    GPS_GROUND_SPEED_INDEX,
    GPS_HEADING_INDEX,
    GPS_ALTITUDE_INDEX,
    GPS_SATELLITES_INDEX,
    ATTITUDE_PITCH_INDEX,
    ATTITUDE_ROLL_INDEX,
    ATTITUDE_YAW_INDEX,
    FLIGHT_MODE_INDEX,
    VERTICAL_SPEED_INDEX,
    BARO_ALTITUDE_INDEX,
    AIRSPEED_INDEX,
    CF_RPM_INDEX,
    TEMP_INDEX,
    CELLS_INDEX,
    VOLT_ARRAY_INDEX,
    UNKNOWN_INDEX,

    TELEMETRY_SENSORS_COUNT
};

const char *TELEMETRY_SENSOR_NAMES[] = {
    "Rx RSSI1", "Rx RSSI2",  "Rx Qual",  "Rx SNR",    "Rx ANT",   "RF Mode",
    "Tx Pwr",   "Tx RSSI",   "Tx Qual",  "Tx SNR",    "Rx RSSI%", "Rx RF PWR",
    "Tx RSSI%", "Tx RF PWR", "Tx FPS",   "Bat V",     "Bat A",    "Bat C",
    "Bat Rem",  "GPS LAT",   "GPS LONG", "GPS SPEED", "GPS HEAD", "GPS ALT",
    "GPS SATS", "ATT PITCH", "ATT ROLL", "ATT YAW",   "F MODE",   "VERT SPEED",
    "BARO ALT", "AIRSPEED",  "CF RPM",   "TEMP",      "CELLS",    "VOLT ARR",
    "UNKNOWN",
};

enum TELEMETRY_UNITS {
    UNIT_RAW,
    UNIT_VOLTS,
    UNIT_AMPS,
    UNIT_MILLIAMPS,
    UNIT_KTS,
    UNIT_METERS_PER_SECOND,
    UNIT_FEET_PER_SECOND,
    UNIT_KMH,
    UNIT_SPEED = UNIT_KMH,
    UNIT_MPH,
    UNIT_METERS,
    UNIT_DIST = UNIT_METERS,
    UNIT_FEET,
    UNIT_CELSIUS,
    UNIT_TEMPERATURE = UNIT_CELSIUS,
    UNIT_FAHRENHEIT,
    UNIT_PERCENT,
    UNIT_MAH,
    UNIT_WATTS,
    UNIT_MILLIWATTS,
    UNIT_DB,
    UNIT_RPMS,
    UNIT_G,
    UNIT_DEGREE,
    UNIT_RADIANS,
    UNIT_MILLILITERS,
    UNIT_FLOZ,
    UNIT_MILLILITERS_PER_MINUTE,
    UNIT_HERTZ,
    UNIT_MS,
    UNIT_US,
    UNIT_KM,
    UNIT_DBM,
    UNIT_MAX = UNIT_DBM,
    UNIT_SPARE6,
    UNIT_SPARE7,
    UNIT_SPARE8,
    UNIT_SPARE9,
    UNIT_SPARE10,
    UNIT_HOURS,
    UNIT_MINUTES,
    UNIT_SECONDS,

    TELEMETRY_UNITS_COUNT
};

const char *TELEMETRY_UNIT_NAMES[] = {
    "",    "V",   "A",  "mA",   "KTS",  "m/s", "ft/s", "kmh",  "mph", "m",
    "ft",  "C",   "F",  "%",    "mAH",  "W",   "mW",   "db",   "rpm", "G",
    "deg", "rad", "ml", "floz", "ml/m", "Hz",  "msec", "usec", "km",  "dbm",
    "",    "",    "",   "",     "",     "H",   "M",    "S",
};

#define TFRAME_BATTERY_SENSOR_COUNT 4
const SensorDefinition_t TFRAME_BATTERY_SENSORS[TFRAME_BATTERY_SENSOR_COUNT] = {
    // name,  offs, len, prec, unit
    {BATT_VOLTAGE_INDEX, 1, 2, 1, UNIT_VOLTS, nullptr},
    {BATT_CURRENT_INDEX, 3, 2, 1, UNIT_AMPS, nullptr},
    {BATT_CAPACITY_INDEX, 5, 3, 0, UNIT_MILLIAMPS, nullptr},
    {BATT_REMAINING_INDEX, 8, 1, 0, UNIT_PERCENT, nullptr}};

long powerConversion(long pwr) {

    switch (pwr) {
    case 0:
        return 0;
    case 1:
        return 10;
    case 2:
        return 25;
    case 3:
        return 100;
    case 4:
        return 500;
    case 5:
        return 1000;
    case 6:
        return 2000;
    case 7:
        return 250;
    case 8:
        return 50;
    default:
        return 0;
    }
}

long negate(long db) { return -(255 - db); }

#define TFRAME_LINK_SENSOR_COUNT 10
const SensorDefinition_t TFRAME_LINK_SENSORS[TFRAME_LINK_SENSOR_COUNT] = {
    {RX_RSSI1_INDEX, 1, 1, 0xff, UNIT_DB, negate},
    {RX_RSSI2_INDEX, 2, 1, 0xff, UNIT_DB, negate},
    {RX_QUALITY_INDEX, 3, 1, 0, UNIT_PERCENT, nullptr},
    {RX_SNR_INDEX, 4, 1, 0xff, UNIT_DB, negate},
    {RX_ANTENNA_INDEX, 5, 1, 0, UNIT_RAW, nullptr},
    {RF_MODE_INDEX, 6, 1, 0, UNIT_RAW, nullptr},
    {TX_POWER_INDEX, 7, 1, 0xff, UNIT_MILLIWATTS, powerConversion},
    {TX_RSSI_INDEX, 8, 1, 0xff, UNIT_DB, negate},
    {TX_QUALITY_INDEX, 9, 1, 0, UNIT_PERCENT, nullptr},
    {TX_SNR_INDEX, 10, 1, 0xff, UNIT_DB, negate}};

/*
#define TFRAME_RADIO_SENSOR_COUNT 5
const SensorDefinition_t TFRAME_RADIO_SENSORS[TFRAME_RADIO_SENSOR_COUNT] = {
    { "R Addr",   1, 1, 0, TUNIT_NONE },
    { "M Addr",   2, 1, 0, TUNIT_NONE },
    { "TCF",      3, 1, 0, TUNIT_NONE },
    { "Intv",     4, 4, 1, TUNIT_uSec },
    { "Offs",     8, 4, 1, TUNIT_uSec }
};
*/

const TelemetryFrame_t TELEMETRY_FRAMES[] = {
    {TFRAME_BATTERY, TFRAME_BATTERY_SENSOR_COUNT, TFRAME_BATTERY_SENSORS},
    {TFRAME_LINK, TFRAME_LINK_SENSOR_COUNT, TFRAME_LINK_SENSORS},

    /* Keep End Marker */
    {TFRAME_ENDMARKER, 0, nullptr}};

#endif
