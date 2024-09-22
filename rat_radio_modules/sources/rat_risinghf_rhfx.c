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
//   Only EU 868 MHz is supported.
//
//   Only RHF0M003-HF20 has been tested.
//
// Known issues :
//
//   There are no known issues.
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// RisingHF RHFX Source File
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../../rat_application/headers/defines.h"

// ----------------------------------------------------------------------------------------------------
// Conditional compilation
// ----------------------------------------------------------------------------------------------------
#ifdef RISINGHF_RHFX

// ----------------------------------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------------------------------
#include "../headers/rat_lorawan.h"

#include "../../rat_utilities/headers/rat_utilities.h"
#include "../../rat_utilities/headers/rat_mcu_utilities.h"
#include "../../rat_utilities/headers/rat_uart_utilities.h"

#ifdef TEST_MODE
  #include "../../rat_utilities/headers/rat_debug_utilities.h"
#endif

#include "../headers/rat_risinghf_rhfx.h"

// ----------------------------------------------------------------------------------------------------
// Buffers
// ----------------------------------------------------------------------------------------------------
char g_rat_req_buffer [RAT_UART_TX_BUFFER_SIZE];
char g_rat_rsp_buffer [RAT_UART_RX_BUFFER_SIZE];

// ----------------------------------------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Clear all the buffers
// ----------------------------------------------------------------------------------------------------
static void rat_radio_module_clear_buffers (void)
{
  rat_clear_string(g_rat_req_buffer,RAT_UART_TX_BUFFER_SIZE);
  rat_clear_string(g_rat_rsp_buffer,RAT_UART_RX_BUFFER_SIZE);
}

// ----------------------------------------------------------------------------------------------------
// Send request and receive response
// ----------------------------------------------------------------------------------------------------
static void rat_radio_command (char * req,
                               char * rsp,
                               bool   val)
{
  // --------------------------------------------------------------------------------------------------
  // Clear the UART buffer
  // --------------------------------------------------------------------------------------------------
  rat_uart_clear_buffer();

  // --------------------------------------------------------------------------------------------------
  // Request
  // --------------------------------------------------------------------------------------------------
#ifdef RAT_RADIO_MODULE_VERBOSE_MODE
  rat_debug_write_start_line();
  rat_debug_write_start_line();
  rat_debug_write_string(req);
#endif

  rat_uart_send_request(RAT_UART_NONE,
                        RAT_UART_CARRIER_RETURN_AND_NEW_LINE,
                        req);

  // --------------------------------------------------------------------------------------------------
  // Response
  // --------------------------------------------------------------------------------------------------
  rat_uart_receive_response(RAT_UART_NONE,
                            RAT_UART_CARRIER_RETURN_AND_NEW_LINE,
                            rsp,
                            val);

#ifdef RAT_RADIO_MODULE_VERBOSE_MODE
  rat_debug_write_start_line();
  rat_debug_write_string(rsp);
#endif
}

// ----------------------------------------------------------------------------------------------------
// Send request and receive response with delay
// ----------------------------------------------------------------------------------------------------
static void rat_radio_command_delay (char    * req,
                                     char    * rsp,
                                     uint8_t   delay)
{
  // --------------------------------------------------------------------------------------------------
  // Clear the UART buffer
  // --------------------------------------------------------------------------------------------------
  rat_uart_clear_buffer();

  // --------------------------------------------------------------------------------------------------
  // Request
  // --------------------------------------------------------------------------------------------------
#ifdef RAT_RADIO_MODULE_VERBOSE_MODE
  rat_debug_write_start_line();
  rat_debug_write_start_line();
  rat_debug_write_string(req);
#endif

  rat_uart_send_request(RAT_UART_NONE,
                        RAT_UART_CARRIER_RETURN_AND_NEW_LINE,
                        req);

  // --------------------------------------------------------------------------------------------------
  // Response
  // --------------------------------------------------------------------------------------------------
  rat_uart_receive_response_delay(RAT_UART_NONE,
                            RAT_UART_CARRIER_RETURN_AND_NEW_LINE,
                            rsp,
                            delay);

#ifdef RAT_RADIO_MODULE_VERBOSE_MODE
  rat_debug_write_start_line();
  rat_debug_write_string(rsp);
#endif
}

// ----------------------------------------------------------------------------------------------------
// Auxiliary function
  //   +(C)MSGHEX: PORT < PORT NUMBER >; RX: "< DOWNLINK PAYLOAD >"\r\n
// ----------------------------------------------------------------------------------------------------
static bool rat_radio_module_parse_downlink (char    * response,
                                             uint8_t * downlink_port,
                                             uint8_t * downlink_data,
                                             uint8_t   downlink_length)
{
  char * index;

  index = strstr(response,"RX: ");
  
  if (index == NULL) {
    return false;
  } else {
    rat_char_array_to_hex_array(response,
                                index + 4,
                                downlink_length,
                                downlink_data);
  
    return true;
  }
}

// ----------------------------------------------------------------------------------------------------
// Functions
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Init
// ----------------------------------------------------------------------------------------------------
void rat_radio_module_init (void)
{
  // --------------------------------------------------------------------------------------------------
  // Reset
  // --------------------------------------------------------------------------------------------------
  RAT_RADIO_MODULE_RST_TYP = 0b0;
  RAT_RADIO_MODULE_RST_DIR = 0b0;

  RAT_RADIO_MODULE_RST_PIN = 0b1;
}

// ----------------------------------------------------------------------------------------------------
// Reset
// ----------------------------------------------------------------------------------------------------
void rat_radio_module_reset (void)
{
  RAT_RADIO_MODULE_RST_PIN = 0b0;

  rat_delay_milliseconds(RAT_RADIO_MODULE_RESET_DELAY);

  RAT_RADIO_MODULE_RST_PIN = 0b1;
  
  rat_delay_milliseconds(RAT_RADIO_MODULE_RESET_DELAY);
}

// ----------------------------------------------------------------------------------------------------
// Set the device EUI
//
// Returns true if the response is valid; false otherwise.
// ----------------------------------------------------------------------------------------------------
static bool rat_radio_module_set_device_eui (char * device_eui)
{
  // --------------------------------------------------------------------------------------------------
  // Request and response
  //
  // Request :
  //
  //   AT+ID=DevEui, "0123456789ABCDEF"
  //
  // Response :
  //
  //   +ID: DevEui, 01:23:45:67:89:AB:CD:EF
  // --------------------------------------------------------------------------------------------------

  // --------------------------------------------------------------------------------------------------
  // Request and response
  // --------------------------------------------------------------------------------------------------
  rat_radio_module_clear_buffers();

  strcat(g_rat_req_buffer,"AT+ID=DevEui, \"");
  strcat(g_rat_req_buffer,device_eui);
  strcat(g_rat_req_buffer,"\"");
  
  // --------------------------------------------------------------------------------------------------
  // Send the request and check the response
  // --------------------------------------------------------------------------------------------------
  rat_radio_command(g_rat_req_buffer,g_rat_rsp_buffer,false);
}

// ----------------------------------------------------------------------------------------------------
// Set the application EUI
//
// Returns true if the response is valid; false otherwise.
// ----------------------------------------------------------------------------------------------------
static bool rat_radio_module_set_application_eui (char * application_eui)
{
  // --------------------------------------------------------------------------------------------------
  // Request and response
  //
  // Request :
  //
  //   AT+ID=AppEui, "0123456789ABCDEF"
  //
  // Response :
  //
  //   +ID: AppEui, 01:23:45:67:89:AB:CD:EF
  // --------------------------------------------------------------------------------------------------

  // --------------------------------------------------------------------------------------------------
  // Request and response
  // --------------------------------------------------------------------------------------------------
  rat_radio_module_clear_buffers();

  strcat(g_rat_req_buffer,"AT+ID=AppEui, \"");
  strcat(g_rat_req_buffer,application_eui);
  strcat(g_rat_req_buffer,"\"");

  // --------------------------------------------------------------------------------------------------
  // Send the request and check the response
  // --------------------------------------------------------------------------------------------------
  rat_radio_command(g_rat_req_buffer,g_rat_rsp_buffer,false);
}

// ----------------------------------------------------------------------------------------------------
// Set the application key
// ----------------------------------------------------------------------------------------------------
static bool rat_radio_module_set_application_key (char * application_key)
{
  // --------------------------------------------------------------------------------------------------
  // Request and response
  //
  // Request :
  //
  //   AT+KEY=APPKEY, "0123456789ABCDEF0123456789ABCDEF"
  //
  // Response :
  //
  //   +KEY: APPKEY, 0123456789ABCDEF0123456789ABCDEF
  // --------------------------------------------------------------------------------------------------

  // --------------------------------------------------------------------------------------------------
  // Request and response
  // --------------------------------------------------------------------------------------------------
  rat_radio_module_clear_buffers();

  strcat(g_rat_req_buffer,"AT+KEY=APPKEY, \"");
  strcat(g_rat_req_buffer,application_key);
  strcat(g_rat_req_buffer,"\"");

  // --------------------------------------------------------------------------------------------------
  // Send the request and check the response
  // --------------------------------------------------------------------------------------------------
  rat_radio_command(g_rat_req_buffer,g_rat_rsp_buffer,false);
}

// ----------------------------------------------------------------------------------------------------
// Set the device address
// ----------------------------------------------------------------------------------------------------
static bool rat_radio_module_set_device_address (char * device_address)
{
  // --------------------------------------------------------------------------------------------------
  // Request and response
  //
  // Request :
  //
  //   AT+ID=DevAddr, "01234567"
  //
  // Response :
  //
  //   +ID: DevAddr, 01:23:45:67
  // --------------------------------------------------------------------------------------------------

  // --------------------------------------------------------------------------------------------------
  // Request and response
  // --------------------------------------------------------------------------------------------------
  rat_radio_module_clear_buffers();

  strcat(g_rat_req_buffer,"AT+ID=DevAddr, \"");
  strcat(g_rat_req_buffer,device_address);
  strcat(g_rat_req_buffer,"\"");

  // --------------------------------------------------------------------------------------------------
  // Send the request and check the response
  // --------------------------------------------------------------------------------------------------
  rat_radio_command(g_rat_req_buffer,g_rat_rsp_buffer,false);
}

// ----------------------------------------------------------------------------------------------------
// Set the network session key
// ----------------------------------------------------------------------------------------------------
static bool rat_radio_module_set_network_session_key (char * network_session_key)
{
  // --------------------------------------------------------------------------------------------------
  // Request and response
  //
  // Request :
  //
  //   AT+KEY=NWKSKEY, "0123456789ABCDEF0123456789ABCDEF"
  //
  // Response :
  //
  //   +KEY: NWKSKEY, 0123456789ABCDEF0123456789ABCDEF
  // --------------------------------------------------------------------------------------------------

  // --------------------------------------------------------------------------------------------------
  // Request and response
  // --------------------------------------------------------------------------------------------------
  rat_radio_module_clear_buffers();

  strcat(g_rat_req_buffer,"AT+KEY=NWKSKEY, \"");
  strcat(g_rat_req_buffer,network_session_key);
  strcat(g_rat_req_buffer,"\"");

  // --------------------------------------------------------------------------------------------------
  // Send the request and check the response
  // --------------------------------------------------------------------------------------------------
  rat_radio_command(g_rat_req_buffer,g_rat_rsp_buffer,false);
}

// ----------------------------------------------------------------------------------------------------
// Set the application session key
// ----------------------------------------------------------------------------------------------------
static bool rat_radio_module_set_application_session_key (char * application_session_key)
{
  // --------------------------------------------------------------------------------------------------
  // Request and response
  //
  // Request :
  //
  //   AT+KEY=APPSKEY, "0123456789ABCDEF0123456789ABCDEF"
  //
  // Response :
  //
  //   +KEY: APPSKEY, 0123456789ABCDEF0123456789ABCDEF
  // --------------------------------------------------------------------------------------------------

  // --------------------------------------------------------------------------------------------------
  // Request and response
  // --------------------------------------------------------------------------------------------------
  rat_radio_module_clear_buffers();

  strcat(g_rat_req_buffer,"AT+ID=APPSKEY, \"");
  strcat(g_rat_req_buffer,application_session_key);
  strcat(g_rat_req_buffer,"\"");

  // --------------------------------------------------------------------------------------------------
  // Send the request and check the response
  // --------------------------------------------------------------------------------------------------
  rat_radio_command(g_rat_req_buffer,g_rat_rsp_buffer,false);
}

// ----------------------------------------------------------------------------------------------------
// Set the LoRaWAN mode
//
// Activation by personalization (ABP) :
//
//   Request  : "AT+MODE=LWABP"
//   Response : "+MODE: LWABP"
//
// Over-the-air activation (OTAA) :
//
//   Request  : "AT+MODE=LWOTAA"
//   Response : "+MODE: LWOTAA"
// ----------------------------------------------------------------------------------------------------
void rat_radio_module_set_mode (lorawan_mode mode)
{
  // --------------------------------------------------------------------------------------------------
  // Request and response
  // --------------------------------------------------------------------------------------------------
  rat_radio_module_clear_buffers();

  if (mode == LORAWAN_ABP) {
    strcat(g_rat_req_buffer,"AT+MODE=LWABP");
  } else {
    strcat(g_rat_req_buffer,"AT+MODE=LWOTAA");
  }

  // --------------------------------------------------------------------------------------------------
  // Send the request and check the response
  // --------------------------------------------------------------------------------------------------
  rat_radio_command(g_rat_req_buffer,g_rat_rsp_buffer,false);
}

// ----------------------------------------------------------------------------------------------------
// Set ABP parameters
// ----------------------------------------------------------------------------------------------------
bool rat_radio_module_set_abp (void)
{
  // --------------------------------------------------------------------------------------------------
  // Auxiliary variables
  // --------------------------------------------------------------------------------------------------
  bool result;
  
  // --------------------------------------------------------------------------------------------------
  // Parameters
  // --------------------------------------------------------------------------------------------------
  uint8_t device_address [4];                     //  32 bits
  uint8_t device_eui [8];                         //  64 bits
  uint8_t network_session_key [16];               // 128 bits
  uint8_t application_session_key [16];           // 128 bits

  char device_address_string [8 + 1];             //  8 digits, 1 null character
  char device_eui_string [16 + 1];                // 16 digits, 1 null character
  char network_session_key_string [32 + 1];       // 32 digits, no separators, 1 null character, unreadable
  char application_session_key_string [32 + 1];   // 32 digits, no separators, 1 null character, unreadable
  
  // --------------------------------------------------------------------------------------------------
  // Clear strings
  // --------------------------------------------------------------------------------------------------
  rat_clear_string(device_address_string,8 + 1);
  rat_clear_string(device_eui_string,16 + 1);
  rat_clear_string(network_session_key_string,32 + 1);
  rat_clear_string(application_session_key_string,32 + 1);

  // --------------------------------------------------------------------------------------------------
  // Read the parameters from the EEPROM
  // --------------------------------------------------------------------------------------------------
  lorawan_read_abp_parameters(device_address,
                              device_eui,
                              network_session_key,
                              application_session_key);

  // --------------------------------------------------------------------------------------------------
  // Convert from hex into string
  // --------------------------------------------------------------------------------------------------
  rat_hex_array_to_char_array(device_address,4,device_address_string,0);
  rat_hex_array_to_char_array(device_eui,8,device_eui_string,0);
  rat_hex_array_to_char_array(network_session_key,16,network_session_key_string,0);
  rat_hex_array_to_char_array(application_session_key,16,application_session_key_string,0);

  // --------------------------------------------------------------------------------------------------
  // Set the parameters to the radio module
  // --------------------------------------------------------------------------------------------------
  result = rat_radio_module_set_device_eui(device_eui_string);
  result = rat_radio_module_set_device_address(device_address_string);
  result = rat_radio_module_set_network_session_key(network_session_key_string);
  result = rat_radio_module_set_application_session_key(application_session_key_string);

#ifdef RAT_RADIO_MODULE_DEBUG_MODE
  rat_debug_write_new_line();
  rat_debug_write_separator();

  rat_debug_write_start_line();
  rat_debug_write_string("Device EUI              : ");
  rat_debug_write_string(device_eui_string);

  rat_debug_write_start_line();
  rat_debug_write_string("Device address          : ");
  rat_debug_write_string(device_address_string);

  rat_debug_write_start_line();
  rat_debug_write_string("Network session key     : ");
  rat_debug_write_string(network_session_key_string);

  rat_debug_write_start_line();
  rat_debug_write_string("Application session key : ");
  rat_debug_write_string(application_session_key_string);

  rat_debug_write_separator();
#endif

  return result;
}

// ----------------------------------------------------------------------------------------------------
// Set OTAA parameters
// ----------------------------------------------------------------------------------------------------
bool rat_radio_module_set_otaa (void)
{
  // --------------------------------------------------------------------------------------------------
  // Auxiliary variables
  // --------------------------------------------------------------------------------------------------
  bool result;

  // --------------------------------------------------------------------------------------------------
  // Parameters
  // --------------------------------------------------------------------------------------------------
  uint8_t device_eui [8];                 //  64 bits
  uint8_t application_eui [8];            //  64 bits
  uint8_t application_key [16];           // 128 bits

  char device_eui_string [16 + 1];        // 16 digits, 1 null character
  char application_eui_string [16 + 1];   // 16 digits, no separators, 1 null character
  char application_key_string [32 + 1];   // 32 digits, no separators, 1 null character, unreadable

  // --------------------------------------------------------------------------------------------------
  // Clear strings
  // --------------------------------------------------------------------------------------------------
  rat_clear_string(device_eui_string,16 + 1);
  rat_clear_string(application_eui_string,16 + 1);
  rat_clear_string(application_key_string,32 + 1);

  // --------------------------------------------------------------------------------------------------
  // Read the parameters from the EEPROM
  // --------------------------------------------------------------------------------------------------
  lorawan_read_otaa_parameters(device_eui,
                               application_eui,
                               application_key);

  // --------------------------------------------------------------------------------------------------
  // Convert from hex into string
  // --------------------------------------------------------------------------------------------------
  rat_hex_array_to_char_array(device_eui,8,device_eui_string,0);
  rat_hex_array_to_char_array(application_eui,8,application_eui_string,0);
  rat_hex_array_to_char_array(application_key,16,application_key_string,0);
  
  rat_clear_string(device_eui_string,16 + 1);
  rat_clear_string(application_eui_string,16 + 1);
  rat_clear_string(application_key_string,32 + 1);

  // --------------------------------------------------------------------------------------------------
  // Set the parameters to the radio module
  // --------------------------------------------------------------------------------------------------
  result = rat_radio_module_set_device_eui(device_eui_string);
  result = rat_radio_module_set_application_eui(application_eui_string);
  result = rat_radio_module_set_application_key(application_key_string);

#ifdef RAT_RADIO_MODULE_DEBUG_MODE
  rat_debug_write_new_line();
  rat_debug_write_separator();

  rat_debug_write_start_line();
  rat_debug_write_string("Device EUI      : ");
  rat_debug_write_string(device_eui_string);

  rat_debug_write_start_line();
  rat_debug_write_string("Application EUI : ");
  rat_debug_write_string(application_eui_string);

  rat_debug_write_start_line();
  rat_debug_write_string("Application key : ");
  rat_debug_write_string(application_key_string);

  rat_debug_write_separator();
#endif

  return result;
}

// ----------------------------------------------------------------------------------------------------
// Set the LoRaWAN parameters
// ----------------------------------------------------------------------------------------------------
bool rat_radio_module_set_parameters (lorawan_mode mode)
{
  bool result;
  
  if (mode == LORAWAN_ABP) {
    result = rat_radio_module_set_abp();
  } else {
    result = rat_radio_module_set_otaa();
  }
  
  return result;
}

// ----------------------------------------------------------------------------------------------------
// Join OTAA
//
// Request :
//
//   "AT+JOIN=FORCE"
//
// Response :
//
//   "+JOIN: Network joined"
// ----------------------------------------------------------------------------------------------------
bool rat_radio_module_join_otaa (void)
{
  // --------------------------------------------------------------------------------------------------
  // Request and response
  // --------------------------------------------------------------------------------------------------
  rat_radio_module_clear_buffers();

  strcat(g_rat_req_buffer,"AT+JOIN=FORCE");

  // --------------------------------------------------------------------------------------------------
  // Send the request and check the response
  // --------------------------------------------------------------------------------------------------
  rat_radio_command_delay(g_rat_req_buffer,g_rat_rsp_buffer,RAT_RADIO_MODULE_JOIN_DELAY);
  
  // --------------------------------------------------------------------------------------------------
  // Reference
  // --------------------------------------------------------------------------------------------------
  if (rat_string_find(g_rat_rsp_buffer,"+JOIN: Network joined")) {
    return true;
  } else {
    return false;
  }
}


// ----------------------------------------------------------------------------------------------------
// Transmit and receive a message
// ----------------------------------------------------------------------------------------------------

bool rat_radio_module_transmit (uint8_t   uplink_port,       // Uplink port
                                uint8_t   uplink_length,     // Uplink payload length
                                uint8_t * uplink_data,       // Uplink payload data
                                bool    * uplink_status,     // Uplink transmission status

                                bool      acknowledge,       // Confirmed or unconfirmed message

                                uint8_t * downlink_port,     // Downlink port
                                uint8_t   downlink_length,   // Downlink data length
                                uint8_t * downlink_data,     // Downlink payload data
                                bool    * downlink_status)   // Downlink transmission status
{
  // --------------------------------------------------------------------------------------------------
  // Request and response
  //
  // Request :
  //
  //   AT+(C)MSGHEX="< UPLINK PAYLOAD >"
  //
  // Response :
  //
  //   +(C)MSGHEX: Start\r\n
  //   ...
  //   +(C)MSGHEX: ACK received\r\n
  //   +(C)MSGHEX: PORT < PORT NUMBER >; RX: "< DOWNLINK PAYLOAD >"\r\n
  //   +(C)MSGHEX: Done\r\n
  // --------------------------------------------------------------------------------------------------

  // --------------------------------------------------------------------------------------------------
  // Request and response
  // --------------------------------------------------------------------------------------------------
  rat_radio_module_clear_buffers();

  if (acknowledge) {
    strcat(g_rat_req_buffer,"AT+CMSGHEX=\"");
  } else {
    strcat(g_rat_req_buffer,"AT+MSGHEX=\"");
  }

  rat_hex_array_to_char_array(uplink_data,
                              uplink_length,
                              g_rat_req_buffer,
                              strlen(g_rat_req_buffer));

  strcat(g_rat_req_buffer,"\"");

  // --------------------------------------------------------------------------------------------------
  // Send the request and check the response
  // --------------------------------------------------------------------------------------------------
  rat_radio_command_delay(g_rat_req_buffer,g_rat_rsp_buffer,RAT_RADIO_MODULE_DEFAULT_DELAY);


  // --------------------------------------------------------------------------------------------------
  // Parse the response
  // --------------------------------------------------------------------------------------------------
  if (acknowledge) {
    *uplink_status = rat_string_find (g_rat_rsp_buffer,"+CMSGHEX: Done");
  } else {
    *uplink_status = rat_string_find (g_rat_rsp_buffer,"+MSGHEX: Done");
  }

  if (acknowledge) {
    *downlink_status = rat_string_find(g_rat_rsp_buffer,"+CMSGHEX: PORT: 1; RX: ");
  } else {
    *downlink_status = rat_string_find(g_rat_rsp_buffer,"+MSGHEX: PORT: 1; RX: ");
  }

  if (acknowledge && *downlink_status) {
    *downlink_status = rat_radio_module_parse_downlink(g_rat_rsp_buffer,
                                                       downlink_port,
                                                       downlink_data,
                                                       downlink_length);
  }
}

// ----------------------------------------------------------------------------------------------------
// Sleep
//
// Request :
//
//   "AT+LOWPOWER"
//
// Response :
//
//   "+LOWPOWER: SLEEP"
// ----------------------------------------------------------------------------------------------------
bool rat_radio_module_sleep (void)
{
  // --------------------------------------------------------------------------------------------------
  // Request and response
  // --------------------------------------------------------------------------------------------------
  rat_radio_module_clear_buffers();

  strcat(g_rat_req_buffer,"AT+LOWPOWER");

  // --------------------------------------------------------------------------------------------------
  // Send the request and check the response
  // --------------------------------------------------------------------------------------------------
  rat_radio_command(g_rat_req_buffer,g_rat_rsp_buffer,false);
}

// ----------------------------------------------------------------------------------------------------
// Wakeup
//
// Request :
//
//   "W"
//
// Response :
//
//   "+LOWPOWER: WAKEUP"
// ----------------------------------------------------------------------------------------------------
uint8_t rat_radio_module_wakeup (void)
{
  // --------------------------------------------------------------------------------------------------
  // Request and response
  // --------------------------------------------------------------------------------------------------
  rat_radio_module_clear_buffers();

  strcat(g_rat_req_buffer,"W");

  // --------------------------------------------------------------------------------------------------
  // Send the request and check the response
  // --------------------------------------------------------------------------------------------------
  rat_radio_command(g_rat_req_buffer,g_rat_rsp_buffer,false);
}

#endif