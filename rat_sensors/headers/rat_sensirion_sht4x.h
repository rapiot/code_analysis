// ----------------------------------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2021 - 2023 Rapiot Open Hardware Project
//
// Known limitations :
//
//   There are no known limitations.
//
// Known issues :
//
//   There are no known issues.
//
// Test instructions :
//
//   Use mikroBus socket 1 with EasyPIC v8.
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Sensirion SHT4X Header File
//
// The following versions are supported :
//
//   - Sensirion SHT40
//   - Sensirion SHT41
//   - Sensirion SHT45
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// ----------------------------------------------------------------------------------------------------
// Defines
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Address
// ----------------------------------------------------------------------------------------------------
#define RAT_HUMIDITY_SENSOR_ADDRESS 0x44

// ----------------------------------------------------------------------------------------------------
// Commands
// ----------------------------------------------------------------------------------------------------
#define RAT_HUMIDITY_SENSOR_MEASURE_REPEATABILITY_HGH 0xFD
#define RAT_HUMIDITY_SENSOR_MEASURE_REPEATABILITY_MED 0xF6
#define RAT_HUMIDITY_SENSOR_MEASURE_REPEATABILITY_LOW 0xE0

#define RAT_HUMIDITY_SENSOR_READ_SERIAL 0x89
#define RAT_HUMIDITY_SENSOR_SOFT_RESET  0x94

#define RAT_HUMIDITY_SENSOR_HEATER_HGH_PRECISION_HIGH_1_0_S 0x39
#define RAT_HUMIDITY_SENSOR_HEATER_HGH_PRECISION_HIGH_0_1_S 0x32
#define RAT_HUMIDITY_SENSOR_HEATER_MED_PRECISION_HIGH_1_0_S 0x2F
#define RAT_HUMIDITY_SENSOR_HEATER_MED_PRECISION_HIGH_0_1_S 0x24
#define RAT_HUMIDITY_SENSOR_HEATER_LOW_PRECISION_HIGH_1_0_S 0x1E
#define RAT_HUMIDITY_SENSOR_HEATER_LOW_PRECISION_HIGH_0_1_S 0x15

// ----------------------------------------------------------------------------------------------------
// CRC
// ----------------------------------------------------------------------------------------------------
#define RAT_HUMIDITY_SENSOR_INITIALIZATION 0xFF
#define RAT_HUMIDITY_SENSOR_POLYNOMIAL     0x31

// ----------------------------------------------------------------------------------------------------
// Minimums and maximums
// ----------------------------------------------------------------------------------------------------
#define RAT_HUMIDITY_SENSOR_TEMPERATURE_MINIMUM -40.0   // The minimum temperature rating is - 40 C
#define RAT_HUMIDITY_SENSOR_TEMPERATURE_MAXIMUM  85.0   // The maximum temperature rating is + 85 C

#define RAT_HUMIDITY_SENSOR_HUMIDITY_MINIMUM   0.0
#define RAT_HUMIDITY_SENSOR_HUMIDITY_MAXIMUM 100.0

// ----------------------------------------------------------------------------------------------------
// Generic delays
// ----------------------------------------------------------------------------------------------------
#define RAT_HUMIDITY_SENSOR_STABILIZATION_DELAY 1000   // 1,000 ms
#define RAT_HUMIDITY_SENSOR_RESET_DELAY 1000           // 1,000 ms

// ----------------------------------------------------------------------------------------------------
// Specific delays
// ----------------------------------------------------------------------------------------------------
#define SHT4X_MEASUREMENT_DELAY 100   // 100 ms

// ----------------------------------------------------------------------------------------------------
// Typedefs
// ----------------------------------------------------------------------------------------------------
typedef enum rat_humidity_sensor_repeatabilities {RAT_HUMIDITY_SENSOR_REPEATABILITY_HGH,
                                                  RAT_HUMIDITY_SENSOR_REPEATABILITY_MED,
                                                  RAT_HUMIDITY_SENSOR_REPEATABILITY_LOW} rat_humidity_sensor_repeatability;

// ----------------------------------------------------------------------------------------------------
// Pin names, types, and directions
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Power
// ----------------------------------------------------------------------------------------------------
sbit RAT_HUMIDITY_SENSOR_PWR_PIN at LATC.B2;
sbit RAT_HUMIDITY_SENSOR_PWR_DIR at TRISC.B2;
sbit RAT_HUMIDITY_SENSOR_PWR_TYP at ANSELC.B2;

// ----------------------------------------------------------------------------------------------------
// Functions
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Init
// ----------------------------------------------------------------------------------------------------
void rat_humidity_sensor_init (void);

// ----------------------------------------------------------------------------------------------------
// Software reset
// ----------------------------------------------------------------------------------------------------
void rat_humidity_sensor_software_reset (void);

// ----------------------------------------------------------------------------------------------------
// Read serial
//
// Returns true if the checksums match; false otherwise.
// ----------------------------------------------------------------------------------------------------
uint8_t rat_humidity_sensor_read_serial (uint8_t * serial);

// ----------------------------------------------------------------------------------------------------
// Measure the temperature and humidity
//
// Returns true if the checksums match; false otherwise.
// ----------------------------------------------------------------------------------------------------
uint8_t rat_humidity_sensor_measure (float * temperature,
                                     float * humidity);
