// ----------------------------------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2020 - 2024 Rapiot Open Hardware Project
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Math Utilities Source File
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../../rat_utilities/headers/rat_math_utilities.h"

// ----------------------------------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------------------------------
uint32_t g_interrupt_counter;

// ----------------------------------------------------------------------------------------------------
// Conversions
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Hex to char
//
// Digits 0 ... 9 are 0x30 ... 0x39
// Digits A ... F are 0x41 ... 0x46
// ----------------------------------------------------------------------------------------------------
char rat_hex_to_char (uint8_t value)
{
  char result;

  if (value < 10) {
    result = value + 0x30;
  } else {
    result = value + 0x37;
  }

  return result;
}

// ----------------------------------------------------------------------------------------------------
// Char to hex
//
// Digits 0 ... 9 are 0x30 ... 0x39
// Digits A ... F are 0x41 ... 0x46
// ----------------------------------------------------------------------------------------------------
uint8_t rat_char_to_hex (char value)
{
  uint8_t result;

  if (isdigit(value)) {
    result = value - 0x30;
  } else {
    result = value - 0x37;
  }

  return result;
}

// ----------------------------------------------------------------------------------------------------
// Hex array to char array
//
//   hex_array        - The array of hex values
//   hex_array_length - The length of the array of hex values
//   char_array       - The array of chars
//   char_array_index - The start index of the array of chars
// ----------------------------------------------------------------------------------------------------
void rat_hex_array_to_char_array (uint8_t * hex_array,
                                  uint8_t   hex_array_length,
                                  char    * char_array,
                                  uint8_t   char_array_index)
{
  uint8_t n;

  for (n = 0;n < hex_array_length;n++) {
    char_array[char_array_index + 2 * n]     = rat_hex_to_char(hex_array[n] >> 4);
    char_array[char_array_index + 2 * n + 1] = rat_hex_to_char(hex_array[n] % 16);
  }
}

// ----------------------------------------------------------------------------------------------------
// Char array to hex array
//
//   char_array        - The array of chars
//   char_array_index  - The start index of the array of chars
//   char_array_length - The length of the char array
//   hex_array         - The array of hex values
// ----------------------------------------------------------------------------------------------------
void rat_char_array_to_hex_array (char    * char_array,
                                  uint8_t   char_array_index,
                                  uint8_t   char_array_length,
                                  uint8_t * hex_array)
{
  uint8_t n;

  for (n = 0;n < char_array_length / 2;n++) {
    hex_array[n] = ( rat_char_to_hex(char_array[char_array_index + 2 * n]) << 4 ) +
                     rat_char_to_hex(char_array[char_array_index + 2 * n + 1]);
  }
}

// ----------------------------------------------------------------------------------------------------
// Clear a string
// ----------------------------------------------------------------------------------------------------
void rat_clear_string (char     * char_array,
                       uint16_t   length)
{
  uint16_t counter;

  for (counter = 0;counter < length;counter++) {
    char_array[counter] = '\0';
  }
}

// ----------------------------------------------------------------------------------------------------
// Two's complement, 8 bit unsigned integer
// ----------------------------------------------------------------------------------------------------
uint8_t rat_twos_complement_short (uint8_t value)
{
  uint8_t result;

  uint8_t mask = 0xFF;

  result = value ^ mask;

  result++;

  return result;
}

// ----------------------------------------------------------------------------------------------------
// Two's complement, 16 bit unsigned integer
// ----------------------------------------------------------------------------------------------------
uint16_t rat_twos_complement (uint16_t value)
{
  uint16_t result;

  uint16_t mask = 0xFFFF;

  result = value ^ mask;

  result++;

  return result;
}

// ----------------------------------------------------------------------------------------------------
// Two's complement, 32 bit unsigned integer
// ----------------------------------------------------------------------------------------------------
uint32_t rat_twos_complement_long (uint32_t value)
{
  uint32_t result;

  uint32_t mask = 0xFFFFFFFF;

  result = value ^ mask;

  result++;

  return result;
}

// ----------------------------------------------------------------------------------------------------
// Convert a float to a two's complement (8 bit unsigned integer)
// ----------------------------------------------------------------------------------------------------
uint8_t rat_convert_twos_complement_short (float   value,
                                           uint8_t decimals)
{
  uint8_t mask = 0xFF;
  uint8_t result;

  if (value < 0) {
    result = - value * pow(10,decimals);
  } else {
    result =   value * pow(10,decimals);
  }

  if (value < 0) {
    result = result ^ mask;

    result++;
  }

  return result;
}

// ----------------------------------------------------------------------------------------------------
// Convert a float to a two's complement (16 bit unsigned integer)
// ----------------------------------------------------------------------------------------------------
uint16_t rat_convert_twos_complement (float   value,
                                      uint8_t decimals)
{
  uint16_t mask = 0xFFFF;
  uint16_t result;

  if (value < 0) {
    result = - value * pow(10,decimals);
  } else {
    result =   value * pow(10,decimals);
  }

  if (value < 0) {
    result = result ^ mask;

    result++;
  }

  return result;
}

// ----------------------------------------------------------------------------------------------------
// Convert a float to a two's complement (32 bit unsigned integer)
// ----------------------------------------------------------------------------------------------------
uint32_t rat_convert_twos_complement_long (float   value,
                                           uint8_t decimals)
{
  uint32_t mask = 0xFFFFFFFF;
  uint32_t result;

  if (value < 0) {
    result = - value * pow(10,decimals);
  } else {
    result =   value * pow(10,decimals);
  }

  if (value < 0) {
    result = result ^ mask;

    result++;
  }

  return result;
}

// ----------------------------------------------------------------------------------------------------
// Generic CRC algorithm
// ----------------------------------------------------------------------------------------------------
uint8_t rat_calculate_crc (uint16_t value,
                           uint8_t  initialisation,
                           uint8_t  polynomial)
{
  // --------------------------------------------------------------------------------------------------
  // Auxiliary variables
  // --------------------------------------------------------------------------------------------------
  uint8_t counter_byte;
  uint8_t counter_bit;

  // --------------------------------------------------------------------------------------------------
  // Checksum
  // --------------------------------------------------------------------------------------------------
  uint8_t checksum;

  // --------------------------------------------------------------------------------------------------
  // Data
  // --------------------------------------------------------------------------------------------------
  uint8_t raw_data [2];

  // --------------------------------------------------------------------------------------------------
  // Initialisation
  // --------------------------------------------------------------------------------------------------
  checksum = initialisation;

  raw_data[0] = value >> 8;
  raw_data[1] = value % 256;

  // --------------------------------------------------------------------------------------------------
  // Calculate the checksum
  // --------------------------------------------------------------------------------------------------
  for (counter_byte = 0;counter_byte < 2;counter_byte++) {
    checksum ^= (raw_data[counter_byte]);

    for (counter_bit = 8;counter_bit > 0;--counter_bit) {
      if (checksum & 0x80) {
        checksum = (checksum << 1) ^ polynomial;
      } else {
        checksum = (checksum << 1);
      }
    }
  }

  // --------------------------------------------------------------------------------------------------
  // Return
  // --------------------------------------------------------------------------------------------------
  return checksum;
}

// ----------------------------------------------------------------------------------------------------
// String compare
// ----------------------------------------------------------------------------------------------------
bool rat_string_compare (char * a,
                         char * b)
{
  uint8_t length_a;
  uint8_t length_b;
  uint8_t n;
  
  bool result = true;
  
  length_a = strlen(a);
  length_b = strlen(b);

  if (length_a < length_b) {
    result = false;
  } else {
    for (n = 0;n < length_b;n++) {
      if (a[n] != b[n]) {
        result = false;
      
        break;
      }
    }
  }
  
  return result;
}

// ----------------------------------------------------------------------------------------------------
// String compare (reverse)
// ----------------------------------------------------------------------------------------------------
bool rat_string_compare_reverse (char * a,
                                 char * b)
{
  uint8_t length_a;
  uint8_t length_b;
  uint8_t n;

  bool result = true;

  length_a = strlen(a);
  length_b = strlen(b);

  if (length_a < length_b) {
    result = false;
  } else {
    for (n = 0;n < length_b;n++) {
      if (a[length_a - 1 - n] != b[length_b - 1 - n]) {
        result = false;

        break;
      }
    }
  }

  return result;
}

// ----------------------------------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------------------------------
bool rat_string_find_char (char    * a,
                           char      b,
                           uint8_t * location)
{
  bool result = false;
  
  uint8_t n = 0;
  
  while (a[n] != '\0') {
    if (a[n] == b) {
      *location = n;
      
      result = true;
      
      break;
    }

    n++;
  }
  
  return result;
}

// ----------------------------------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------------------------------
bool rat_string_find (char * a,
                      char * b)
{
  if (strstr(a,b) == NULL) {
    return false;
  } else {
    return true;
  }
}


// ----------------------------------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------------------------------
void rat_string_sub (char * a,
                     char * b,
                     uint8_t index,
                     uint8_t length)
{
  uint8_t n;
  
  for (n = index;n < index + length;n++) {
    b[n - index] = a[n];
  }
}

// ----------------------------------------------------------------------------------------------------
// Wait an interrupt
// ----------------------------------------------------------------------------------------------------
void rat_wait_interrupt (void)
{
  uint32_t interrupt_counter = g_interrupt_counter;

  while (interrupt_counter == g_interrupt_counter) {
    // Wait until the interrupt
  }
}

// ----------------------------------------------------------------------------------------------------
// Wait interrupts
// ----------------------------------------------------------------------------------------------------
void rat_wait_interrupts (uint8_t interrupts)
{
  uint8_t n;
  
  for (n = 0;n < interrupts;n++) {
    rat_wait_interrupt();
  }
}

// ----------------------------------------------------------------------------------------------------
// Init the interrupt counter
// ----------------------------------------------------------------------------------------------------
void rat_init_interrupt_counter (void)
{
  g_interrupt_counter = 0;
}

// ----------------------------------------------------------------------------------------------------
// Get the interrupt counter
// ----------------------------------------------------------------------------------------------------
uint32_t rat_interrupt_counter (void)
{
  return g_interrupt_counter;
}

// ----------------------------------------------------------------------------------------------------
// Interrupt routine implementation
//
// Note that the implementation of the interrupt routine will be called automatically in case of
// an interrupt. However, it is up to the application to check which interrupt has been generated.
// It is also up to the application to clear interrupts unless otherwise stated.
// ----------------------------------------------------------------------------------------------------
void interrupt (void) {
  if (PIR1.TMR1IF == 0b1) {    // Check timer 1 interrupt flag
    PIR1.TMR1IF = 0b0;         // Clear timer 1 interrupt flag (must be cleared by the software)

    g_interrupt_counter++;
  }
}