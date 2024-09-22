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
// Texas Instruments TMP117 Source File
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------------------------------
#include <stdint.h>

#include "../headers/rat_sensors.h"

#ifdef TEXAS_INSTRUMENTS_TMP117

#include "../../rat-utils/headers/rat_mcu_utilities.h"
#include "../../rat-utils/headers/rat_i2c_utilities.h"
#include "../../rat-utils/headers/rat_debug_utilities.h"

#include "../headers/rat_texas_instruments_tmp117.h"

// ----------------------------------------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Set the pins
// ----------------------------------------------------------------------------------------------------
static void rat_temperature_sensor_set_pins (void)
{
  // --------------------------------------------------------------------------------------------------
  // Power
  // --------------------------------------------------------------------------------------------------
  RAT_TEMPERATURE_SENSOR_PWR_DIR = 0b0;
  RAT_TEMPERATURE_SENSOR_PWR_TYP = 0b0;
}

// ----------------------------------------------------------------------------------------------------
// Reset
//
// Since the sensor does NOT have a reset pin,
// the hardware reset is implemented with a power pin.
// ----------------------------------------------------------------------------------------------------
static void rat_temperature_sensor_reset (void)
{
  RAT_TEMPERATURE_SENSOR_PWR_PIN = 0b0;

  rat_delay_milliseconds(RAT_TEMPERATURE_SENSOR_RESET_DELAY);

  RAT_TEMPERATURE_SENSOR_PWR_PIN = 0b1;
}

// ----------------------------------------------------------------------------------------------------
// Enable
// ----------------------------------------------------------------------------------------------------
static void rat_temperature_sensor_enable (void)
{
  RAT_TEMPERATURE_SENSOR_PWR_PIN = 0b1;

  rat_delay_milliseconds(RAT_TEMPERATURE_SENSOR_STABILIZATION_DELAY);
}

// ----------------------------------------------------------------------------------------------------
// Reset
// ----------------------------------------------------------------------------------------------------
static void rat_temperature_sensor_disable (void)
{
  RAT_TEMPERATURE_SENSOR_PWR_PIN = 0b0;
}

// ----------------------------------------------------------------------------------------------------
// Write the configuration
//
// Measurement mode [11:10]
//
//   0 - Continuous conversion
//   1 - Shutdown
//   3 - One shot
//
// Averaging [6:5]
//
//   0 - Disabled
//   1 -  8 conversions
//   2 - 32 conversions
//   3 - 64 conversions
// ----------------------------------------------------------------------------------------------------
static void rat_temperature_sensor_configuration_write (uint8_t measurement_mode,
                                                        uint8_t averaging)
{
  // --------------------------------------------------------------------------------------------------
  // Request
  // --------------------------------------------------------------------------------------------------
  uint8_t request [3];
  
  request[0] = RAT_TEMPERATURE_SENSOR_CONFIGURATION_ADDRESS;

  request[1] = measurement_mode;
  request[1] = request[1] << 2;

  request[2] = averaging;
  request[2] = request[2] << 5;
  
  rat_i2c_write_stream(I2C_ADDRESS_DEFINED,
                       RAT_TEMPERATURE_SENSOR_ADDRESS,
                       3,
                       request,
                       I2C_STOP_BIT_APPLIED);
}

// ----------------------------------------------------------------------------------------------------
// Read the configuration
//
// Measurement mode [11:10]
//
//   0 - Continuous conversion
//   1 - Shutdown
//   3 - One shot
//
// Averaging [6:5]
//
//   0 - Disabled
//   1 -  8 conversions
//   2 - 32 conversions
//   3 - 64 conversions
// ----------------------------------------------------------------------------------------------------
static void rat_temperature_sensor_configuration_read (uint8_t * measurement_mode,
                                                       uint8_t * averaging)
{
  // --------------------------------------------------------------------------------------------------
  // Request and response
  // --------------------------------------------------------------------------------------------------
  uint8_t request [1];
  uint8_t response [2];

  // --------------------------------------------------------------------------------------------------
  // Request
  // --------------------------------------------------------------------------------------------------
  request[0] = RAT_TEMPERATURE_SENSOR_CONFIGURATION_ADDRESS;

  rat_i2c_write_stream(I2C_ADDRESS_DEFINED,
                       RAT_TEMPERATURE_SENSOR_ADDRESS,
                       1,
                       request,
                       I2C_STOP_BIT_NOT_APPLIED);

  // --------------------------------------------------------------------------------------------------
  // Response
  // --------------------------------------------------------------------------------------------------
  rat_i2c_read_stream(I2C_ADDRESS_DEFINED,
                      RAT_TEMPERATURE_SENSOR_ADDRESS,
                      2,
                      response,
                      I2C_STOP_BIT_APPLIED);

  // --------------------------------------------------------------------------------------------------
  // Assign the configuration
  // --------------------------------------------------------------------------------------------------
  *measurement_mode = ( response[0] >> 2 ) % 4;
  *averaging        = ( response[1] >> 5 ) % 4;
}

// ----------------------------------------------------------------------------------------------------
// Read the temperature
// ----------------------------------------------------------------------------------------------------
static void rat_temperature_sensor_read_temperature (uint16_t * temperature)
{
  // --------------------------------------------------------------------------------------------------
  // Request and response
  // --------------------------------------------------------------------------------------------------
  uint8_t request [1];
  uint8_t response [2];

  // --------------------------------------------------------------------------------------------------
  // Request
  // --------------------------------------------------------------------------------------------------
  request[0] = RAT_TEMPERATURE_SENSOR_TEMPERATURE_ADDRESS;

  rat_i2c_write_stream(I2C_ADDRESS_DEFINED,
                       RAT_TEMPERATURE_SENSOR_ADDRESS,
                       1,
                       request,
                       I2C_STOP_BIT_NOT_APPLIED);

  // --------------------------------------------------------------------------------------------------
  // Response
  // --------------------------------------------------------------------------------------------------
  rat_i2c_read_stream(I2C_ADDRESS_DEFINED,
                      RAT_TEMPERATURE_SENSOR_ADDRESS,
                      2,
                      response,
                      I2C_STOP_BIT_APPLIED);

  // --------------------------------------------------------------------------------------------------
  // Assign the temperature
  // --------------------------------------------------------------------------------------------------
  *temperature = ( response[0] << 8 ) +
                   response[1];
}

// ----------------------------------------------------------------------------------------------------
// Convert the temperature
//
// All the temperatures which are below the minimum or above the maximum rating will be cut.
// ----------------------------------------------------------------------------------------------------
static float rat_temperature_sensor_convert_temperature (uint16_t temperature)
{
  const float step = 0.0078125;
  
  float result;

  result = ( (float) ( temperature & 0x7FFF ) ) * step;
  
  if (( temperature & 0x8000 ) == 0x8000) {
    result -= ( (float) pow(2,15) ) * step;
  }

  if (result < RAT_TEMPERATURE_SENSOR_TEMPERATURE_MINIMUM) {
    result = RAT_TEMPERATURE_SENSOR_TEMPERATURE_MINIMUM;
  } else if (result > RAT_TEMPERATURE_SENSOR_TEMPERATURE_MAXIMUM) {
    result = RAT_TEMPERATURE_SENSOR_TEMPERATURE_MAXIMUM;
  }

  return result;
}

// ----------------------------------------------------------------------------------------------------
// Functions
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Init
// ----------------------------------------------------------------------------------------------------
void rat_temperature_sensor_init (void)
{

  rat_temperature_sensor_set_pins();
  rat_temperature_sensor_enable();
  rat_temperature_sensor_reset();
}

// ----------------------------------------------------------------------------------------------------
// Measure
//
// Returns true (1) if the checksum matches; false (0) otherwise.
//
// Since the sensor does NOT support checksum, the function always returns true.
// ----------------------------------------------------------------------------------------------------
uint8_t rat_temperature_sensor_measure (float * temperature)
{
  // --------------------------------------------------------------------------------------------------
  // Raw temperature
  // --------------------------------------------------------------------------------------------------
  uint16_t raw_temperature;

  // --------------------------------------------------------------------------------------------------
  // Request
  // --------------------------------------------------------------------------------------------------
  rat_temperature_sensor_configuration_write(RAT_TEMPERATURE_SENSOR_MEASUREMENT_MODE_ONE_SHOT,
                                             RAT_TEMPERATURE_SENSOR_AVERAGING_8);

  // --------------------------------------------------------------------------------------------------
  // Delay
  // --------------------------------------------------------------------------------------------------
  rat_delay_milliseconds(RAT_TEMPERATURE_SENSOR_RESPONSE_DELAY);

  // --------------------------------------------------------------------------------------------------
  // Response
  // --------------------------------------------------------------------------------------------------
  rat_temperature_sensor_read_temperature(&raw_temperature);

  // --------------------------------------------------------------------------------------------------
  // Convert
  // --------------------------------------------------------------------------------------------------
  *temperature = rat_temperature_sensor_convert_temperature(raw_temperature);

  // --------------------------------------------------------------------------------------------------
  // Since the sensor does not support checksum, the function always returns true.
  // --------------------------------------------------------------------------------------------------
  return 1;
}

#endif