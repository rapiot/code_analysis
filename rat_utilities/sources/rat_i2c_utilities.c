// ----------------------------------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2020 - 2022 Rapiot Open Hardware Project
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// I2C Utilities Source File
//
// The purpose of the utilities is to provide a high level layer for the I2C functions of mikroC.
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------------------------------
#include <stdlib.h>

#include "../../rat_utilities/headers/rat_i2c_utilities.h"

#ifdef TEST_MODE
  #include "../../rat_utilities/headers/rat_debug_utilities.h"
#endif

// ----------------------------------------------------------------------------------------------------
// Write
//
//   mode    - Defines if the address is defined. If the address is not defined, it is ignored.
//   address - The address of the device without the R/W bit.
//   length  - The length of the payload.
//   payload - The payload.
//   end     - Defines if the stop bit is applied. If the stop bit is not applied, it is ignored.
// ----------------------------------------------------------------------------------------------------
void rat_i2c_write_stream (uint8_t   mode,
                           uint8_t   address,
                           uint8_t   length,
                           uint8_t * payload,
                           uint8_t   end)
{
  // --------------------------------------------------------------------------------------------------
  // Auxiliary variables
  // --------------------------------------------------------------------------------------------------
  uint8_t n;

  // --------------------------------------------------------------------------------------------------
  // Debugging STARTS
  // --------------------------------------------------------------------------------------------------
  #ifdef TEST_MODE
    debug_write_new_line();
    debug_write_separator();
    debug_write_start_line();
    debug_write_string("I2C WR : ");
  #endif
  // --------------------------------------------------------------------------------------------------
  // Debugging ENDS
  // --------------------------------------------------------------------------------------------------

  // --------------------------------------------------------------------------------------------------
  // Start condition
  // --------------------------------------------------------------------------------------------------
  I2C1_Start();

  // --------------------------------------------------------------------------------------------------
  // Write the address (if applicable)
  // --------------------------------------------------------------------------------------------------
  if (mode == I2C_ADDRESS_DEFINED) {
    I2C1_Wr(address << 1);
  }

  // --------------------------------------------------------------------------------------------------
  // Write the payload
  // --------------------------------------------------------------------------------------------------
  for (n = 0;n < length;n++) {
    I2C1_Wr(payload[n]);
  }

  // --------------------------------------------------------------------------------------------------
  // Stop condition (if applicable)
  // --------------------------------------------------------------------------------------------------
  if (end == I2C_STOP_BIT_APPLIED) {
    I2C1_Stop();
  }
}

// ----------------------------------------------------------------------------------------------------
// Read
//
//   mode    - Defines if the address is defined. If the address is not defined, it is ignored.
//   address - The address of the device without the R/W bit.
//   length  - The length of the payload.
//   payload - The payload.
//   end     - Defines if the stop bit is applied. If the stop bit is not applied, it is ignored.
// ----------------------------------------------------------------------------------------------------
void rat_i2c_read_stream (uint8_t   mode,
                          uint8_t   address,
                          uint8_t   length,
                          uint8_t * payload,
                          uint8_t   end)
{
  // --------------------------------------------------------------------------------------------------
  // Auxiliary variables
  // --------------------------------------------------------------------------------------------------
  uint8_t n;

  #ifdef TEST_MODE
    debug_write_new_line();
    debug_write_separator();
    debug_write_start_line();
    debug_write_string("I2C RD : ");
  #endif

  // --------------------------------------------------------------------------------------------------
  // Start condition
  // --------------------------------------------------------------------------------------------------
  I2C1_Start();

  // --------------------------------------------------------------------------------------------------
  // Write the address (if applicable)
  // --------------------------------------------------------------------------------------------------
  if (mode == I2C_ADDRESS_DEFINED) {
    I2C1_Wr(( address << 1 ) + 1);
  }

  // --------------------------------------------------------------------------------------------------
  // Read the payload
  // --------------------------------------------------------------------------------------------------
  for (n = 0;n < length;n++) {
    if (n == length - 1) {
      payload[n] = I2C1_Rd(0);
    } else {
      payload[n] = I2C1_Rd(1);
    }
  }

  // --------------------------------------------------------------------------------------------------
  // End (if applicable)
  // --------------------------------------------------------------------------------------------------
  if (end == I2C_STOP_BIT_APPLIED) {
    I2C1_Stop();
  }

  // --------------------------------------------------------------------------------------------------
  // Debugging STARTS
  // --------------------------------------------------------------------------------------------------
  #ifdef TEST_MODE
    debug_write_string("SRT");

    if (mode == I2C_ADDRESS_DEFINED) {
      debug_write_string(" ");
      debug_write_unsigned_int_short_hex((address << 1) + 1);
    }

    for (n = 0;n < length;n++) {
      debug_write_string(" ");
      debug_write_unsigned_int_short_hex(payload[n]);
    }

    if (end == I2C_STOP_BIT_APPLIED) {
      debug_write_string(" ");
      debug_write_string("STP");
    }

    debug_write_separator();
  #endif
  // --------------------------------------------------------------------------------------------------
  // Debugging ENDS
  // --------------------------------------------------------------------------------------------------
}