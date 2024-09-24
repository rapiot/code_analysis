// -----------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2020 - 2024 Rapiot Open Hardware Project
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Indoor Monitor Application
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../../rat_utilities/headers/rat_math_utilities.h"
#include "../../rat_utilities/headers/rat_pic_utilities.h"
#include "../../rat_sensors/headers/rat_sensirion_sht4x.h"
#include "../../rat_radio_modules/headers/rat_lorawan.h"
#include "../../rat_radio_modules/headers/rat_rakwireless_rakx.h"

// -----------------------------------------------------------------------------
// Constants
// -----------------------------------------------------------------------------
#define APP_STABILIZATION_DELAY 1000    // 1,000 ms
#define APP_TIMER_CONSTANT 4            // 4 seconds
#define APP_SLEEP_CYCLES 15             // 15 minutes
#define APP_SLEEP_CYCLES_THRESHOLD 96   // 96 * 15 = 24 * 60 = 24 hours
#define APP_UPLINK_DATA_SIZE   5        // 3 bytes for temperature and
                                        // 2 bytes for humidity
#define APP_DOWNLINK_DATA_SIZE 1        // 1 byte for transmission interval

// -----------------------------------------------------------------------------
// Global variables
// -----------------------------------------------------------------------------
uint8_t  gbl_sleep_cycles;
uint32_t gbl_sleep_cycles_counter;

// -----------------------------------------------------------------------------
// Auxiliary functions
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Wakeup
// -----------------------------------------------------------------------------
bool app_wakeup (void)
{
  // ---------------------------------------------------------------------------
  // Note that one sleep cycle is one minute,
  // but the value of the interrupt counter depends on the timer constant
  //
  // By default, the timer constant is four seconds
  // ---------------------------------------------------------------------------
  if (rat_interrupt_counter() %
      ( APP_SLEEP_CYCLES * ( 60 / APP_TIMER_CONSTANT ) ) == 0) {
    return true;
  } else {
    return false;
  }
}

// -----------------------------------------------------------------------------
// Sleep
// -----------------------------------------------------------------------------
void app_sleep (void)
{
  while (true) {
    rat_sleep();

    if (app_wakeup()) {
      break;
    }
  }
}

// -----------------------------------------------------------------------------
// Application init
//
// This function is called automatically only once when the application starts
// -----------------------------------------------------------------------------
void app_init (void)
{
  // ---------------------------------------------------------------------------
  // Init global variables
  // ---------------------------------------------------------------------------
  gbl_sleep_cycles         = APP_SLEEP_CYCLES;
  gbl_sleep_cycles_counter = 0;

  // ---------------------------------------------------------------------------
  // Init the MCU
  // ---------------------------------------------------------------------------
  rat_mcu_init();
  
  // ---------------------------------------------------------------------------
  // Init the interrupt counter
  // ---------------------------------------------------------------------------
  rat_init_interrupt_counter();

  // ---------------------------------------------------------------------------
  // Init the sensor
  // ---------------------------------------------------------------------------
  rat_humidity_sensor_init();

  // ---------------------------------------------------------------------------
  // Init and reset the radio module
  // ---------------------------------------------------------------------------
  rat_radio_module_init();
  rat_radio_module_reset();
  
  // ---------------------------------------------------------------------------
  // Init the interrupt counter
  // ---------------------------------------------------------------------------
  rat_init_interrupt_counter();
  
  // ---------------------------------------------------------------------------
  // Stabilization delay
  // ---------------------------------------------------------------------------
  rat_delay(APP_STABILIZATION_DELAY);

  // ---------------------------------------------------------------------------
  // Wait for an interrupt from the timer
  // ---------------------------------------------------------------------------
  rat_wait_interrupt();

  // ---------------------------------------------------------------------------
  // LoRaWAN setup
  // ---------------------------------------------------------------------------
  if (!rat_radio_module_set_abp_mode()) {
      rat_reset();
  }
  
  if (!rat_radio_module_set_abp_parameters()) {
      rat_reset();
  }

  // ---------------------------------------------------------------------------
  // Wait for an interrupt from the timer
  // ---------------------------------------------------------------------------
  rat_wait_interrupt();
}

// -----------------------------------------------------------------------------
// Application task
//
// This function is called automatically indefinitely
// -----------------------------------------------------------------------------
void app_task (void)
{
  // ---------------------------------------------------------------------------
  // Auxiliary variables
  // ---------------------------------------------------------------------------
  bool uplink_status    = false;
  bool downlink_status  = false;

  uint8_t uplink_data [APP_UPLINK_DATA_SIZE];
  uint8_t downlink_data [APP_DOWNLINK_DATA_SIZE];

  float temperature = 0;
  float humidity    = 0;

  // ---------------------------------------------------------------------------
  // Measure
  // ---------------------------------------------------------------------------
  if (!rat_humidity_sensor_measure(&temperature, &humidity)) {
    rat_reset();
  }

  // ---------------------------------------------------------------------------
  // Create the payload
  // ---------------------------------------------------------------------------

  // ---------------------------------------------------------------------------
  // Temperature
  // ---------------------------------------------------------------------------
  uplink_data[0] = rat_convert_twos_complement(temperature,2) >> 16;
  uplink_data[1] = rat_convert_twos_complement(temperature,2) >> 8;
  uplink_data[2] = rat_convert_twos_complement(temperature,2) % 256;

  // ---------------------------------------------------------------------------
  // Humidity
  // ---------------------------------------------------------------------------
  uplink_data[3] = rat_convert_twos_complement(humidity,1) >> 8;
  uplink_data[4] = rat_convert_twos_complement(humidity,1) % 256;

  // ---------------------------------------------------------------------------
  // Transmit
  // ---------------------------------------------------------------------------
  rat_radio_module_transmit(APP_UPLINK_DATA_SIZE,     // Uplink data length
                            uplink_data,              // Uplink data
                            &uplink_status,           // Uplink status

                            APP_DOWNLINK_DATA_SIZE,   // Downlink data length
                            downlink_data,            // Downlink data
                            &downlink_status);        // Downlink status
                            
  // ---------------------------------------------------------------------------
  // Set the amount of the sleep cycles
  // ---------------------------------------------------------------------------
  if (downlink_status) {
    gbl_sleep_cycles = downlink_data[0];
  }

  // ---------------------------------------------------------------------------
  // Sleep
  // ---------------------------------------------------------------------------
  app_sleep();
}

// -----------------------------------------------------------------------------
// The main program
// -----------------------------------------------------------------------------
void main (void) {
  // ---------------------------------------------------------------------------
  // Run the application init only once
  // ---------------------------------------------------------------------------
  app_init();

  // ---------------------------------------------------------------------------
  // Run the application task indefinitely until reset, shutdown, or restart
  // ---------------------------------------------------------------------------
  while (true) {
    app_task();
  }
}