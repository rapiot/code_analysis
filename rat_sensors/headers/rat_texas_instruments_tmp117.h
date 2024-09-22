// ----------------------------------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2020 Rapiot Open Hardware Project
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
// Texas Instruments TMP117 Header File
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------------------------------
#include <stdint.h>

#include "../headers/rat_sensors.h"

#ifdef TEXAS_INSTRUMENTS_TMP117

// ----------------------------------------------------------------------------------------------------
// Defines
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Address
//
// 0x48 - The address pin is set to logic LOW
// 0x49 - The address pin is set to logic HIGH
// ----------------------------------------------------------------------------------------------------
#define RAT_TEMPERATURE_SENSOR_ADDRESS 0x48

// ----------------------------------------------------------------------------------------------------
// Register addresses
// ----------------------------------------------------------------------------------------------------
#define RAT_TEMPERATURE_SENSOR_TEMPERATURE_ADDRESS            0x00
#define RAT_TEMPERATURE_SENSOR_CONFIGURATION_ADDRESS          0x01
#define RAT_TEMPERATURE_SENSOR_TEMPERATURE_HIGH_LIMIT_ADDRESS 0x02
#define RAT_TEMPERATURE_SENSOR_TEMPERATURE_LOW_LIMIT_ADDRESS  0x03
#define RAT_TEMPERATURE_SENSOR_EEPROM_UNLOCK_ADDRESS          0x04
#define RAT_TEMPERATURE_SENSOR_EEPROM_1_ADDRESS               0x05
#define RAT_TEMPERATURE_SENSOR_EEPROM_2_ADDRESS               0x06
#define RAT_TEMPERATURE_SENSOR_TEMPERATURE_OFFSET_ADDRESS     0x07
#define RAT_TEMPERATURE_SENSOR_EEPROM_3_ADDRESS               0x08
#define RAT_TEMPERATURE_SENSOR_DEVICE_ID_ADDRESS              0x0F

// ----------------------------------------------------------------------------------------------------
// Configuration
// ----------------------------------------------------------------------------------------------------
#define RAT_TEMPERATURE_SENSOR_MEASUREMENT_MODE_CONTINUOUS 0
#define RAT_TEMPERATURE_SENSOR_MEASUREMENT_MODE_SHUTDOWN   1
#define RAT_TEMPERATURE_SENSOR_MEASUREMENT_MODE_ONE_SHOT   3

#define RAT_TEMPERATURE_SENSOR_AVERAGING_DISABLED          0
#define RAT_TEMPERATURE_SENSOR_AVERAGING_8                 1
#define RAT_TEMPERATURE_SENSOR_AVERAGING_32                2
#define RAT_TEMPERATURE_SENSOR_AVERAGING_64                3

// ----------------------------------------------------------------------------------------------------
// Minimums and maximums
// ----------------------------------------------------------------------------------------------------
#define RAT_TEMPERATURE_SENSOR_TEMPERATURE_MINIMUM -40   // The minimum temperature rating is - 40 C
#define RAT_TEMPERATURE_SENSOR_TEMPERATURE_MAXIMUM  85   // The maximum temperature rating is + 85 C

// ----------------------------------------------------------------------------------------------------
// Delays
// ----------------------------------------------------------------------------------------------------
#define RAT_TEMPERATURE_SENSOR_STABILIZATION_DELAY 1000   // 1,000 ms
#define RAT_TEMPERATURE_SENSOR_RESET_DELAY         1000   // 1,000 ms
#define RAT_TEMPERATURE_SENSOR_RESPONSE_DELAY       100   //   100 ms

// ----------------------------------------------------------------------------------------------------
// Pin names, types, and directions
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Temperature sensor power
// ----------------------------------------------------------------------------------------------------
sbit RAT_TEMPERATURE_SENSOR_PWR_PIN at LATC.B2;
sbit RAT_TEMPERATURE_SENSOR_PWR_DIR at TRISC.B2;
sbit RAT_TEMPERATURE_SENSOR_PWR_TYP at ANSELC.B2;

// ----------------------------------------------------------------------------------------------------
// Functions
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Init
// ----------------------------------------------------------------------------------------------------
void rat_temperature_sensor_init (void);

// ----------------------------------------------------------------------------------------------------
// Measure
//
// Returns true (1) if the checksum matches; false (0) otherwise.
//
// Since the sensor does NOT support checksum, the function always returns true.
// ----------------------------------------------------------------------------------------------------
uint8_t rat_temperature_sensor_measure (float * temperature);

#endif