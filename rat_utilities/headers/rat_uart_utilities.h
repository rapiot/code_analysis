// -----------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2020 - 2024 Rapiot Open Hardware Project
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// UART Utilities Header File
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// -----------------------------------------------------------------------------
// Constants
// -----------------------------------------------------------------------------
#define RAT_UART_BUFFER_SIZE 64

// -----------------------------------------------------------------------------
// UART separators
//
// Carrier return - '\r' - 0x0D (13)
// New line       - '\n' - 0x0A (10)
// -----------------------------------------------------------------------------
typedef enum rat_uart_separators {
  RAT_UART_CARRIER_RETURN_AND_NEW_LINE,   // "\r\n"
  RAT_UART_CARRIER_RETURN,                // '\r'
  RAT_UART_NEW_LINE,                      // '\n'
  RAT_UART_NONE}                          // No separator
rat_uart_separator;

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Clear the UART buffer
// -----------------------------------------------------------------------------
void rat_uart_clear_buffer (void);

// -----------------------------------------------------------------------------
// Send request
// -----------------------------------------------------------------------------
void rat_uart_request (rat_uart_separator   leading_separator,
                       rat_uart_separator   trailing_separator,
                       char               * request);

// -----------------------------------------------------------------------------
// Receive response without a value
// -----------------------------------------------------------------------------
void rat_uart_response (rat_uart_separator   leading_separator,
                        rat_uart_separator   trailing_separator,
                        char               * response);

// -----------------------------------------------------------------------------
// Receive response with a value
// -----------------------------------------------------------------------------
void rat_uart_response_with_value (rat_uart_separator   leading_separator,
                                   rat_uart_separator   trailing_separator,
                                   char               * response);