// -----------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2020 - 2024 Rapiot Open Hardware Project
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
   
#include "../../rat_radio_modules/headers/rat_lorawan.h"
#include "../../rat_utilities/headers/rat_math_utilities.h"

// -----------------------------------------------------------------------------
// Read the parameters of the ABP
//
//   - Device address,                     32 bits - Addresses 0x00 - 0x03
//   - Device extended unique identifier,  64 bits - Addresses 0x10 - 0x17
//   - Network session key,               128 bits - Addresses 0x20 - 0x2F
//   - Application session key,           128 bits - Addresses 0x30 - 0x3F
// -----------------------------------------------------------------------------
void lorawan_read_abp_parameters (char * device_address,
                                  char * device_eui,
                                  char * network_session_key,
                                  char * application_session_key)
{
  uint8_t n;

  for (n = 0;n < LORAWAN_DEVICE_ADDRESS_BITS / 8;n++) {
    device_address[n * 2]     = rat_hex_to_char(EEPROM_Read(LORAWAN_DEVICE_ADDRESS_BASE + n) >> 4);
    device_address[n * 2 + 1] = rat_hex_to_char(EEPROM_Read(LORAWAN_DEVICE_ADDRESS_BASE + n) & 0x0F);
  }

  for (n = 0;n < LORAWAN_DEVICE_EUI_BITS / 8;n++) {
    device_eui[n * 2]     = rat_hex_to_char(EEPROM_Read(LORAWAN_DEVICE_EUI_BASE + n) >> 4);
    device_eui[n * 2 + 1] = rat_hex_to_char(EEPROM_Read(LORAWAN_DEVICE_EUI_BASE + n) & 0x0F);
  }

  for (n = 0;n < LORAWAN_NETWORK_SESSION_KEY_BITS / 8;n++) {
    network_session_key[n * 2]     = rat_hex_to_char(EEPROM_Read(LORAWAN_NETWORK_SESSION_KEY_BASE + n) >> 4);
    network_session_key[n * 2 + 1] = rat_hex_to_char(EEPROM_Read(LORAWAN_NETWORK_SESSION_KEY_BASE + n) & 0x0F);
  }

  for (n = 0;n < LORAWAN_APPLICATION_SESSION_KEY_BITS / 8;n++) {
    application_session_key[n * 2]     = rat_hex_to_char(EEPROM_Read(LORAWAN_APPLICATION_SESSION_KEY_BASE + n) >> 4);
    application_session_key[n * 2 + 1] = rat_hex_to_char(EEPROM_Read(LORAWAN_APPLICATION_SESSION_KEY_BASE + n) & 0x0F);
  }
}

void lorawan_read_device_address (char * device_address)
{
  uint8_t n;

  for (n = 0;n < LORAWAN_DEVICE_ADDRESS_BITS / 8;n++) {
    device_address[n * 2]     = rat_hex_to_char(EEPROM_Read(LORAWAN_DEVICE_ADDRESS_BASE + n) >> 4);
    device_address[n * 2 + 1] = rat_hex_to_char(EEPROM_Read(LORAWAN_DEVICE_ADDRESS_BASE + n) & 0x0F);
  }
}

void lorawan_read_device_eui (char * device_eui)
{
  uint8_t n;

  for (n = 0;n < LORAWAN_DEVICE_EUI_BITS / 8;n++) {
    device_eui[n * 2]     = rat_hex_to_char(EEPROM_Read(LORAWAN_DEVICE_EUI_BASE + n) >> 4);
    device_eui[n * 2 + 1] = rat_hex_to_char(EEPROM_Read(LORAWAN_DEVICE_EUI_BASE + n) & 0x0F);
  }
}

void lorawan_read_network_session_key (char * network_session_key)
{
  uint8_t n;

  for (n = 0;n < LORAWAN_NETWORK_SESSION_KEY_BITS / 8;n++) {
    network_session_key[n * 2]     = rat_hex_to_char(EEPROM_Read(LORAWAN_NETWORK_SESSION_KEY_BASE + n) >> 4);
    network_session_key[n * 2 + 1] = rat_hex_to_char(EEPROM_Read(LORAWAN_NETWORK_SESSION_KEY_BASE + n) & 0x0F);
  }
}

void lorawan_read_application_session_key (char * application_session_key)
{
  uint8_t n;

  for (n = 0;n < LORAWAN_APPLICATION_SESSION_KEY_BITS / 8;n++) {
    application_session_key[n * 2]     = rat_hex_to_char(EEPROM_Read(LORAWAN_APPLICATION_SESSION_KEY_BASE + n) >> 4);
    application_session_key[n * 2 + 1] = rat_hex_to_char(EEPROM_Read(LORAWAN_APPLICATION_SESSION_KEY_BASE + n) & 0x0F);
  }
}