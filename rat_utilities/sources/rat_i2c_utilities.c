// -----------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2020 - 2024 Rapiot Open Hardware Project
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// I2C Utilities Source File
//
// The purpose of the utilities is to provide
// a high level layer for the I2C functions of mikroC.
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../../rat_utilities/headers/rat_i2c_utilities.h"

// -----------------------------------------------------------------------------
// Write
//
//   mode    - Defines if the address has been defined.
//   address - The address of the device without the R/W bit.
//   length  - The length of the payload.
//   payload - The payload.
//   end     - Defines if the stop bit has been applied.
// -----------------------------------------------------------------------------
void rat_i2c_write_stream (uint8_t   mode,
                           uint8_t   address,
                           uint8_t   length,
                           uint8_t * payload,
                           uint8_t   end)
{
  // ---------------------------------------------------------------------------
  // Auxiliary variables
  // ---------------------------------------------------------------------------
  uint8_t byte_index = 0;

  // ---------------------------------------------------------------------------
  // Start condition
  // ---------------------------------------------------------------------------
  I2C1_Start();

  // ---------------------------------------------------------------------------
  // Write the address (if applicable)
  // ---------------------------------------------------------------------------
  if (mode == I2C_ADDRESS_DEFINED) {
    I2C1_Wr(address << 1);
  }

  // ---------------------------------------------------------------------------
  // Write the payload
  // ---------------------------------------------------------------------------
  for (byte_index = 0;byte_index < length;++byte_index) {
    I2C1_Wr(payload[byte_index]);
  }

  // ---------------------------------------------------------------------------
  // Stop condition (if applicable)
  // ---------------------------------------------------------------------------
  if (end == I2C_STOP_BIT_APPLIED) {
    I2C1_Stop();
  }
}

// -----------------------------------------------------------------------------
// Read
//
//   mode    - Defines if the address has been defined.
//   address - The address of the device without the R/W bit.
//   length  - The length of the payload.
//   payload - The payload.
//   end     - Defines if the stop bit has been applied.
// -----------------------------------------------------------------------------
void rat_i2c_read_stream (uint8_t   mode,
                          uint8_t   address,
                          uint8_t   length,
                          uint8_t * payload,
                          uint8_t   end)
{
  // ---------------------------------------------------------------------------
  // Auxiliary variables
  // ---------------------------------------------------------------------------
  uint8_t byte_index = 0;

  // ---------------------------------------------------------------------------
  // Start condition
  // ---------------------------------------------------------------------------
  I2C1_Start();

  // ---------------------------------------------------------------------------
  // Write the address (if applicable)
  // ---------------------------------------------------------------------------
  if (mode == I2C_ADDRESS_DEFINED) {
    I2C1_Wr(( address << 1 ) + 1);
  }

  // ---------------------------------------------------------------------------
  // Read the payload
  // ---------------------------------------------------------------------------
  for (byte_index = 0;byte_index < length;++byte_index) {
    if (byte_index == length - 1) {
      payload[byte_index] = I2C1_Rd(0);
    } else {
      payload[byte_index] = I2C1_Rd(1);
    }
  }

  // ---------------------------------------------------------------------------
  // End (if applicable)
  // ---------------------------------------------------------------------------
  if (end == I2C_STOP_BIT_APPLIED) {
    I2C1_Stop();
  }
}