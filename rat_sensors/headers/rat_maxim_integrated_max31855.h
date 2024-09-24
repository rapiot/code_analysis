// ----------------------------------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2021 - 2024 Rapiot Open Hardware Project
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Maxim Integrated MAX31855 Header File
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
// Minimums and maximums
// ----------------------------------------------------------------------------------------------------
#define RAT_THERMOCOUPLE_SENSOR_TEMPERATURE_MINIMUM -270
#define RAT_THERMOCOUPLE_SENSOR_TEMPERATURE_MAXIMUM 1800

// ----------------------------------------------------------------------------------------------------
// Delays
// ----------------------------------------------------------------------------------------------------
#define RAT_THERMOCOUPLE_SENSOR_STABILIZATION_DELAY 1000   // 1,000 ms
#define RAT_THERMOCOUPLE_SENSOR_RESET_DELAY 1000           // 1,000 ms
#define RAT_THERMOCOUPLE_SENSOR_CHIP_SELECT_DELAY 100      //   100 ms

// ----------------------------------------------------------------------------------------------------
// Typedefs
// ----------------------------------------------------------------------------------------------------
typedef enum rat_thermocouple_sensor_data_types {
  RAT_THERMOCOUPLE_SENSOR_THERMOCOUPLE_TEMPERATURE_DATA,
  RAT_THERMOCOUPLE_SENSOR_INTERNAL_TEMPERATURE_DATA
} rat_thermocouple_sensor_data_type;

typedef enum rat_thermocouple_sensor_selection_types {
  RAT_THERMOCOUPLE_SENSOR_LFT,
  RAT_THERMOCOUPLE_SENSOR_RGT
} rat_thermocouple_sensor_selection_type;

// ----------------------------------------------------------------------------------------------------
// Pin names, types, and directions
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Power pins
// ----------------------------------------------------------------------------------------------------
sbit RAT_THERMOCOUPLE_SENSOR_LFT_PWR_PIN at LATC.B2;
sbit RAT_THERMOCOUPLE_SENSOR_LFT_PWR_DIR at TRISC.B2;
sbit RAT_THERMOCOUPLE_SENSOR_LFT_PWR_TYP at ANSELC.B2;

sbit RAT_THERMOCOUPLE_SENSOR_RGT_PWR_PIN at LATC.B3;
sbit RAT_THERMOCOUPLE_SENSOR_RGT_PWR_DIR at TRISC.B3;
sbit RAT_THERMOCOUPLE_SENSOR_RGT_PWR_TYP at ANSELC.B3;

// ----------------------------------------------------------------------------------------------------
// Chip select pins
// ----------------------------------------------------------------------------------------------------
sbit RAT_THERMOCOUPLE_SENSOR_LFT_CSL_PIN at LATA.B5;
sbit RAT_THERMOCOUPLE_SENSOR_LFT_CSL_DIR at TRISA.B5;
sbit RAT_THERMOCOUPLE_SENSOR_LFT_CSL_TYP at ANSELA.B5;

sbit RAT_THERMOCOUPLE_SENSOR_RGT_CSL_PIN at LATA.B6;
sbit RAT_THERMOCOUPLE_SENSOR_RGT_CSL_DIR at TRISA.B6;
sbit RAT_THERMOCOUPLE_SENSOR_RGT_CSL_TYP at ANSELA.B6;

// ----------------------------------------------------------------------------------------------------
// Functions
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Init
// ----------------------------------------------------------------------------------------------------
void rat_thermocouple_sensor_init (void);

// ----------------------------------------------------------------------------------------------------
// Measure the temperature
// ----------------------------------------------------------------------------------------------------
void rat_thermocouple_sensor_measure (float   * thermocouple_temperature,
                                      float   * internal_temperature,
                                      uint8_t * fault_flag,
                                      uint8_t * short_vcc_flag,
                                      uint8_t * short_gnd_flag,
                                      uint8_t * open_circuit_flag);
