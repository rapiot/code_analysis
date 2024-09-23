// ----------------------------------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2020 - 2022 Rapiot Open Hardware Project
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// UART Utilities Source File
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../../rat_utilities/headers/rat_utilities.h"
#include "../../rat_utilities/headers/rat_utilities.h"
#include "../../rat_utilities/headers/rat_uart_utilities.h"


#define RAT_UART_BUFFER_SIZE 64

// ----------------------------------------------------------------------------------------------------
// Clear the buffer
// ----------------------------------------------------------------------------------------------------
void rat_uart_clear_buffer ()
{
  char temp;

  while (UART1_Data_Ready()) {
    temp = UART1_Read();
  }
}

// ----------------------------------------------------------------------------------------------------
// Write separator
// ----------------------------------------------------------------------------------------------------
static void rat_uart_write_separator (rat_uart_separator separator)
{
  // --------------------------------------------------------------------------------------------------
  // "\r\n"
  // --------------------------------------------------------------------------------------------------
  if (separator == RAT_UART_CARRIER_RETURN_AND_NEW_LINE) {
    UART1_Write('\r');
    UART1_Write('\n');

  // --------------------------------------------------------------------------------------------------
  // '\r'
  // --------------------------------------------------------------------------------------------------
  } else if (separator == RAT_UART_CARRIER_RETURN) {
    UART1_Write('\r');

  // --------------------------------------------------------------------------------------------------
  // '\n'
  // --------------------------------------------------------------------------------------------------
  } else if (separator == RAT_UART_NEW_LINE) {
    UART1_Write('\n');
  }
}

// ----------------------------------------------------------------------------------------------------
// Read separator
// ----------------------------------------------------------------------------------------------------
static void rat_uart_read_separator (rat_uart_separator separator)
{
  // --------------------------------------------------------------------------------------------------
  // Auxiliary variables
  // --------------------------------------------------------------------------------------------------
  char temp;

  // --------------------------------------------------------------------------------------------------
  // "\r\n"
  // --------------------------------------------------------------------------------------------------
  if (separator == RAT_UART_CARRIER_RETURN_AND_NEW_LINE) {
    // ------------------------------------------------------------------------------------------------
    // \r
    // ------------------------------------------------------------------------------------------------
    while (1) {
      while (!UART1_Data_Ready()) {
        // Wait for a new character
      }

      temp = UART1_Read();

      if (temp == '\r') {
        break;
      }
    }

    // ------------------------------------------------------------------------------------------------
    // \n
    // ------------------------------------------------------------------------------------------------
    while (1) {
      while (!UART1_Data_Ready()) {
        // Wait for a new character
      }

      temp = UART1_Read();

      if (temp == '\n') {
        break;
      }
    }

  // --------------------------------------------------------------------------------------------------
  // '\r'
  // --------------------------------------------------------------------------------------------------
  } else if (separator == RAT_UART_CARRIER_RETURN) {
    while (1) {
      while (!UART1_Data_Ready()) {
        // Wait for a new character
      }

      temp = UART1_Read();

      if (temp == '\r') {
        break;
      }
    }

  // --------------------------------------------------------------------------------------------------
  // '\n'
  // --------------------------------------------------------------------------------------------------
  } else if (separator == RAT_UART_NEW_LINE) {
    while (1) {
      while (!UART1_Data_Ready()) {
        // Wait for a new character
      }

      temp = UART1_Read();

      if (temp == '\n') {
        break;
      }
    }
  }
}

// ----------------------------------------------------------------------------------------------------
// Check separator
// ----------------------------------------------------------------------------------------------------
static bool rat_check_separator (rat_uart_separator   separator,
                                 char               * stream,
                                 uint8_t              index)
{
  // --------------------------------------------------------------------------------------------------
  // "\r\n"
  // --------------------------------------------------------------------------------------------------
  if (separator == RAT_UART_CARRIER_RETURN_AND_NEW_LINE) {
    if (index > 0) {
      if (stream[index]     == '\n' &&
          stream[index - 1] == '\r') {
        return true;
      } else {
        return false;
      }
    } else {
      return false;
    }

  // --------------------------------------------------------------------------------------------------
  // '\r'
  // --------------------------------------------------------------------------------------------------
  } else if (separator == RAT_UART_CARRIER_RETURN) {
    if (stream[index] == '\r') {
      return true;
    } else {
      return false;
    }

  // --------------------------------------------------------------------------------------------------
  // '\n'
  // --------------------------------------------------------------------------------------------------
  } else if (separator == RAT_UART_NEW_LINE) {
    if (stream[index] == '\n') {
      return true;
    } else {
      return false;
    }
  }
}

// ----------------------------------------------------------------------------------------------------
// Remove separator
// ----------------------------------------------------------------------------------------------------
static void rat_remove_separator (rat_uart_separator   separator,
                                  char               * stream,
                                  uint8_t            * current_index)
{
  // --------------------------------------------------------------------------------------------------
  // Auxiliary variables
  // --------------------------------------------------------------------------------------------------
  uint8_t new_index = *current_index;
  
  // --------------------------------------------------------------------------------------------------
  // "\r\n"
  // --------------------------------------------------------------------------------------------------
  if (separator == RAT_UART_CARRIER_RETURN_AND_NEW_LINE) {
    stream[new_index]     = '\0';
    stream[new_index - 1] = '\0';
    
    new_index = new_index - 2;

  // --------------------------------------------------------------------------------------------------
  // '\r'
  // --------------------------------------------------------------------------------------------------
  } else if (separator == RAT_UART_CARRIER_RETURN) {
    stream[new_index] = '\0';
    
    new_index = new_index - 1;

  // --------------------------------------------------------------------------------------------------
  // '\n'
  // --------------------------------------------------------------------------------------------------
  } else if (separator == RAT_UART_NEW_LINE) {
    stream[new_index] = '\0';
    
    new_index = new_index - 1;
  }
  
  // --------------------------------------------------------------------------------------------------
  // Set the new index
  // --------------------------------------------------------------------------------------------------
  *current_index = new_index;
}

// ----------------------------------------------------------------------------------------------------
// Send a request
// ----------------------------------------------------------------------------------------------------
void rat_uart_send_request (rat_uart_separator   leading_separator,
                            rat_uart_separator   trailing_separator,
                            char               * request)
{
  // --------------------------------------------------------------------------------------------------
  // Auxiliary variables
  // --------------------------------------------------------------------------------------------------
  uint8_t n;

  // --------------------------------------------------------------------------------------------------
  // Write leading separator
  // --------------------------------------------------------------------------------------------------
  if (leading_separator != RAT_UART_NONE) {
    rat_uart_write_separator(leading_separator);
  }

  // --------------------------------------------------------------------------------------------------
  // Send request
  // --------------------------------------------------------------------------------------------------
  n = 0;
  
  while (request[n] != '\0') {
    UART1_Write(request[n]);
    
    n++;
    
    if (n == RAT_UART_BUFFER_SIZE - 1) {
      break;
    }
  }

  // --------------------------------------------------------------------------------------------------
  // Write trailing separator
  // --------------------------------------------------------------------------------------------------
  if (trailing_separator != RAT_UART_NONE) {
    rat_uart_write_separator(trailing_separator);
  }
}

// ----------------------------------------------------------------------------------------------------
// Receive value (if applicable) and response code
// ----------------------------------------------------------------------------------------------------
void rat_uart_receive_response (rat_uart_separator   leading_separator,
                                rat_uart_separator   trailing_separator,
                                char               * response,
                                bool                 value)
{
  // --------------------------------------------------------------------------------------------------
  // Auxiliary variables
  // --------------------------------------------------------------------------------------------------
  uint8_t n;
  
  bool value_flag = false;

  // --------------------------------------------------------------------------------------------------
  // Leading separator
  // --------------------------------------------------------------------------------------------------
  if (leading_separator != RAT_UART_NONE) {
    rat_uart_read_separator(leading_separator);
  }

  // --------------------------------------------------------------------------------------------------
  // Read value (if applicable) and response code
  // --------------------------------------------------------------------------------------------------
  n = 0;

  while (true) {
    while (!UART1_Data_Ready()) {

    }

    response[n] = UART1_Read();
    
    if (rat_check_separator(trailing_separator,response,n)) {
      rat_remove_separator(trailing_separator,response,&n);
    
      if (value) {
        if (value_flag) {
          break;
        } else {
          value_flag = true;
        }
      } else {
        break;
      }
    }

    if (n == RAT_UART_BUFFER_SIZE - 1) {
      break;
    }
    
    n++;
  }
}

// ----------------------------------------------------------------------------------------------------
// Receive response with delay
// ----------------------------------------------------------------------------------------------------
void rat_uart_receive_response_delay (rat_uart_separator   leading_separator,
                                      rat_uart_separator   trailing_separator,
                                      char               * response,
                                      uint8_t              delay)
{
  // --------------------------------------------------------------------------------------------------
  // Auxiliary variables
  // --------------------------------------------------------------------------------------------------
  uint8_t n;
  
  uint32_t initial_interrupt_counter;

  // --------------------------------------------------------------------------------------------------
  // Leading separator
  // --------------------------------------------------------------------------------------------------
  if (leading_separator != RAT_UART_NONE) {
    rat_uart_read_separator(leading_separator);
  }

  // --------------------------------------------------------------------------------------------------
  // Read response
  // --------------------------------------------------------------------------------------------------
  initial_interrupt_counter = rat_interrupt_counter();
  
  n = 0;

  while (true) {
    if (rat_interrupt_counter() - initial_interrupt_counter >= delay) {
      break;
    }
    
    if (UART1_Data_Ready()) {
      response[n] = UART1_Read();

      if (n == RAT_UART_BUFFER_SIZE - 1) {
        break;
      }
    
      n++;
    }
  }
}