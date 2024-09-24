// -----------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2021 - 2024 Rapiot Open Hardware Project
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Maxim Integrated MAX31855 Source File
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../../rat_utilities/headers/rat_math_utilities.h"
#include "../../rat_utilities/headers/rat_pic_utilities.h"
#include "../../rat_utilities/headers/rat_i2c_utilities.h"
#include "../../rat_sensors/headers/rat_maxim_integrated_max31855.h"

// -----------------------------------------------------------------------------
// Static functions
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Set the pins
// -----------------------------------------------------------------------------
static void rat_thermocouple_sensor_set_pins (void)
{
  // ---------------------------------------------------------------------------
  // Power pins
  // ---------------------------------------------------------------------------
  RAT_THERMOCOUPLE_SENSOR_LFT_PWR_DIR = 0b0;
  RAT_THERMOCOUPLE_SENSOR_LFT_PWR_TYP = 0b0;
  RAT_THERMOCOUPLE_SENSOR_LFT_PWR_PIN = 0b0;
  
  RAT_THERMOCOUPLE_SENSOR_RGT_PWR_DIR = 0b0;
  RAT_THERMOCOUPLE_SENSOR_RGT_PWR_TYP = 0b0;
  RAT_THERMOCOUPLE_SENSOR_RGT_PWR_PIN = 0b0;
  
  // --------------------------------------------------------------------------------------------------
  // Chip select pins
  // --------------------------------------------------------------------------------------------------
  RAT_THERMOCOUPLE_SENSOR_LFT_CSL_DIR = 0b0;
  RAT_THERMOCOUPLE_SENSOR_LFT_CSL_TYP = 0b0;
  RAT_THERMOCOUPLE_SENSOR_LFT_CSL_PIN = 0b0;
  
  RAT_THERMOCOUPLE_SENSOR_RGT_CSL_DIR = 0b0;
  RAT_THERMOCOUPLE_SENSOR_RGT_CSL_TYP = 0b0;
  RAT_THERMOCOUPLE_SENSOR_RGT_CSL_PIN = 0b0;
}

// -----------------------------------------------------------------------------
// Reset
// -----------------------------------------------------------------------------
static void rat_thermocouple_sensor_reset (void)
{
  RAT_THERMOCOUPLE_SENSOR_LFT_PWR_PIN = 0b0;
  RAT_THERMOCOUPLE_SENSOR_RGT_PWR_PIN = 0b0;

  rat_delay(RAT_THERMOCOUPLE_SENSOR_RESET_DELAY);

  RAT_THERMOCOUPLE_SENSOR_LFT_PWR_PIN = 0b1;
  RAT_THERMOCOUPLE_SENSOR_RGT_PWR_PIN = 0b1;
}

// -----------------------------------------------------------------------------
// Enable
// -----------------------------------------------------------------------------
static void rat_thermocouple_sensor_enable (rat_thermocouple_selection_type selection)
{
  if (selection == RAT_THERMOCOUPLE_SENSOR_LFT) {
    RAT_THERMOCOUPLE_SENSOR_LFT_PWR_PIN = 0b1;
    RAT_THERMOCOUPLE_SENSOR_LFT_CSL_PIN = 0b1;
  } else {
    RAT_THERMOCOUPLE_SENSOR_RGT_PWR_PIN = 0b1;
    RAT_THERMOCOUPLE_SENSOR_RGT_CSL_PIN = 0b1;
  }

  rat_delay(RAT_THERMOCOUPLE_SENSOR_STABILIZATION_DELAY);
}

// -----------------------------------------------------------------------------
// Disable
// -----------------------------------------------------------------------------
static void rat_thermocouple_sensor_disable (rat_thermocouple_selection_type selection)
{
  if (selection == RAT_THERMOCOUPLE_SENSOR_LFT) {
    RAT_THERMOCOUPLE_SENSOR_LFT_PWR_PIN = 0b0;
    RAT_THERMOCOUPLE_SENSOR_LFT_CSL_PIN = 0b0;
  } else {
    RAT_THERMOCOUPLE_SENSOR_RGT_PWR_PIN = 0b0;
    RAT_THERMOCOUPLE_SENSOR_RGT_CSL_PIN = 0b0;
  }
}

// -----------------------------------------------------------------------------
// Convert the temperature
//
// All the temperatures which are below the minimum or
// above the maximum rating will be cut.
// -----------------------------------------------------------------------------
static float rat_convert_temperature (uint32_t temperature,
                                      rat_thermocouple_data_type data_type)
{
  // ---------------------------------------------------------------------------
  // Auxiliary variables
  // ---------------------------------------------------------------------------
  uint8_t index   = 0;
  uint8_t length  = 0;
  uint8_t counter = 0;
  
  uint32_t value  = 0;

  float result = 0.0;

  // ---------------------------------------------------------------------------
  // Determine the start index and the length of the value
  // --------------------------------------------------------------------------------------------------
  if (data_type == RAT_THERMOCOUPLE_SENSOR_THERMOCOUPLE_TEMPERATURE_DATA) {
    index = -2;
    length = 13;
  } else {
    index = -4;
    length = 11;
  }
  
  // ---------------------------------------------------------------------------
  // Convert the temperature
  // ---------------------------------------------------------------------------
  value = temperature;
  
  result = 0;
  
  for (counter = 0;counter < length;counter++) {
    if (value % 2 == 1) {
      result += pow(2, index);
    }
    
    value = value >> 1;
  
    index += 1;
  }
  
  // ---------------------------------------------------------------------------
  // Check the sign bit
  // ---------------------------------------------------------------------------
  if (value % 2 == 1) {
    result = -result;
  }

  // ---------------------------------------------------------------------------
  // Check that the result is between the minimum and the maximum
  // ---------------------------------------------------------------------------
  if (result < RAT_THERMOCOUPLE_SENSOR_TEMPERATURE_MINIMUM) {
    result = RAT_THERMOCOUPLE_SENSOR_TEMPERATURE_MINIMUM;
  } else if (result > RAT_THERMOCOUPLE_SENSOR_TEMPERATURE_MAXIMUM) {
    result = RAT_THERMOCOUPLE_SENSOR_TEMPERATURE_MAXIMUM;
  }

  // ---------------------------------------------------------------------------
  // Return
  // ---------------------------------------------------------------------------
  return result;
}

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Init
// -----------------------------------------------------------------------------
void rat_thermocouple_sensor_init (void)
{
  rat_thermocouple_sensor_set_pins();
  rat_thermocouple_sensor_enable(RAT_THERMOCOUPLE_SENSOR_LFT);
  rat_thermocouple_sensor_enable(RAT_THERMOCOUPLE_SENSOR_RGT);
  rat_thermocouple_sensor_reset();
}

// -----------------------------------------------------------------------------
// Measure the temperature
// -----------------------------------------------------------------------------
void rat_thermocouple_sensor_measure (float   * thermocouple_temperature,
                                      float   * internal_temperature,
                                      uint8_t * fault_flag,
                                      uint8_t * short_vcc_flag,
                                      uint8_t * short_gnd_flag,
                                      uint8_t * open_circuit_flag)
{
  // ---------------------------------------------------------------------------
  // Auxiliary variables
  // ---------------------------------------------------------------------------
  uint8_t counter = 0;
  
  uint8_t dummy_buffer = 0x00;

  uint8_t response [4] = {0x00,0x00,0x00,0x00};

  uint32_t raw_thermocouple_temperature_data = 0x00000000;
  uint32_t raw_internal_temperature_data      = 0x00000000;

  // ---------------------------------------------------------------------------
  // Enable the sensor
  // ---------------------------------------------------------------------------
  rat_thermocouple_sensor_enable(RAT_THERMOCOUPLE_SENSOR_LFT);
  
  // ---------------------------------------------------------------------------
  // Read the raw data
  // ---------------------------------------------------------------------------
  RAT_THERMOCOUPLE_SENSOR_LFT_CSL_PIN = 0b0;
   
  rat_delay(RAT_THERMOCOUPLE_SENSOR_CHIP_SELECT_DELAY);
  
  for (counter = 0;counter < 4;counter++) {
    response[n] = SPI_Read(dummy_buffer);
  }
  
  rat_delay(RAT_THERMOCOUPLE_SENSOR_CHIP_SELECT_DELAY);

  // ---------------------------------------------------------------------------
  // Disable the sensor
  // ---------------------------------------------------------------------------
  rat_thermocouple_sensor_disable(RAT_THERMOCOUPLE_SENSOR_LFT);

  // ---------------------------------------------------------------------------
  // Parse the raw data
  // ---------------------------------------------------------------------------
  
  // ---------------------------------------------------------------------------
  // Thermocouple temperature data
  // ---------------------------------------------------------------------------
  raw_thermocouple_temperature_data = response[0];
  
  raw_thermocouple_temperature_data = raw_thermocouple_temperature_data << 6;
  
  raw_thermocouple_temperature_data += ( response[1] >> 2 );
  
  // ---------------------------------------------------------------------------
  // Fault flag
  // ---------------------------------------------------------------------------
  *fault_flag = response[1] % 2;
  
  // --------------------------------------------------------------------------------------------------
  // Internal temperature data
  // --------------------------------------------------------------------------------------------------
  raw_internal_temperature_data = response[2];

  raw_internal_temperature_data = raw_internal_temperature_data << 5;

  raw_internal_temperature_data += ( response[3] >> 3 );
  
  // ---------------------------------------------------------------------------
  // Short to VCC flag
  // ---------------------------------------------------------------------------
  *short_vcc_flag = ( response[3] >> 2 ) % 2;
  
  // ---------------------------------------------------------------------------
  // Short to GND flag
  // ---------------------------------------------------------------------------
  *short_gnd_flag = ( response[3] >> 1 ) % 2;
  
  // ---------------------------------------------------------------------------
  // Open circuit flag
  // ---------------------------------------------------------------------------
  *open_circuit_flag = response[3] % 2;

  // ---------------------------------------------------------------------------
  // Convert
  // ---------------------------------------------------------------------------
  *thermocouple_temperature = rat_convert_temperature(raw_thermocouple_temperature_data,
                                                      RAT_THERMOCOUPLE_SENSOR_THERMOCOUPLE_TEMPERATURE_DATA);
  *internal_temperature     = rat_convert_temperature(raw_internal_temperature_data,
                                                      RAT_THERMOCOUPLE_SENSOR_INTERNAL_TEMPERATURE_DATA);
}