// ----------------------------------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2020 - 2023 Rapiot Open Hardware Project
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// MCU Utilities Source File
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------------------------------
#include <stdint.h>
#include <stdbool.h>

#include "../../rat_utilities/headers/rat_mcu_utilities.h"

// ----------------------------------------------------------------------------------------------------
// Delay in milliseconds
// ----------------------------------------------------------------------------------------------------
void rat_delay (uint16_t limit)
{
  // Auxiliary variables
  uint32_t init_cycles;
  uint32_t curr_cycles;
  uint32_t diff_cycles;
  
  uint32_t milliseconds;
  
  // Set the init state
  init_cycles = TMR1H;
  init_cycles = init_cycles << 8;
  init_cycles += TMR1L;
  
  diff_cycles = 0;

  // Calculate the difference in milliseconds
  //
  // Note that the frequency is 32,768 KHz and the prescaler is 4:1
  while (true) {
    curr_cycles = TMR1H;
    curr_cycles = curr_cycles << 8;
    curr_cycles += TMR1L;
    
    if (curr_cycles < init_cycles) {
      diff_cycles += curr_cycles + 65535 - init_cycles;
    } else {
      diff_cycles += curr_cycles - init_cycles;
    }
    
    init_cycles = curr_cycles;
  
    milliseconds = ( diff_cycles * 1000 * 4 ) / 32768;
    
    if (milliseconds >= limit) {
      break;
    }
  }
}

// ----------------------------------------------------------------------------------------------------
// Sleep
// ----------------------------------------------------------------------------------------------------
void rat_sleep (void)
{
  asm SLEEP;
}

// ----------------------------------------------------------------------------------------------------
// Clear watchdog
//
// Note! This is a very dangerous command, because the purpose of the watchdog is to make sure that
// the microcontroller can recover from unexpected situations! Do not use this command unless
// absolutely necessary!
// ----------------------------------------------------------------------------------------------------
void rat_clear_watchdog (void)
{
  asm CLRWDT;
}