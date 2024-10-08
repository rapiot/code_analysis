// -----------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2020 - 2024 Rapiot Open Hardware Project
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// UART Utilities Source File
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../../rat_utilities/headers/rat_math_utilities.h"
#include "../../rat_utilities/headers/rat_uart_utilities.h"

// -----------------------------------------------------------------------------
// Clear the buffer
// -----------------------------------------------------------------------------
void rat_uart_clear_buffer (void)
{
  while (UART1_Data_Ready() != 0) {
    (void) UART1_Read();
  }
}

// -----------------------------------------------------------------------------
// Write separator
// -----------------------------------------------------------------------------
static void rat_uart_write_separator (rat_uart_separator separator)
{
  // ---------------------------------------------------------------------------
  // "\r\n"
  // ---------------------------------------------------------------------------
  if (separator == RAT_UART_CARRIER_RETURN_AND_NEW_LINE) {
    (void)UART1_Write('\r');
    (void)UART1_Write('\n');

  // ---------------------------------------------------------------------------
  // '\r'
  // ---------------------------------------------------------------------------
  } else if (separator == RAT_UART_CARRIER_RETURN) {
    (void)UART1_Write('\r');

  // ---------------------------------------------------------------------------
  // '\n'
  // ---------------------------------------------------------------------------
  } else if (separator == RAT_UART_NEW_LINE) {
    (void)UART1_Write('\n');
  }
}

// -----------------------------------------------------------------------------
// Read char
// -----------------------------------------------------------------------------
static void rat_uart_read_char (char character)
{
  char temp = '\0';
  
  while (true) {
    while (UART1_Data_Ready() == 0) {
      // Wait for a new character
    }

    temp = UART1_Read();

    if (temp == character) {
      break;
    }
  }
}

// -----------------------------------------------------------------------------
// Read separator
// -----------------------------------------------------------------------------
static void rat_uart_read_separator (rat_uart_separator separator)
{
  // ---------------------------------------------------------------------------
  // "\r\n"
  // ---------------------------------------------------------------------------
  if (separator == RAT_UART_CARRIER_RETURN_AND_NEW_LINE) {
    rat_uart_read_char ('\r');
    rat_uart_read_char ('\n');

  // ---------------------------------------------------------------------------
  // '\r'
  // ---------------------------------------------------------------------------
  } else if (separator == RAT_UART_CARRIER_RETURN) {
    rat_uart_read_char ('\r');
    
  // ---------------------------------------------------------------------------
  // '\n'
  // ---------------------------------------------------------------------------
  } else if (separator == RAT_UART_NEW_LINE) {
    rat_uart_read_char ('\n');
  }
}

// -----------------------------------------------------------------------------
// Check separator
// -----------------------------------------------------------------------------
static bool rat_check_separator (rat_uart_separator   separator,
                                 char               * stream,
                                 uint8_t              index)
{
  // ---------------------------------------------------------------------------
  // "\r\n"
  // ---------------------------------------------------------------------------
  if (separator == RAT_UART_CARRIER_RETURN_AND_NEW_LINE) {
    if (index > 0) {
      if ((stream[index]     == '\n') &&
          (stream[index - 1] == '\r')) {
        return true;
      } else {
        return false;
      }
    } else {
      return false;
    }

  // ---------------------------------------------------------------------------
  // '\r'
  // ---------------------------------------------------------------------------
  } else if (separator == RAT_UART_CARRIER_RETURN) {
    if (stream[index] == '\r') {
      return true;
    } else {
      return false;
    }

  // ---------------------------------------------------------------------------
  // '\n'
  // ---------------------------------------------------------------------------
  } else if (separator == RAT_UART_NEW_LINE) {
    if (stream[index] == '\n') {
      return true;
    } else {
      return false;
    }

  // ---------------------------------------------------------------------------
  // Invalid, always return false
  // ---------------------------------------------------------------------------
  } else {
    return false;
  }
}

// -----------------------------------------------------------------------------
// Remove separator
// -----------------------------------------------------------------------------
static void rat_remove_separator (rat_uart_separator   separator,
                                  char               * stream,
                                  uint8_t            * current_index)
{
  // ---------------------------------------------------------------------------
  // Auxiliary variables
  // ---------------------------------------------------------------------------
  uint8_t new_index = *current_index;
  
  // ---------------------------------------------------------------------------
  // "\r\n"
  // ---------------------------------------------------------------------------
  if (separator == RAT_UART_CARRIER_RETURN_AND_NEW_LINE) {
    if (new_index > 1) {
      stream[new_index]     = '\0';
      stream[new_index - 1] = '\0';
    
      new_index = new_index - 2;
    }
    
  // ---------------------------------------------------------------------------
  // '\r' or '\n'
  // ---------------------------------------------------------------------------
  } else if ((separator == RAT_UART_CARRIER_RETURN) ||
             (separator == RAT_UART_NEW_LINE)) {
    if (new_index > 0) {
      stream[new_index] = '\0';
    
      new_index = new_index - 1;
    }
  }
  
  // ---------------------------------------------------------------------------
  // Set the new index
  // ---------------------------------------------------------------------------
  *current_index = new_index;
}

// -----------------------------------------------------------------------------
// Send a request
// -----------------------------------------------------------------------------
void rat_uart_request (rat_uart_separator   leading_separator,
                       rat_uart_separator   trailing_separator,
                       char               * request)
{
  // ---------------------------------------------------------------------------
  // Auxiliary variables
  // ---------------------------------------------------------------------------
  uint8_t character_index = 0;

  // ---------------------------------------------------------------------------
  // Write leading separator
  // ---------------------------------------------------------------------------
  if (leading_separator != RAT_UART_NONE) {
    rat_uart_write_separator(leading_separator);
  }

  // ---------------------------------------------------------------------------
  // Send request
  // ---------------------------------------------------------------------------
  while (request[character_index] != '\0') {
    (void)UART1_Write(request[character_index]);
    
    character_index++;
    
    if (character_index == (RAT_UART_BUFFER_SIZE - 1)) {
      break;
    }
  }

  // ---------------------------------------------------------------------------
  // Write trailing separator
  // ---------------------------------------------------------------------------
  if (trailing_separator != RAT_UART_NONE) {
    rat_uart_write_separator(trailing_separator);
  }
}

// -----------------------------------------------------------------------------
// Receive response without a value
// -----------------------------------------------------------------------------
void rat_uart_response (rat_uart_separator   leading_separator,
                        rat_uart_separator   trailing_separator,
                        char               * response)
{
  // ---------------------------------------------------------------------------
  // Auxiliary variables
  // ---------------------------------------------------------------------------
  uint8_t character_index     = 0;
  uint8_t trailing_separators = 0;
  
  // ---------------------------------------------------------------------------
  // Leading separator
  // ---------------------------------------------------------------------------
  if (leading_separator != RAT_UART_NONE) {
    rat_uart_read_separator(leading_separator);
  }

  // ---------------------------------------------------------------------------
  // Read response code
  // ---------------------------------------------------------------------------
  while ((character_index < RAT_UART_BUFFER_SIZE) &&
         (trailing_separators < 1)) {
    while (UART1_Data_Ready() == 0) {
      // Wait for a new character
    }

    response[character_index] = UART1_Read();
    
    if (rat_check_separator(trailing_separator, response, character_index)) {
      rat_remove_separator(trailing_separator, response, &character_index);
      
      trailing_separators++;
    }
    
    character_index++;
  }
}

// -----------------------------------------------------------------------------
// Receive response with a value
// -----------------------------------------------------------------------------
void rat_uart_response_with_value (rat_uart_separator   leading_separator,
                                   rat_uart_separator   trailing_separator,
                                   char               * response)
{
  // ---------------------------------------------------------------------------
  // Auxiliary variables
  // ---------------------------------------------------------------------------
  uint8_t character_index     = 0;
  uint8_t trailing_separators = 0;

  // ---------------------------------------------------------------------------
  // Leading separator
  // ---------------------------------------------------------------------------
  if (leading_separator != RAT_UART_NONE) {
    rat_uart_read_separator(leading_separator);
  }

  // ---------------------------------------------------------------------------
  // Read value and response code
  // ---------------------------------------------------------------------------
  while ((character_index < RAT_UART_BUFFER_SIZE) && 
         (trailing_separators < 2)) {
    while (UART1_Data_Ready() == 0) {
      // Wait for a new character
    }

    response[character_index] = UART1_Read();

    if (rat_check_separator(trailing_separator, response, character_index)) {
      rat_remove_separator(trailing_separator, response, &character_index);

      trailing_separators++;
    }

    character_index++;
  }
}