// ----------------------------------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2020 - 2024 Rapiot Open Hardware Project
//
// Known limitations :
//
//   There are no known limitations
//
// Known issues :
//
//   There are no known issues.
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Indoor Monitor Application
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../../rat_utilities/headers/rat_utilities.h"                 // Conversions and other math related utilities
#include "../../rat_utilities/headers/rat_mcu_utilities.h"             // Delays, sleep, and watchdog utilities
#include "../../rat_utilities/headers/rat_debug_utilities.h"           // Debugging utilities based on software UART
#include "../../rat_utilities/headers/rat_microchip_pic18f_k22.h"      // Extreme low power 8-bit PIC microcontroller family
#include "../../rat_sensors/headers/rat_sensirion_sht4x.h"             // Temperature and humidity sensor, I2C
#include "../../rat_radio_modules/headers/rat_lorawan.h"               // LoRaWAN definitions
#include "../../rat_radio_modules/headers/rat_rakwireless_rakx.h"      // LoRaWAN radio module, UART

// ----------------------------------------------------------------------------------------------------
// Constants
// ----------------------------------------------------------------------------------------------------
#define APP_STABILIZATION_DELAY 1000   // 1,000 ms

#define APP_TIMER_CONSTANT 4  // 4 seconds

#define APP_SLEEP_CYCLES 15            // 15 minutes

#define APP_SLEEP_CYCLES_THRESHOLD 96   // 96 * 15 = 24 * 60 = 24 hours

#define APP_UPLINK_DATA_SIZE   5
#define APP_DOWNLINK_DATA_SIZE 1

// ----------------------------------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------------------------------
uint8_t  gbl_sleep_cycles;
uint32_t gbl_sleep_cycles_counter;

// ----------------------------------------------------------------------------------------------------
// Auxiliary functions
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Wakeup
// ----------------------------------------------------------------------------------------------------
uint8_t app_wakeup (void)
{
  // --------------------------------------------------------------------------------------------------
  // Note that one sleep cycle is one minute,
  // but the value of the interrupt counter depends on the timer constant
  //
  // By default, the timer constant is four seconds
  // --------------------------------------------------------------------------------------------------
  if (rat_interrupt_counter() % ( APP_SLEEP_CYCLES * ( 60 / APP_TIMER_CONSTANT ) ) == 0) {
    return 1;
  } else {
    return 0;
  }
}

// ----------------------------------------------------------------------------------------------------
// Sleep
// ----------------------------------------------------------------------------------------------------
void app_sleep (void)
{
  while (true) {
    rat_sleep();

    if (app_wakeup()) {
      break;
    }
  }
}

// ----------------------------------------------------------------------------------------------------
// Application init
//
// This function is called automatically only once when the application starts
// ----------------------------------------------------------------------------------------------------
void app_init (void)
{
  // --------------------------------------------------------------------------------------------------
  // Auxiliary variables
  // --------------------------------------------------------------------------------------------------
  bool result;
  
  // --------------------------------------------------------------------------------------------------
  // Init global variables
  // --------------------------------------------------------------------------------------------------
  gbl_sleep_cycles         = APP_SLEEP_CYCLES;
  gbl_sleep_cycles_counter = 0;

  // --------------------------------------------------------------------------------------------------
  // Init the MCU
  // --------------------------------------------------------------------------------------------------
  rat_mcu_init();
  
  // --------------------------------------------------------------------------------------------------
  // Init the interrupt counter
  // --------------------------------------------------------------------------------------------------
  rat_init_interrupt_counter();

  // --------------------------------------------------------------------------------------------------
  // DEBUG STARTS
  // --------------------------------------------------------------------------------------------------
  #ifdef TEST_MODE
    rat_debug_write_separator();
    rat_debug_write_start_line();
    rat_debug_write_string("Rapiot Indoor Monitor");
    rat_debug_write_start_line();

    rat_debug_write_start_line();
    rat_debug_write_string("RAK Wireless RAK3172H (EU868)");

    #ifdef APP_HUMIDITY_SENSOR
      rat_debug_write_start_line();
      rat_debug_write_string("Sensirion SHT40");
    #endif

    #ifdef APP_THERMOCOUPLE_SENSOR
      rat_debug_write_start_line();
      rat_debug_write_string("Maxim Integrated MAX31855");
    #endif

    rat_debug_write_separator();
  #endif
  // --------------------------------------------------------------------------------------------------
  // DEBUG ENDS
  // --------------------------------------------------------------------------------------------------

  // --------------------------------------------------------------------------------------------------
  // Init the sensors
  // --------------------------------------------------------------------------------------------------
  #ifdef APP_HUMIDITY_SENSOR
    rat_humidity_sensor_init();
  #endif
  
  #ifdef APP_THERMOCOUPLE_SENSOR
    rat_thermocouple_sensor_init();
  #endif
  
  // --------------------------------------------------------------------------------------------------
  // Init and reset the radio module
  // --------------------------------------------------------------------------------------------------
  rat_radio_module_init();
  rat_radio_module_reset();
  
  // --------------------------------------------------------------------------------------------------
  // Init the interrupt counter
  // --------------------------------------------------------------------------------------------------
  rat_init_interrupt_counter();
  
  // --------------------------------------------------------------------------------------------------
  // Stabilization delay
  // --------------------------------------------------------------------------------------------------
  rat_delay(APP_STABILIZATION_DELAY);

  // --------------------------------------------------------------------------------------------------
  // Wait for an interrupt from the timer
  // --------------------------------------------------------------------------------------------------
  rat_wait_interrupt();

  // --------------------------------------------------------------------------------------------------
  // LoRaWAN setup
  // --------------------------------------------------------------------------------------------------
  result = rat_radio_module_set_abp_mode();
  result = rat_radio_module_set_abp_parameters();

  // --------------------------------------------------------------------------------------------------
  // Wait for an interrupt from the timer
  // --------------------------------------------------------------------------------------------------
  rat_wait_interrupt();
}

// ----------------------------------------------------------------------------------------------------
// Application task
//
// This function is called automatically indefinitely until the application restarts
// ----------------------------------------------------------------------------------------------------
void app_task (void)
{
  // --------------------------------------------------------------------------------------------------
  // Auxiliary variables
  // --------------------------------------------------------------------------------------------------
  uint8_t index;
  
  bool uplink_status    = false;
  bool downlink_status  = false;

  uint8_t uplink_data [APP_UPLINK_DATA_SIZE];
  uint8_t downlink_data [APP_DOWNLINK_DATA_SIZE];

  // --------------------------------------------------------------------------------------------------
  // Sensors
  // --------------------------------------------------------------------------------------------------
  #ifdef APP_HUMIDITY_SENSOR
    float temperature;
    float humidity;
  #endif

  #ifdef APP_THERMOCOUPLE_SENSOR
    float thermocouple_temperature;
    float internal_temperature;

    uint8_t fault_flag;
    uint8_t short_vcc_flag;
    uint8_t short_gnd_flag;
    uint8_t open_circuit_flag;
  
    uint8_t flags;
  #endif
  
  // --------------------------------------------------------------------------------------------------
  // Measure
  // --------------------------------------------------------------------------------------------------

  // --------------------------------------------------------------------------------------------------
  // Temperature and humidity sensor
  // --------------------------------------------------------------------------------------------------
  #ifdef APP_HUMIDITY_SENSOR
    if (!rat_humidity_sensor_measure(&temperature,
                                     &humidity)) {
      rat_reset();
    }
  #endif

  // --------------------------------------------------------------------------------------------------
  // Thermocouple sensor
  // --------------------------------------------------------------------------------------------------
  #ifdef APP_THERMOCOUPLE_SENSOR
    rat_thermocouple_sensor_measure(&thermocouple_temperature,
                                    &internal_temperature,
                                    &fault_flag,
                                    &short_vcc_flag,
                                    &short_gnd_flag,
                                    &open_circuit_flag);
                                    
    if (fault_flag        == 1 ||
        short_vcc_flag    == 1 ||
        short_gnd_flag    == 1 ||
        open_circuit_flag == 1) {
      thermocouple_temperature = 0;
      internal_temperature     = 0;
    }
    
    flags = fault_flag;
    
    flags = flags << 1;
    
    flags += short_vcc_flag;
    
    flags = flags << 1;

    flags += short_gnd_flag;
    
    flags = flags << 1;

    flags += open_circuit_flag;
  #endif

  // --------------------------------------------------------------------------------------------------
  // DEBUG STARTS
  // --------------------------------------------------------------------------------------------------
  #ifdef TEST_MODE
    // ------------------------------------------------------------------------------------------------
    // Temperature and humidity sensor
    // ------------------------------------------------------------------------------------------------
    #ifdef APP_HUMIDITY_SENSOR
      app_debug_humidity(temperature,
                         humidity);
    #endif
    
    // ------------------------------------------------------------------------------------------------
    // Thermocouple sensor
    // ------------------------------------------------------------------------------------------------
    #ifdef APP_THERMOCOUPLE_SENSOR
      app_debug_thermocouple(thermocouple_temperature,
                             internal_temperature,
                             fault_flag,
                             short_vcc_flag,
                             short_gnd_flag,
                             open_circuit_flag);
    #endif
  #endif
  // --------------------------------------------------------------------------------------------------
  // DEBUG ENDS
  // --------------------------------------------------------------------------------------------------

  // --------------------------------------------------------------------------------------------------
  // Create the payload
  // --------------------------------------------------------------------------------------------------
  index = 0;
  
  // --------------------------------------------------------------------------------------------------
  // Temperature and humidity (if applicable)
  // --------------------------------------------------------------------------------------------------
  #ifdef APP_HUMIDITY_SENSOR
    uplink_data[index]     = rat_convert_twos_complement(temperature,2) >> 16;
    uplink_data[index + 1] = rat_convert_twos_complement(temperature,2) >> 8;
    uplink_data[index + 2] = rat_convert_twos_complement(temperature,2) % 256;

    uplink_data[index + 3] = rat_convert_twos_complement(humidity,1) >> 8;
    uplink_data[index + 4] = rat_convert_twos_complement(humidity,1) % 256;

    index = index + 5;
  #endif

  // --------------------------------------------------------------------------------------------------
  // Thermocouple (if applicable)
  // --------------------------------------------------------------------------------------------------
  #ifdef APP_THERMOCOUPLE_SENSOR
    uplink_data[index]     = rat_convert_twos_complement(thermocouple_temperature,2) >> 16;
    uplink_data[index + 1] = rat_convert_twos_complement(thermocouple_temperature,2) >> 8;
    uplink_data[index + 2] = rat_convert_twos_complement(thermocouple_temperature,2) % 256;
    
    uplink_data[index + 3] = rat_convert_twos_complement(internal_temperature,2) >> 16;
    uplink_data[index + 4] = rat_convert_twos_complement(internal_temperature,2) >> 8;
    uplink_data[index + 5] = rat_convert_twos_complement(internal_temperature,2) % 256;
    
    uplink_data[index + 6] = flags;

    index = index + 7;
  #endif

  // --------------------------------------------------------------------------------------------------
  // DEBUG STARTS
  // --------------------------------------------------------------------------------------------------
  #ifdef TEST_MODE
    app_debug_payload(uplink_data,index);
    
    app_debug_transmit_start();
  #endif
  // --------------------------------------------------------------------------------------------------
  // DEBUG ENDS
  // --------------------------------------------------------------------------------------------------
  
  // --------------------------------------------------------------------------------------------------
  // Transmit
  // --------------------------------------------------------------------------------------------------
  rat_radio_module_transmit(APP_UPLINK_DATA_SIZE,     // Uplink data length
                            uplink_data,              // Uplink data
                            &uplink_status,           // Uplink status

                            APP_DOWNLINK_DATA_SIZE,   // Downlink data length
                            downlink_data,            // Downlink data
                            &downlink_status);        // Downlink status
                            
  // --------------------------------------------------------------------------------------------------
  // Set the amount of the sleep cycles
  // --------------------------------------------------------------------------------------------------
  if (downlink_status) {
    gbl_sleep_cycles = downlink_data[0];
  }

  // --------------------------------------------------------------------------------------------------
  // DEBUG STARTS
  // --------------------------------------------------------------------------------------------------
  #ifdef TEST_MODE
    if (downlink_status) {
      app_debug_downlink(downlink_data);
    }
    app_debug_transmit_stop();

    app_debug_sleep(gbl_sleep_cycles);
  #endif
  // --------------------------------------------------------------------------------------------------
  // DEBUG ENDS
  // --------------------------------------------------------------------------------------------------
  
  // --------------------------------------------------------------------------------------------------
  // Sleep
  // --------------------------------------------------------------------------------------------------
  app_sleep();

  // --------------------------------------------------------------------------------------------------
  // DEBUG STARTS
  // --------------------------------------------------------------------------------------------------
  #ifdef TEST_MODE
    app_debug_wakeup();
  #endif

  // --------------------------------------------------------------------------------------------------
  // DEBUG ENDS
  // --------------------------------------------------------------------------------------------------
}

// ----------------------------------------------------------------------------------------------------
// The main program
// ----------------------------------------------------------------------------------------------------
void main (void) {
  // --------------------------------------------------------------------------------------------------
  // Run the application init only once
  // --------------------------------------------------------------------------------------------------
  app_init();

  // --------------------------------------------------------------------------------------------------
  // Run the application task indefinitely until reset, shutdown, or restart
  // --------------------------------------------------------------------------------------------------
  while (true) {
    app_task();
  }
}