// ----------------------------------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2020 - 2022 Rapiot Open Hardware Project
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
// Texas Instruments HDC2080 Source File
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------------------------------
#include <stdint.h>

#include "../../rat_application/headers/defines.h"

// ----------------------------------------------------------------------------------------------------
// Conditional compilation
// ----------------------------------------------------------------------------------------------------
#ifdef TEXAS_INSTRUMENTS_HDC2080

#include "../../rat_utilities/headers/rat_i2c_utilities.h"
#include "../../rat_utilities/headers/rat_mcu_utilities.h"

#ifdef TEST_MODE
  #include "../../rat_utilities/headers/rat_debug_utilities.h"
#endif

#include "../headers/rat_texas_instruments_hdc2080.h"

// ----------------------------------------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Set the pins
// ----------------------------------------------------------------------------------------------------
static void rat_humidity_sensor_set_pins (void)
{
  // --------------------------------------------------------------------------------------------------
  // Power
  // --------------------------------------------------------------------------------------------------
  RAT_HUMIDITY_SENSOR_PWR_DIR = 0b0;
  RAT_HUMIDITY_SENSOR_PWR_TYP = 0b0;
  
  // --------------------------------------------------------------------------------------------------
  // Interrupt
  // --------------------------------------------------------------------------------------------------
  RAT_HUMIDITY_SENSOR_INT_DIR = 0b1;
  RAT_HUMIDITY_SENSOR_INT_TYP = 0b0;
}

// ----------------------------------------------------------------------------------------------------
// Reset
// ----------------------------------------------------------------------------------------------------
static void rat_humidity_sensor_reset (void)
{
  RAT_HUMIDITY_SENSOR_PWR_PIN = 0b0;
  
  rat_delay_milliseconds(RAT_HUMIDITY_SENSOR_RESET_DELAY);
  
  RAT_HUMIDITY_SENSOR_PWR_PIN = 0b1;
}

// ----------------------------------------------------------------------------------------------------
// Enable
// ----------------------------------------------------------------------------------------------------
static void rat_humidity_sensor_enable (void)
{
  RAT_HUMIDITY_SENSOR_PWR_PIN = 0b1;
  
  rat_delay_milliseconds(RAT_HUMIDITY_SENSOR_STABILIZATION_DELAY);
}

// ----------------------------------------------------------------------------------------------------
// Disable
// ----------------------------------------------------------------------------------------------------
static void rat_humidity_sensor_disable (void)
{
  RAT_HUMIDITY_SENSOR_PWR_PIN = 0b0;
}

// ----------------------------------------------------------------------------------------------------
// Write the measurement configuration
// ----------------------------------------------------------------------------------------------------
static void rat_humidity_sensor_write_measurement_configuration (uint8_t temperature_resolution,
                                                                 uint8_t humidity_resolution,
                                                                 uint8_t measurement_configuration,
                                                                 uint8_t measurement_trigger)
{
  // --------------------------------------------------------------------------------------------------
  // Request
  // --------------------------------------------------------------------------------------------------
  uint8_t request [2];
  
  request[0]  = RAT_HUMIDITY_SENSOR_MEASUREMENT_CONFIGURATION_ADDRESS;
  
  request[1]  = temperature_resolution;
  request[1]  = request[0] << 2;
  
  request[1] += humidity_resolution;
  request[1]  = request[0] << 3;
  
  request[1] += measurement_configuration;
  request[1]  = request[0] << 1;
  
  request[1] += measurement_trigger;
  
  rat_i2c_write_stream(I2C_ADDRESS_DEFINED,
                       RAT_HUMIDITY_SENSOR_ADDRESS,
                       2,
                       request,
                       I2C_STOP_BIT_APPLIED);
}

// ----------------------------------------------------------------------------------------------------
// Read the measurement configuration
// ----------------------------------------------------------------------------------------------------
static void rat_humidity_sensor_read_measurement_configuration (uint8_t * temperature_resolution,
                                                                uint8_t * humidity_resolution,
                                                                uint8_t * measurement_configuration,
                                                                uint8_t * measurement_trigger)
{
  // --------------------------------------------------------------------------------------------------
  // Request and response
  // --------------------------------------------------------------------------------------------------
  uint8_t request [1];
  
  uint8_t response [1];
  
  // --------------------------------------------------------------------------------------------------
  // Request
  // --------------------------------------------------------------------------------------------------
  request[0] = RAT_HUMIDITY_SENSOR_MEASUREMENT_CONFIGURATION_ADDRESS;
  
  rat_i2c_write_stream(I2C_ADDRESS_DEFINED,
                       RAT_HUMIDITY_SENSOR_ADDRESS,
                       1,
                       request,
                       I2C_STOP_BIT_NOT_APPLIED);

  // --------------------------------------------------------------------------------------------------
  // Response
  // --------------------------------------------------------------------------------------------------
  rat_i2c_read_stream(I2C_ADDRESS_DEFINED,
                      RAT_HUMIDITY_SENSOR_ADDRESS,
                      1,
                      response,
                      I2C_STOP_BIT_APPLIED);

  // --------------------------------------------------------------------------------------------------
  // Result
  // --------------------------------------------------------------------------------------------------
  *temperature_resolution    = ( response[0] >> 6 ) % 4;
  *humidity_resolution       = ( response[0] >> 4 ) % 4;
  *measurement_configuration = ( response[0] >> 1 ) % 4;
  *measurement_trigger       = ( response[0]      ) % 2;
}

// ----------------------------------------------------------------------------------------------------
// Read the temperature
// ----------------------------------------------------------------------------------------------------
static void rat_humidity_sensor_read_temperature (uint16_t * temperature)
{
  // --------------------------------------------------------------------------------------------------
  // Request and response
  // --------------------------------------------------------------------------------------------------
  uint8_t request [1];
  
  uint8_t response [2];

  // --------------------------------------------------------------------------------------------------
  // Request
  // --------------------------------------------------------------------------------------------------
  request[0] = RAT_HUMIDITY_SENSOR_TEMPERATURE_ADDRESS;
  
  rat_i2c_write_stream(I2C_ADDRESS_DEFINED,
                       RAT_HUMIDITY_SENSOR_ADDRESS,
                       1,
                       request,
                       I2C_STOP_BIT_NOT_APPLIED);

  // --------------------------------------------------------------------------------------------------
  // Response
  // --------------------------------------------------------------------------------------------------
  rat_i2c_read_stream(I2C_ADDRESS_DEFINED,
                      RAT_HUMIDITY_SENSOR_ADDRESS,
                      2,
                      response,
                      I2C_STOP_BIT_APPLIED);
  
  // --------------------------------------------------------------------------------------------------
  // Result
  // --------------------------------------------------------------------------------------------------
  temperature = ( response[1] << 8 ) + 
                  response[0];
}

// ----------------------------------------------------------------------------------------------------
// Read the humidity
// ----------------------------------------------------------------------------------------------------
static void rat_humidity_sensor_read_humidity (uint16_t * humidity)
{
  // --------------------------------------------------------------------------------------------------
  // Request and response
  // --------------------------------------------------------------------------------------------------
  uint8_t request [1];
  
  uint8_t response [2];

  // --------------------------------------------------------------------------------------------------
  // Request
  // --------------------------------------------------------------------------------------------------
  request[0] = RAT_HUMIDITY_SENSOR_HUMIDITY_ADDRESS;

  rat_i2c_write_stream(I2C_ADDRESS_DEFINED,
                       RAT_HUMIDITY_SENSOR_ADDRESS,
                       1,
                       request,
                       I2C_STOP_BIT_NOT_APPLIED);

  // --------------------------------------------------------------------------------------------------
  // Response
  // --------------------------------------------------------------------------------------------------
  rat_i2c_read_stream(I2C_ADDRESS_DEFINED,
                      RAT_HUMIDITY_SENSOR_ADDRESS,
                      2,
                      response,
                      I2C_STOP_BIT_APPLIED);

  // --------------------------------------------------------------------------------------------------
  // Result
  // --------------------------------------------------------------------------------------------------
  humidity = ( response[1] << 8 ) +
               response[0];
}

// ----------------------------------------------------------------------------------------------------
// Convert the temperature
// ----------------------------------------------------------------------------------------------------
static float rat_humidity_sensor_convert_temperature (uint16_t temperature)
{
  float result;
  
  result = ( ( (float) temperature ) * 165 ) / 65536 - 40;
  
  if (result < RAT_HUMIDITY_SENSOR_TEMPERATURE_MINIMUM) {
    result = RAT_HUMIDITY_SENSOR_TEMPERATURE_MINIMUM;
  } else if (result > RAT_HUMIDITY_SENSOR_TEMPERATURE_MAXIMUM) {
    result = RAT_HUMIDITY_SENSOR_TEMPERATURE_MAXIMUM;
  }
  
  return result;
}

// ----------------------------------------------------------------------------------------------------
// Convert the humidity
// ----------------------------------------------------------------------------------------------------
static float rat_humidity_sensor_convert_humidity (uint16_t humidity)
{
  float result;
  
  result = ( ( (float) humidity ) * 100 ) / 65536;

  if (result < RAT_HUMIDITY_SENSOR_HUMIDITY_MINIMUM) {
    result = RAT_HUMIDITY_SENSOR_HUMIDITY_MINIMUM;
  } else if (result > RAT_HUMIDITY_SENSOR_HUMIDITY_MAXIMUM) {
    result = RAT_HUMIDITY_SENSOR_HUMIDITY_MAXIMUM;
  }
  
  return result;
}

// ----------------------------------------------------------------------------------------------------
// Functions
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Init
// ----------------------------------------------------------------------------------------------------
void rat_humidity_sensor_init (void)
{
  rat_humidity_sensor_set_pins();
  rat_humidity_sensor_enable();
  rat_humidity_sensor_reset();
}

// ----------------------------------------------------------------------------------------------------
// Measure
//
// Returns true (1) if the checksum matches; false (0) otherwise.
//
// Since the sensor does NOT support checksum, the function always returns true.
// ----------------------------------------------------------------------------------------------------
uint8_t rat_humidity_sensor_measure (float * temperature,
                                     float * humidity)
{
  // --------------------------------------------------------------------------------------------------
  // Request and response
  // --------------------------------------------------------------------------------------------------
  uint8_t request [1];
  uint8_t response [4];

  // --------------------------------------------------------------------------------------------------
  // Raw temperature and humidity
  // --------------------------------------------------------------------------------------------------
  uint16_t raw_temperature;
  uint16_t raw_humidity;

  // --------------------------------------------------------------------------------------------------
  // Configuration
  // --------------------------------------------------------------------------------------------------
  rat_humidity_sensor_write_measurement_configuration(RAT_HUMIDITY_SENSOR_TEMPERATURE_RESOLUTION_14_BIT,
                                                      RAT_HUMIDITY_SENSOR_HUMIDITY_RESOLUTION_14_BIT,
                                                      RAT_HUMIDITY_SENSOR_MEASUREMENT_CONFIGURATION_T_H,
                                                      RAT_HUMIDITY_SENSOR_MEASUREMENT_TRIGGER_ENABLED);

  // --------------------------------------------------------------------------------------------------
  // Request
  // --------------------------------------------------------------------------------------------------
  request[0] = RAT_HUMIDITY_SENSOR_TEMPERATURE_ADDRESS;

  rat_i2c_write_stream(I2C_ADDRESS_DEFINED,
                       RAT_HUMIDITY_SENSOR_ADDRESS,
                       1,
                       request,
                       I2C_STOP_BIT_NOT_APPLIED);

  // --------------------------------------------------------------------------------------------------
  // Delay
  // --------------------------------------------------------------------------------------------------
  rat_delay_milliseconds(RAT_HUMIDITY_SENSOR_RESPONSE_DELAY);

  // --------------------------------------------------------------------------------------------------
  // Response
  // --------------------------------------------------------------------------------------------------
  rat_i2c_read_stream(I2C_ADDRESS_DEFINED,
                      RAT_HUMIDITY_SENSOR_ADDRESS,
                      4,
                      response,
                      I2C_STOP_BIT_APPLIED);

  // --------------------------------------------------------------------------------------------------
  // Assign the temperature and the humidity
  // --------------------------------------------------------------------------------------------------
  raw_temperature = ( response[1] << 8 ) +
                      response[0];

  raw_humidity    = ( response[3] << 8 ) +
                      response[2];

  // --------------------------------------------------------------------------------------------------
  // Convert
  // --------------------------------------------------------------------------------------------------
  *temperature = rat_humidity_sensor_convert_temperature(raw_temperature);
  *humidity    = rat_humidity_sensor_convert_humidity(raw_humidity);

  // --------------------------------------------------------------------------------------------------
  // Since the sensor does NOT support checksum, the function always returns true.
  // --------------------------------------------------------------------------------------------------
  return 1;
}

#endif