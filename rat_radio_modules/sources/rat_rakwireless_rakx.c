// -----------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2021 - 2024 Rapiot Open Hardware Project
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// RAK Wireless RAKX Source File
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../../rat_utilities/headers/rat_math_utilities.h"
#include "../../rat_utilities/headers/rat_pic_utilities.h"
#include "../../rat_utilities/headers/rat_uart_utilities.h"
#include "../../rat_radio_modules/headers/rat_lorawan.h"
#include "../../rat_radio_modules/headers/rat_rakwireless_rakx.h"

// -----------------------------------------------------------------------------
// Constants
// -----------------------------------------------------------------------------
#define RAT_UART_BUFFER_SIZE 64

// -----------------------------------------------------------------------------
// Buffers
// -----------------------------------------------------------------------------
char g_rat_req_buffer [RAT_UART_BUFFER_SIZE];
char g_rat_rsp_buffer [RAT_UART_BUFFER_SIZE];

// -----------------------------------------------------------------------------
// Static functions
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Clear all the buffers
// -----------------------------------------------------------------------------
static void rat_radio_module_clear_buffers (void)
{
  rat_clear_string(g_rat_req_buffer,RAT_UART_BUFFER_SIZE);
  rat_clear_string(g_rat_rsp_buffer,RAT_UART_BUFFER_SIZE);
}

// -----------------------------------------------------------------------------
// Check the response
// -----------------------------------------------------------------------------
static bool rat_check_response (char * rsp,
                                bool   value_expected)
{
  bool result = false;
  
  if (value_expected && rat_string_compare_reverse(rsp,"OK")) {
    result = true;
  } else if (!value_expected && rat_string_compare(rsp,"OK")) {
    result = true;
  } else {
    result = false;
  }

  return result;
}

// -----------------------------------------------------------------------------
// Send request, receive response, and check response
// -----------------------------------------------------------------------------
static bool rat_radio_command (char * req,
                               char * rsp,
                               bool   val)
{
  // ---------------------------------------------------------------------------
  // Clear the UART buffer
  // ---------------------------------------------------------------------------
  rat_uart_clear_buffer();
  
  // ---------------------------------------------------------------------------
  // Request
  // ---------------------------------------------------------------------------
  rat_uart_request(RAT_UART_NONE,
                   RAT_UART_CARRIER_RETURN_AND_NEW_LINE,
                   req);

  // ---------------------------------------------------------------------------
  // Response
  // ---------------------------------------------------------------------------
  if (val) {
    rat_uart_response_with_value(RAT_UART_CARRIER_RETURN_AND_NEW_LINE,
                                 RAT_UART_CARRIER_RETURN_AND_NEW_LINE,
                                 rsp);
  } else {
    rat_uart_response(RAT_UART_CARRIER_RETURN_AND_NEW_LINE,
                      RAT_UART_CARRIER_RETURN_AND_NEW_LINE,
                      rsp);
  }

  // ---------------------------------------------------------------------------
  // Reference
  // ---------------------------------------------------------------------------
  if (rat_check_response(rsp,val)) {
    return true;
  } else {
    return false;
  }
}

// -----------------------------------------------------------------------------
// Auxiliary function
// -----------------------------------------------------------------------------
static bool rat_radio_module_parse_downlink (char    * response,
                                             uint8_t * downlink_data,
                                             uint8_t   downlink_length)
{
  uint8_t length = 0;

  if (!rat_string_compare(response,RAT_RADIO_MODULE_DOWNLINK_PORT)) {
    return false;
  } else {
    length = strlen(response) - 2;
    
    if ((length / 2) != downlink_length) {
      return false;
    } else {
      rat_char_array_to_hex_array (response,
                                   2,
                                   length,
                                   downlink_data);
                                 
      return true;
    }
  }
}

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Init
// -----------------------------------------------------------------------------
void rat_radio_module_init (void)
{
  // ---------------------------------------------------------------------------
  // Reset
  // ---------------------------------------------------------------------------
  RAT_RADIO_MODULE_RST_TYP = 0b0;
  RAT_RADIO_MODULE_RST_DIR = 0b0;

  RAT_RADIO_MODULE_RST_PIN = 0b1;
}

// -----------------------------------------------------------------------------
// Reset
// -----------------------------------------------------------------------------
void rat_radio_module_reset (void)
{
  RAT_RADIO_MODULE_RST_PIN = 0b0;

  rat_delay(RAT_RADIO_MODULE_RESET_DELAY);

  RAT_RADIO_MODULE_RST_PIN = 0b1;

  rat_delay(RAT_RADIO_MODULE_RESET_DELAY);
}

// -----------------------------------------------------------------------------
// Set the device EUI
// -----------------------------------------------------------------------------
static bool rat_radio_module_set_device_eui (void)
{
  // ---------------------------------------------------------------------------
  // Request and response
  // ---------------------------------------------------------------------------
  rat_uart_clear_buffer();
  rat_radio_module_clear_buffers();
  
  (void)strcat(g_rat_req_buffer,"AT+DEVEUI=");
  
  read_device_eui(&g_rat_req_buffer[strlen(g_rat_req_buffer)]);

  // ---------------------------------------------------------------------------
  // Send the request and check the response
  // ---------------------------------------------------------------------------
  if (rat_radio_command(g_rat_req_buffer,g_rat_rsp_buffer,false)) {
    return true;
  } else {
    return false;
  }
}

// -----------------------------------------------------------------------------
// Set the device address
// -----------------------------------------------------------------------------
static bool rat_radio_module_set_device_address (void)
{
  // ---------------------------------------------------------------------------
  // Request and response
  // ---------------------------------------------------------------------------
  rat_uart_clear_buffer();
  rat_radio_module_clear_buffers();

  (void)strcat(g_rat_req_buffer,"AT+DEVADDR=");

  read_device_address(&g_rat_req_buffer[strlen(g_rat_req_buffer)]);

  // ---------------------------------------------------------------------------
  // Send the request and check the response
  // ---------------------------------------------------------------------------
  if (rat_radio_command(g_rat_req_buffer,g_rat_rsp_buffer,false)) {
    return true;
  } else {
    return false;
  }
}

// -----------------------------------------------------------------------------
// Set the network session key
//
// Returns true if the response is valid; false otherwise.
// -----------------------------------------------------------------------------
static bool rat_radio_module_set_network_session_key (void)
{
  // ---------------------------------------------------------------------------
  // Request and response
  // ---------------------------------------------------------------------------
  rat_uart_clear_buffer();
  rat_radio_module_clear_buffers();

  (void)strcat(g_rat_req_buffer,"AT+NWKSKEY=");

  read_network_session_key(&g_rat_req_buffer[strlen(g_rat_req_buffer)]);

  // ---------------------------------------------------------------------------
  // Send the request and check the response
  // ---------------------------------------------------------------------------
  if (rat_radio_command(g_rat_req_buffer,g_rat_rsp_buffer,false)) {
    return true;
  } else {
    return false;
  }
}

// -----------------------------------------------------------------------------
// Set the application session key
// -----------------------------------------------------------------------------
static bool rat_radio_module_set_application_session_key (void)
{
  // ---------------------------------------------------------------------------
  // Request and response
  // ---------------------------------------------------------------------------
  rat_uart_clear_buffer();
  rat_radio_module_clear_buffers();

  (void)strcat(g_rat_req_buffer,"AT+APPSKEY=");

  read_application_session_key(&g_rat_req_buffer[strlen(g_rat_req_buffer)]);

  // ---------------------------------------------------------------------------
  // Send the request and check the response
  // ---------------------------------------------------------------------------
  if (rat_radio_command(g_rat_req_buffer,g_rat_rsp_buffer,false)) {
    return true;
  } else {
    return false;
  }
}

// -----------------------------------------------------------------------------
// Set the ABP mode
// -----------------------------------------------------------------------------
bool rat_radio_module_set_abp_mode (void)
{
  // ---------------------------------------------------------------------------
  // Request and response
  // ---------------------------------------------------------------------------
  rat_uart_clear_buffer();
  rat_radio_module_clear_buffers();

  (void)strcat(g_rat_req_buffer,"AT+NJM=0");

  // ---------------------------------------------------------------------------
  // Send the request and check the response
  // ---------------------------------------------------------------------------
  if (rat_radio_command(g_rat_req_buffer,g_rat_rsp_buffer,false)) {
    return true;
  } else {
    return false;
  }
}

// -----------------------------------------------------------------------------
// Set the ABP parameters
// -----------------------------------------------------------------------------
bool rat_radio_module_set_abp_parameters (void)
{
  // ---------------------------------------------------------------------------
  // Auxiliary variables
  // ---------------------------------------------------------------------------
  bool result = true;

  // ---------------------------------------------------------------------------
  // Set the parameters to the radio module
  // ---------------------------------------------------------------------------
  result = result && rat_radio_module_set_device_eui();
  result = result && rat_radio_module_set_device_address();
  result = result && rat_radio_module_set_network_session_key();
  result = result && rat_radio_module_set_application_session_key();

  return result;
}

// -----------------------------------------------------------------------------
// Transmit and receive a message
// -----------------------------------------------------------------------------
bool rat_radio_module_transmit (uint8_t   uplink_length,
                                uint8_t * uplink_data,
                                bool    * uplink_status,

                                uint8_t   downlink_length,
                                uint8_t * downlink_data,
                                bool    * downlink_status)
{
  // ---------------------------------------------------------------------------
  // Auxiliary variables
  // ---------------------------------------------------------------------------
  uint8_t counter = 0;

  // ---------------------------------------------------------------------------
  // Set the message type
  // ---------------------------------------------------------------------------
  rat_uart_clear_buffer();
  rat_radio_module_clear_buffers();

  (void)strcat(g_rat_req_buffer,"AT+CFM=0");

  if (!rat_radio_command(g_rat_req_buffer,g_rat_rsp_buffer,false)) {
    return false;
  }

  // ---------------------------------------------------------------------------
  // Create and send the uplink message
  // ---------------------------------------------------------------------------
  rat_uart_clear_buffer();
  rat_radio_module_clear_buffers();
  
  (void)strcat(g_rat_req_buffer,"AT+SEND=");
  (void)strcat(g_rat_req_buffer,RAT_RADIO_MODULE_UPLINK_PORT);
  (void)strcat(g_rat_req_buffer,"AT+SEND=:");

  rat_hex_array_to_char_array(uplink_data,
                              uplink_length,
                              g_rat_req_buffer,
                              strlen(g_rat_req_buffer));

  if (!rat_radio_command(g_rat_req_buffer,g_rat_rsp_buffer,false)) {
    *uplink_status = false;
  
    return false;
  } else {
    *uplink_status = true;
  }
  
  // ---------------------------------------------------------------------------
  // Wait until the downlink message has been processed
  // ---------------------------------------------------------------------------
  for (counter = 0;counter < RAT_RADIO_MODULE_RESPONSE_DELAY;++counter) {
    rat_wait_interrupt();
  }
  
  // ---------------------------------------------------------------------------
  // Check downlink data
  // ---------------------------------------------------------------------------
  rat_uart_clear_buffer();
  rat_radio_module_clear_buffers();
  
  (void)strcat(g_rat_req_buffer,"AT+RECV=?");
  
  *downlink_status = false;

  if (rat_radio_command(g_rat_req_buffer,g_rat_rsp_buffer,true)) {
    if (rat_radio_module_parse_downlink(g_rat_rsp_buffer,
                                        downlink_data,
                                        downlink_length)) {
      *downlink_status = true;
    }
  }

  // ---------------------------------------------------------------------------
  // Return status
  // ---------------------------------------------------------------------------
  return true;
}