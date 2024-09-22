// ----------------------------------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0  License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2020 - 2022 Rapiot Open Hardware Project
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Debug Utilities Source File
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------------------------------
#include <stdint.h>

#include "../../rat_application/headers/defines.h"

// ----------------------------------------------------------------------------------------------------
// Conditional compilation
// ----------------------------------------------------------------------------------------------------
#ifdef TEST_MODE

// ----------------------------------------------------------------------------------------------------
// Clear string
// ----------------------------------------------------------------------------------------------------
void rat_debug_clear_string (char    * debug_string,
                             uint8_t   debug_string_length)
{
  uint8_t n;

  for (n = 0;n < debug_string_length;n++) {
    debug_string[n] = '\0';
  }
}

// ----------------------------------------------------------------------------------------------------
// Write new line
// ----------------------------------------------------------------------------------------------------
void rat_debug_write_new_line (void)
{
  Soft_UART_Write('\r');
  Soft_UART_Write('\n');
}

// ----------------------------------------------------------------------------------------------------
// Write separator
// ----------------------------------------------------------------------------------------------------
void rat_debug_write_separator (void)
{
  char separator_string [] = "# --------------------------------------------------------------------------------";

  uint8_t n = 0;

  rat_debug_write_new_line();

  while (1) {
    if (separator_string[n] == '\0') {
      break;
    } else {
      Soft_UART_Write(separator_string[n]);
    }

    n++;
  }
}

// ----------------------------------------------------------------------------------------------------
// Write start line
// ----------------------------------------------------------------------------------------------------
void rat_debug_write_start_line (void)
{
  char start_string [] = "# ";

  uint8_t n = 0;

  rat_debug_write_new_line();

  while (1) {
    if (start_string[n] == '\0') {
      break;
    } else {
      Soft_UART_Write(start_string[n]);
    }

    n++;
  }
}

// ----------------------------------------------------------------------------------------------------
// Write string (with null character)
// ----------------------------------------------------------------------------------------------------
void rat_debug_write_string (char * debug_string)
{
  while (*debug_string) {
    Soft_UART_Write(*debug_string++);
  }
}

// ----------------------------------------------------------------------------------------------------
// Write string (without null character)
// ----------------------------------------------------------------------------------------------------
void rat_debug_write_string_length (char    * debug_string,
                                    uint8_t   debug_string_length)
{
  uint8_t n;

  for (n = 0;n < debug_string_length;n++) {
    Soft_UART_Write(debug_string[n]);
  }
}


// ----------------------------------------------------------------------------------------------------
// Write float
// ----------------------------------------------------------------------------------------------------
void rat_debug_write_float (float   debug_float,
                            uint8_t debug_length)
{
  char debug_string [15];

  FloatToStr_FixLen(debug_float,debug_string,debug_length);

  rat_debug_write_string(debug_string);
}

// ----------------------------------------------------------------------------------------------------
// Write boolean
// ----------------------------------------------------------------------------------------------------
void rat_debug_write_boolean (uint8_t debug_value)
{
  if (debug_value == 0) {
    rat_debug_write_string("FALSE");
  } else if (debug_value == 1) {
    rat_debug_write_string("TRUE");
  } else {
    rat_debug_write_string("UNKNOWN");
  }
}

// ----------------------------------------------------------------------------------------------------
// Write unsigned int short in decimal
// ----------------------------------------------------------------------------------------------------
void rat_debug_write_unsigned_int_short (uint8_t debug_value)
{
  char debug_string [4];

  ByteToStr(debug_value,debug_string);

  rat_debug_write_string(LTrim(debug_string));
}

// ----------------------------------------------------------------------------------------------------
// Write unsigned int short in hex
// ----------------------------------------------------------------------------------------------------
void rat_debug_write_unsigned_int_short_hex (uint8_t debug_value)
{
  char debug_string [3];

  char prefix [] = "x";

  ByteToHex(debug_value,debug_string);

  rat_debug_write_string(prefix);
  rat_debug_write_string(debug_string);
}

// ----------------------------------------------------------------------------------------------------
// Write unsigned int in decimal
// ----------------------------------------------------------------------------------------------------
void rat_debug_write_unsigned_int (uint16_t debug_value)
{
  char debug_string [6];

  WordToStr(debug_value,debug_string);

  rat_debug_write_string(LTrim(debug_string));
}

// ----------------------------------------------------------------------------------------------------
// Write unsigned int in hex
// ----------------------------------------------------------------------------------------------------
void rat_debug_write_unsigned_int_hex (uint16_t debug_value)
{
  char debug_string [5];

  char prefix [] = "x";

  WordToHex(debug_value,debug_string);

  rat_debug_write_string(prefix);
  rat_debug_write_string(debug_string);
}

// ----------------------------------------------------------------------------------------------------
// Write unsigned int long in decimal
// ----------------------------------------------------------------------------------------------------
void rat_debug_write_unsigned_int_long (uint32_t debug_value)
{
  char debug_string [11];

  LongWordToStr(debug_value,debug_string);

  rat_debug_write_string(LTrim(debug_string));
}

// ----------------------------------------------------------------------------------------------------
// Write unsigned int long in hex
// ----------------------------------------------------------------------------------------------------
void rat_debug_write_unsigned_int_long_hex (uint32_t debug_value)
{
  char debug_string [9];

  char prefix [] = "x";

  LongWordToHex(debug_value,debug_string);

  rat_debug_write_string(prefix);
  rat_debug_write_string(debug_string);
}

#endif