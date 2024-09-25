// -----------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2020 - 2024 Rapiot Open Hardware Project
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Math Utilities Source File
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../../rat_utilities/headers/rat_math_utilities.h"

// -----------------------------------------------------------------------------
// Global variables
// -----------------------------------------------------------------------------
uint32_t g_interrupt_counter = 0;

// -----------------------------------------------------------------------------
// Conversions
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Hex to char
//
// Digits 0 ... 9 are 0x30 ... 0x39
// Digits A ... F are 0x41 ... 0x46
// -----------------------------------------------------------------------------
char rat_hex_to_char (uint8_t value)
{
  char result = '\0';

  if (value < 10) {
    result = value + 0x30;
  } else {
    result = value + 0x37;
  }

  return result;
}

// -----------------------------------------------------------------------------
// Char to hex
//
// Digits 0 ... 9 are 0x30 ... 0x39
// Digits A ... F are 0x41 ... 0x46
// -----------------------------------------------------------------------------
uint8_t rat_char_to_hex (char value)
{
  uint8_t result = 0x00;

  if (isdigit(value) != 0) {
    result = value - 0x30;
  } else {
    result = value - 0x37;
  }

  return result;
}

// -----------------------------------------------------------------------------
// Hex array to char array
//
//   hex_array        - The array of hex values
//   hex_array_length - The length of the array of hex values
//   char_array       - The array of chars
//   char_array_index - The start index of the array of chars
// -----------------------------------------------------------------------------
void rat_hex_array_to_char_array (uint8_t * hex_array,
                                  uint8_t   hex_array_length,
                                  char    * char_array,
                                  uint8_t   char_array_index)
{
  char msb_character = '\0';
  char lsb_character = '\0';
  
  uint8_t hex_index = 0;

  for (hex_index = 0;hex_index < hex_array_length;++hex_index) {
    msb_character = rat_hex_to_char(hex_array[hex_index]) >> 4;
    lsb_character = rat_hex_to_char(hex_array[hex_index]) % 16;
    
    char_array[char_array_index + (2 * hex_index)]     = msb_character;
    char_array[char_array_index + (2 * hex_index) + 1] = lsb_character;
  }
}

// -----------------------------------------------------------------------------
// Char array to hex array
//
//   char_array        - The array of chars
//   char_array_index  - The start index of the array of chars
//   char_array_length - The length of the char array
//   hex_array         - The array of hex values
// -----------------------------------------------------------------------------
void rat_char_array_to_hex_array (char    * char_array,
                                  uint8_t   char_array_index,
                                  uint8_t   char_array_length,
                                  uint8_t * hex_array)
{
  uint8_t char_index = 0;
  
  uint8_t msb_index = 0;
  uint8_t lsb_index = 0;
  
  uint8_t msb_hex = 0;
  uint8_t lsb_hex = 0;

  for (char_index = 0;char_index < (char_array_length / 2);++char_index) {
    msb_index = char_array_index + (2 * char_index);
    lsb_index = char_array_index + (2 * char_index) + 1;
    
    msb_hex = rat_char_to_hex(char_array[msb_index]) << 4;
    lsb_hex = rat_char_to_hex(char_array[lsb_index]);
    
    hex_array[n] = msb_hex + lsb_hex;
  }
}

// -----------------------------------------------------------------------------
// Clear a string
// -----------------------------------------------------------------------------
void rat_clear_string (char     * char_array,
                       uint16_t   length)
{
  uint16_t counter = 0;

  for (counter = 0;counter < length;++counter) {
    char_array[counter] = '\0';
  }
}

// -----------------------------------------------------------------------------
// Two's complement, 8 bit unsigned integer
// -----------------------------------------------------------------------------
uint8_t rat_twos_complement_short (uint8_t value)
{
  uint8_t result = 0x00;

  uint8_t mask = 0xFF;

  result = value ^ mask;

  result++;

  return result;
}

// -----------------------------------------------------------------------------
// Two's complement, 16 bit unsigned integer
// -----------------------------------------------------------------------------
uint16_t rat_twos_complement (uint16_t value)
{
  uint16_t result = 0x0000;

  uint16_t mask = 0xFFFF;

  result = value ^ mask;

  result++;

  return result;
}

// -----------------------------------------------------------------------------
// Two's complement, 32 bit unsigned integer
// -----------------------------------------------------------------------------
uint32_t rat_twos_complement_long (uint32_t value)
{
  uint32_t result = 0x00000000;

  uint32_t mask = 0xFFFFFFFF;

  result = value ^ mask;

  result++;

  return result;
}

// -----------------------------------------------------------------------------
// Convert a float to a two's complement (8 bit unsigned integer)
// -----------------------------------------------------------------------------
uint8_t rat_convert_twos_complement_short (float   value,
                                           uint8_t decimals)
{
  uint8_t mask   = 0xFF;
  uint8_t result = 0x00;

  if (value < 0) {
    result = - value * pow(10, decimals);
  } else {
    result =   value * pow(10, decimals);
  }

  if (value < 0) {
    result = result ^ mask;

    result++;
  }

  return result;
}

// -----------------------------------------------------------------------------
// Convert a float to a two's complement (16 bit unsigned integer)
// -----------------------------------------------------------------------------
uint16_t rat_convert_twos_complement (float   value,
                                      uint8_t decimals)
{
  uint16_t mask   = 0xFFFF;
  uint16_t result = 0x0000;

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

// -----------------------------------------------------------------------------
// Convert a float to a two's complement (32 bit unsigned integer)
// -----------------------------------------------------------------------------
uint32_t rat_convert_twos_complement_long (float   value,
                                           uint8_t decimals)
{
  uint32_t mask   = 0xFFFFFFFF;
  uint32_t result = 0x00000000;

  if (value < 0) {
    result = - value * pow(10, decimals);
  } else {
    result =   value * pow(10, decimals);
  }

  if (value < 0) {
    result = result ^ mask;

    result++;
  }

  return result;
}

// -----------------------------------------------------------------------------
// Generic CRC algorithm
// -----------------------------------------------------------------------------
uint8_t rat_calculate_crc (uint16_t value,
                           uint8_t  initialisation,
                           uint8_t  polynomial)
{
  // ---------------------------------------------------------------------------
  // Auxiliary variables
  // ---------------------------------------------------------------------------
  uint8_t counter_byte = 0x00;
  uint8_t counter_bit  = 0x00;

  // ---------------------------------------------------------------------------
  // Checksum
  // ---------------------------------------------------------------------------
  uint8_t checksum = 0x00;

  // ---------------------------------------------------------------------------
  // Data
  // ---------------------------------------------------------------------------
  uint8_t raw_data [2] = {0x00};

  // ---------------------------------------------------------------------------
  // Initialisation
  // ---------------------------------------------------------------------------
  checksum = initialisation;

  raw_data[0] = value >> 8;
  raw_data[1] = value % 256;

  // ---------------------------------------------------------------------------
  // Calculate the checksum
  // ---------------------------------------------------------------------------
  for (counter_byte = 0;counter_byte < 2;++counter_byte) {
    checksum ^= (raw_data[counter_byte]);

    for (counter_bit = 8;counter_bit > 0;--counter_bit) {
      if ((checksum & 0x80) != 0x00) {
        checksum = (checksum << 1) ^ polynomial;
      } else {
        checksum = (checksum << 1);
      }
    }
  }

  // ---------------------------------------------------------------------------
  // Return
  // ---------------------------------------------------------------------------
  return checksum;
}

// -----------------------------------------------------------------------------
// String compare
// -----------------------------------------------------------------------------
bool rat_string_compare (char * a,
                         char * b)
{
  uint8_t length_a = 0;
  uint8_t length_b = 0;
  uint8_t index    = 0;
  
  bool result = true;
  
  length_a = strlen(a);
  length_b = strlen(b);

  if (length_a < length_b) {
    result = false;
  } else {
    for (index = 0;index < length_b;++index) {
      if (a[index] != b[index]) {
        result = false;
      
        break;
      }
    }
  }
  
  return result;
}

// -----------------------------------------------------------------------------
// String compare (reverse)
// -----------------------------------------------------------------------------
bool rat_string_compare_reverse (char * a,
                                 char * b)
{
  uint8_t length_a = 0;
  uint8_t length_b = 0;
  uint8_t index    = 0;

  bool result = true;

  length_a = strlen(a);
  length_b = strlen(b);

  if (length_a < length_b) {
    result = false;
  } else {
    for (index = 0;index < length_b;++index) {
      if (a[length_a - 1 - index] != b[length_b - 1 - index]) {
        result = false;

        break;
      }
    }
  }

  return result;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool rat_string_find_char (char    * a,
                           char      b,
                           uint8_t * location)
{
  bool result = false;
  
  uint8_t index = 0;
  
  while (a[index] != '\0') {
    if (a[index] == b) {
      *location = index;
      
      result = true;
      
      break;
    }

    index++;
  }
  
  return result;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool rat_string_find (char * a,
                      char * b)
{
  if (strstr(a, b) == NULL) {
    return false;
  } else {
    return true;
  }
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void rat_string_sub (char * a,
                     char * b,
                     uint8_t index,
                     uint8_t length)
{
  uint8_t counter = 0;
  
  for (counter = index;counter < (index + length);++counter) {
    b[counter - index] = a[counter];
  }
}

// -----------------------------------------------------------------------------
// Wait an interrupt
// -----------------------------------------------------------------------------
void rat_wait_interrupt (void)
{
  uint32_t interrupt_counter = g_interrupt_counter;

  while (interrupt_counter == g_interrupt_counter) {
    // Wait until the interrupt
  }
}

// -----------------------------------------------------------------------------
// Wait interrupts
// -----------------------------------------------------------------------------
void rat_wait_interrupts (uint8_t interrupts)
{
  uint8_t counter = 0;
  
  for (counter = 0;counter < interrupts;++counter) {
    rat_wait_interrupt();
  }
}

// -----------------------------------------------------------------------------
// Init the interrupt counter
// -----------------------------------------------------------------------------
void rat_init_interrupt_counter (void)
{
  g_interrupt_counter = 0;
}

// -----------------------------------------------------------------------------
// Get the interrupt counter
// -----------------------------------------------------------------------------
uint32_t rat_interrupt_counter (void)
{
  return g_interrupt_counter;
}

// -----------------------------------------------------------------------------
// Interrupt routine implementation
//
// Note that the implementation of the interrupt routine will be called
// automatically in case of an interrupt. However, it is up to the application
// to check which interrupt has been generated. It is also up to the application
// to clear interrupts unless otherwise stated.
// -----------------------------------------------------------------------------
void interrupt (void) {
  // Check timer 1 interrupt flag
  if (PIR1.TMR1IF == 0b1) {
    // Clear timer 1 interrupt flag (must be cleared by the software)
    PIR1.TMR1IF = 0b0;

    g_interrupt_counter++;
  }
}