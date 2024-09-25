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
void read_abp_parameters (char * device_address,
                          char * device_eui,
                          char * network_session_key,
                          char * application_session_key)
{
  uint8_t counter = 0;
  uint8_t byte    = 0x00;

  for (counter = 0;counter < DEVADD_BITS / 8;counter++) {
    byte = EEPROM_Read(DEVADD_BASE + counter);
  
    device_address[counter * 2]     = rat_hex_to_char(byte >> 4);
    device_address[counter * 2 + 1] = rat_hex_to_char(byte & 0x0F);
  }

  for (counter = 0;counter < DEVEUI_BITS / 8;counter++) {
    byte = EEPROM_Read(DEVEUI_BASE + counter);
    
    device_eui[counter * 2]     = rat_hex_to_char(byte >> 4);
    device_eui[counter * 2 + 1] = rat_hex_to_char(byte & 0x0F);
  }

  for (counter = 0;counter < DEVNSK_BITS / 8;counter++) {
    byte = EEPROM_Read(DEVNSK_BASE + counter);
    
    network_session_key[counter * 2]     = rat_hex_to_char(byte >> 4);
    network_session_key[counter * 2 + 1] = rat_hex_to_char(byte & 0x0F);
  }

  for (counter = 0;counter < DEVASK_BITS / 8;counter++) {
    byte = EEPROM_Read(DEVASK_BASE + counter);
    
    application_session_key[counter * 2]     = rat_hex_to_char(byte >> 4);
    application_session_key[counter * 2 + 1] = rat_hex_to_char(byte & 0x0F);
  }
}

void read_device_address (char * device_address)
{
  uint8_t counter = 0;
  uint8_t byte    = 0x00;

  for (counter = 0;counter < DEVADD_BITS / 8;counter++) {
    byte = EEPROM_Read(DEVADD_BASE + counter);
  
    device_address[counter * 2]     = rat_hex_to_char(byte >> 4);
    device_address[counter * 2 + 1] = rat_hex_to_char(byte & 0x0F);
  }
}

void read_device_eui (char * device_eui)
{
  uint8_t counter = 0;
  uint8_t byte    = 0x00;

  for (counter = 0;counter < DEVEUI_BITS / 8;counter++) {
    byte = EEPROM_Read(DEVEUI_BASE + counter);
    
    device_eui[counter * 2]     = rat_hex_to_char(byte >> 4);
    device_eui[counter * 2 + 1] = rat_hex_to_char(byte & 0x0F);
  }
}

void read_network_session_key (char * network_session_key)
{
  uint8_t counter = 0;
  uint8_t byte    = 0x00;

  for (counter = 0;counter < DEVNSK_BITS / 8;counter++) {
    byte = EEPROM_Read(DEVNSK_BASE + counter);
    
    network_session_key[counter * 2]     = rat_hex_to_char(byte >> 4);
    network_session_key[counter * 2 + 1] = rat_hex_to_char(byte & 0x0F);
  }
}

void read_application_session_key (char * application_session_key)
{
  uint8_t counter = 0;
  uint8_t byte    = 0x00;

  for (counter = 0;counter < DEVASK_BITS / 8;counter++) {
    byte = EEPROM_Read(DEVASK_BASE + counter);
    
    application_session_key[counter * 2]     = rat_hex_to_char(byte >> 4);
    application_session_key[counter * 2 + 1] = rat_hex_to_char(byte & 0x0F);
  }
}
