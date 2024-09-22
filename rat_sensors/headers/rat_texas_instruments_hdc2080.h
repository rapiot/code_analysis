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
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Texas Instruments HDC2080 Header File
// ----------------------------------------------------------------------------------------------------

#include "../../rat_application/headers/defines.h"

// ----------------------------------------------------------------------------------------------------
// Conditional compilation
// ----------------------------------------------------------------------------------------------------
#ifdef TEXAS_INSTRUMENTS_HDC2080

// ----------------------------------------------------------------------------------------------------
// Defines
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Address
// ----------------------------------------------------------------------------------------------------
#define RAT_HUMIDITY_SENSOR_ADDRESS 0x40

// ----------------------------------------------------------------------------------------------------
// Addresses
// ----------------------------------------------------------------------------------------------------
#define RAT_HUMIDITY_SENSOR_TEMPERATURE_ADDRESS                        0x00
#define RAT_HUMIDITY_SENSOR_HUMIDITY_ADDRESS                           0x02
#define RAT_HUMIDITY_SENSOR_INTERRUPT_DATA_READY_ADDRESS               0x04
#define RAT_HUMIDITY_SENSOR_TEMPERATURE_MAX_ADDRESS                    0x05
#define RAT_HUMIDITY_SENSOR_HUMIDITY_MAX_ADDRESS                       0x06
#define RAT_HUMIDITY_SENSOR_INTERRUPT_ENABLE_ADDRESS                   0x07
#define RAT_HUMIDITY_SENSOR_TEMPERATURE_OFFSET_ADJUST_ADDRESS          0x08
#define RAT_HUMIDITY_SENSOR_HUMIDITY_OFFSET_ADJUST_ADDRESS             0x09
#define RAT_HUMIDITY_SENSOR_TEMPERATURE_THRESHOLD_LOW_ADDRESS          0x0A
#define RAT_HUMIDITY_SENSOR_TEMPERATURE_THRESHOLD_HIGH_ADDRESS         0x0B
#define RAT_HUMIDITY_SENSOR_HUMIDITY_THRESHOLD_LOW_ADDRESS             0x0C
#define RAT_HUMIDITY_SENSOR_HUMIDITY_THRESHOLD_HIGHADDRESS             0x0D
#define RAT_HUMIDITY_SENSOR_SOFT_RESET_INTERRUPT_CONFIGURATION_ADDRESS 0x0E
#define RAT_HUMIDITY_SENSOR_MEASUREMENT_CONFIGURATION_ADDRESS          0x0F
#define RAT_HUMIDITY_SENSOR_MANUFACTURER_ID_ADDRESS                    0xFC
#define RAT_HUMIDITY_SENSOR_DEVICE_ID_ADDRESS                          0xFE

// ----------------------------------------------------------------------------------------------------
// Parameters
// ----------------------------------------------------------------------------------------------------
#define RAT_HUMIDITY_SENSOR_TEMPERATURE_RESOLUTION_14_BIT 0x00
#define RAT_HUMIDITY_SENSOR_TEMPERATURE_RESOLUTION_11_BIT 0x01
#define RAT_HUMIDITY_SENSOR_TEMPERATURE_RESOLUTION_8_BIT  0x02

#define RAT_HUMIDITY_SENSOR_HUMIDITY_RESOLUTION_14_BIT    0x00
#define RAT_HUMIDITY_SENSOR_HUMIDITY_RESOLUTION_11_BIT    0x01
#define RAT_HUMIDITY_SENSOR_HUMIDITY_RESOLUTION_8_BIT     0x02

#define RAT_HUMIDITY_SENSOR_MEASUREMENT_CONFIGURATION_T_H 0x00
#define RAT_HUMIDITY_SENSOR_MEASUREMENT_CONFIGURATION_T   0x01
#define RAT_HUMIDITY_SENSOR_MEASUREMENT_CONFIGURATION_H   0x02

#define RAT_HUMIDITY_SENSOR_MEASUREMENT_TRIGGER_ENABLED   0x01
#define RAT_HUMIDITY_SENSOR_MEASUREMENT_TRIGGER_DISABLED  0x00

// ----------------------------------------------------------------------------------------------------
// Minimums and maximums
// ----------------------------------------------------------------------------------------------------
#define RAT_HUMIDITY_SENSOR_TEMPERATURE_MINIMUM -40   // The minimum temperature rating is - 40 C
#define RAT_HUMIDITY_SENSOR_TEMPERATURE_MAXIMUM  85   // The maximum temperature rating is + 85 C

#define RAT_HUMIDITY_SENSOR_HUMIDITY_MINIMUM      0
#define RAT_HUMIDITY_SENSOR_HUMIDITY_MAXIMUM    100

// ----------------------------------------------------------------------------------------------------
// Delays
// ----------------------------------------------------------------------------------------------------
#define RAT_HUMIDITY_SENSOR_STABILIZATION_DELAY 1000   // 1,000 ms
#define RAT_HUMIDITY_SENSOR_RESET_DELAY         1000   // 1,000 ms
#define RAT_HUMIDITY_SENSOR_RESPONSE_DELAY       100   //   100 ms

// ----------------------------------------------------------------------------------------------------
// Pin names, types, and directions
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Temperature and humidity sensor power
// ----------------------------------------------------------------------------------------------------
sbit RAT_HUMIDITY_SENSOR_PWR_PIN at LATC.B2;
sbit RAT_HUMIDITY_SENSOR_PWR_DIR at TRISC.B2;
sbit RAT_HUMIDITY_SENSOR_PWR_TYP at ANSELC.B2;

// ----------------------------------------------------------------------------------------------------
// Temperature and humidity sensor interrupt
// ----------------------------------------------------------------------------------------------------
sbit RAT_HUMIDITY_SENSOR_INT_PIN at PORTB.B0;
sbit RAT_HUMIDITY_SENSOR_INT_DIR at TRISB.B0;
sbit RAT_HUMIDITY_SENSOR_INT_TYP at ANSELB.B0;

// ----------------------------------------------------------------------------------------------------
// Functions
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Init
// ----------------------------------------------------------------------------------------------------
void rat_humidity_sensor_init (void);

// ----------------------------------------------------------------------------------------------------
// Measure
//
// Returns true (1) if the checksum matches; false (0) otherwise.
//
// Since the sensor does NOT support checksum, the function always returns true.
// ----------------------------------------------------------------------------------------------------
uint8_t rat_humidity_sensor_measure (float * temperature,
                                     float * humidity);

#endif