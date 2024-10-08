// -----------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2021 - 2024 Rapiot Open Hardware Project
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Sensirion SHT4X Source File
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

#include "../../rat_sensors/headers/rat_sensirion_sht4x.h"

// -----------------------------------------------------------------------------
// Static functions
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Set the pins
// -----------------------------------------------------------------------------
static void rat_humidity_sensor_set_pins (void)
{
  // ---------------------------------------------------------------------------
  // Power
  // ---------------------------------------------------------------------------
  RAT_HUMIDITY_SENSOR_PWR_DIR = 0b0;
  RAT_HUMIDITY_SENSOR_PWR_TYP = 0b0;

  RAT_HUMIDITY_SENSOR_PWR_PIN = 0b0;
}

// -----------------------------------------------------------------------------
// Reset
// -----------------------------------------------------------------------------
static void rat_humidity_sensor_reset (void)
{
  RAT_HUMIDITY_SENSOR_PWR_PIN = 0b0;

  rat_delay(RAT_HUMIDITY_SENSOR_RESET_DELAY);

  RAT_HUMIDITY_SENSOR_PWR_PIN = 0b1;
}

// -----------------------------------------------------------------------------
// Enable
// -----------------------------------------------------------------------------
static void rat_humidity_sensor_enable (void)
{
  RAT_HUMIDITY_SENSOR_PWR_PIN = 0b1;

  rat_delay(RAT_HUMIDITY_SENSOR_STABILIZATION_DELAY);
}

// -----------------------------------------------------------------------------
// Disable
// -----------------------------------------------------------------------------
static void rat_humidity_sensor_disable (void)
{
  RAT_HUMIDITY_SENSOR_PWR_PIN = 0b0;
}

// -----------------------------------------------------------------------------
// Convert the temperature
//
// All the temperatures which are below the minimum or
// above the maximum rating will be cut.
// -----------------------------------------------------------------------------
static float rat_humidity_sensor_convert_temperature (uint16_t temperature)
{
  float result = 0.0;

  result = ( (float) temperature ) / 65535.0;
  result = 175.0 * result;
  result = result - 45.0;

  if (result < RAT_HUMIDITY_SENSOR_TEMPERATURE_MINIMUM) {
    result = RAT_HUMIDITY_SENSOR_TEMPERATURE_MINIMUM;
  } else if (result > RAT_HUMIDITY_SENSOR_TEMPERATURE_MAXIMUM) {
    result = RAT_HUMIDITY_SENSOR_TEMPERATURE_MAXIMUM;
  }

  return result;
}

// -----------------------------------------------------------------------------
// Convert the humidity
//
// All the humidities which are below the minimum or
// above the maximum rating will be cut.
// -----------------------------------------------------------------------------
static float rat_humidity_sensor_convert_humidity (uint16_t humidity)
{
  float result = 0.0;

  result = ( (float) humidity ) / 65535.0;
  result = 100.0 * result;

  if (result < RAT_HUMIDITY_SENSOR_HUMIDITY_MINIMUM) {
    result = RAT_HUMIDITY_SENSOR_HUMIDITY_MINIMUM;
  } else if (result > RAT_HUMIDITY_SENSOR_HUMIDITY_MAXIMUM) {
    result = RAT_HUMIDITY_SENSOR_HUMIDITY_MAXIMUM;
  }

  return result;
}

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Init
// -----------------------------------------------------------------------------
void rat_humidity_sensor_init (void)
{
  rat_humidity_sensor_set_pins();
  rat_humidity_sensor_enable();
  rat_humidity_sensor_reset();
}

// -----------------------------------------------------------------------------
// Software reset
// -----------------------------------------------------------------------------
void rat_humidity_sensor_software_reset (void)
{
  // ---------------------------------------------------------------------------
  // Request
  // ---------------------------------------------------------------------------
  uint8_t request [2] = {0x00};

  request[0] = RAT_HUMIDITY_SENSOR_SOFT_RESET >> 8;
  request[1] = RAT_HUMIDITY_SENSOR_SOFT_RESET % 256;

  rat_i2c_write_stream(I2C_ADDRESS_DEFINED,
                       RAT_HUMIDITY_SENSOR_ADDRESS,
                       2,
                       request,
                       I2C_STOP_BIT_APPLIED);
}

// -----------------------------------------------------------------------------
// Read serial
//
// Returns true (1) if the checksums match; false (0) otherwise.
// -----------------------------------------------------------------------------
uint8_t rat_humidity_sensor_read_serial (uint8_t * serial)
{
  // ---------------------------------------------------------------------------
  // Request and response
  // ---------------------------------------------------------------------------
  uint8_t request  [1] = {0x00};
  uint8_t response [6] = {0x00};

  // ---------------------------------------------------------------------------
  // Checksums
  // ---------------------------------------------------------------------------
  uint16_t serial_high = 0x0000;
  uint16_t serial_low  = 0x0000;
  
  uint8_t crc [2] = {0x00};

  // ---------------------------------------------------------------------------
  // Request
  // ---------------------------------------------------------------------------
  request[0] = RAT_HUMIDITY_SENSOR_READ_SERIAL;

  rat_i2c_write_stream(I2C_ADDRESS_DEFINED,
                       RAT_HUMIDITY_SENSOR_ADDRESS,
                       1,
                       request,
                       I2C_STOP_BIT_APPLIED);

  // ---------------------------------------------------------------------------
  // Response
  // ---------------------------------------------------------------------------
  rat_i2c_read_stream(I2C_ADDRESS_DEFINED,
                      RAT_HUMIDITY_SENSOR_ADDRESS,
                      6,
                      response,
                      I2C_STOP_BIT_APPLIED);

  // ---------------------------------------------------------------------------
  // Serial
  // ---------------------------------------------------------------------------
  serial_high = ( response[0] << 8 ) +
                  response[1];

  serial_low  = ( response[3] << 8 ) +
                  response[4];
                  
  serial[0] = response[0];
  serial[1] = response[1];
  serial[2] = response[3];
  serial[3] = response[4];

  // ---------------------------------------------------------------------------
  // Check the checksums
  // ---------------------------------------------------------------------------
  crc[0] = rat_calculate_crc(serial_high,
                             RAT_HUMIDITY_SENSOR_INITIALIZATION,
                             RAT_HUMIDITY_SENSOR_POLYNOMIAL);
  crc[1] = rat_calculate_crc(serial_low,
                             RAT_HUMIDITY_SENSOR_INITIALIZATION,
                             RAT_HUMIDITY_SENSOR_POLYNOMIAL);

  // ---------------------------------------------------------------------------
  // Return
  // ---------------------------------------------------------------------------
  if ((crc[0] == response[2]) &&
      (crc[1] == response[5])) {
    return 1;
  } else {
    return 0;
  }
}

// -----------------------------------------------------------------------------
// Measure the temperature and the humidity
//
// Returns true if the checksums match; false otherwise.
// -----------------------------------------------------------------------------
bool rat_humidity_sensor_measure (float * temperature,
                                  float * humidity)
{
  // ---------------------------------------------------------------------------
  // Request and response
  // ---------------------------------------------------------------------------
  uint8_t request  [1] = {0x00};
  uint8_t response [6] = {0x00};

  // ---------------------------------------------------------------------------
  // Raw temperature and humidity
  // ---------------------------------------------------------------------------
  uint16_t raw_temperature = 0x0000;
  uint16_t raw_humidity    = 0x0000;

  // ---------------------------------------------------------------------------
  // Checksums
  // ---------------------------------------------------------------------------
  uint8_t crc [2] = {0x00};

  // ---------------------------------------------------------------------------
  // Request
  // ---------------------------------------------------------------------------
  request[0] = RAT_HUMIDITY_SENSOR_MEASURE_REPEATABILITY_HGH;

  rat_i2c_write_stream(I2C_ADDRESS_DEFINED,
                       RAT_HUMIDITY_SENSOR_ADDRESS,
                       1,
                       request,
                       I2C_STOP_BIT_APPLIED);

  // ---------------------------------------------------------------------------
  // Note! SHT4X does NOT support clock stretching!
  //
  //       SHT4X sends NACK response if data is not ready.
  // ---------------------------------------------------------------------------
  rat_delay(SHT4X_MEASUREMENT_DELAY);

  // ---------------------------------------------------------------------------
  // Response
  // ---------------------------------------------------------------------------
  rat_i2c_read_stream(I2C_ADDRESS_DEFINED,
                      RAT_HUMIDITY_SENSOR_ADDRESS,
                      6,
                      response,
                      I2C_STOP_BIT_APPLIED);

  // ---------------------------------------------------------------------------
  // Result
  // ---------------------------------------------------------------------------
  raw_temperature = ( response[0] << 8 ) +
                      response[1];

  raw_humidity    = ( response[3] << 8 ) +
                      response[4];

  // ---------------------------------------------------------------------------
  // Check the checksums
  // ---------------------------------------------------------------------------
  crc[0] = rat_calculate_crc(raw_temperature,
                             RAT_HUMIDITY_SENSOR_INITIALIZATION,
                             RAT_HUMIDITY_SENSOR_POLYNOMIAL);
  crc[1] = rat_calculate_crc(raw_humidity,
                             RAT_HUMIDITY_SENSOR_INITIALIZATION,
                             RAT_HUMIDITY_SENSOR_POLYNOMIAL);

  // ---------------------------------------------------------------------------
  // Convert
  // ---------------------------------------------------------------------------
  *temperature = rat_humidity_sensor_convert_temperature(raw_temperature);
  *humidity    = rat_humidity_sensor_convert_humidity(raw_humidity);

  // ---------------------------------------------------------------------------
  // Return
  // ---------------------------------------------------------------------------
  if ((crc[0] == response[2]) &&
      (crc[1] == response[5])) {
    return true;
  } else {
    return false;
  }
}