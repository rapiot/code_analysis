// -----------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2020 - 2024 Rapiot Open Hardware Project
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// I2C Utilities Header File
//
// The purpose of the utilities is to provide a high level layer
// for the I2C functions of mikroC.
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// -----------------------------------------------------------------------------
// Defines
// -----------------------------------------------------------------------------
#define I2C_ADDRESS_NOT_DEFINED  0
#define I2C_ADDRESS_DEFINED      1

#define I2C_STOP_BIT_NOT_APPLIED 0
#define I2C_STOP_BIT_APPLIED     1

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------

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
                           uint8_t   end);

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
                          uint8_t * input,
                          uint8_t   end);