// ----------------------------------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2020 - 2022 Rapiot Open Hardware Project
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Sensor Platform Debugging Source File
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------------------------------
#include <stdint.h>

#include "../../rat_utilities/headers/rat_debug_utilities.h"

// ----------------------------------------------------------------------------------------------------
// Conditional compilation
// ----------------------------------------------------------------------------------------------------
#ifdef TEST_MODE

// ----------------------------------------------------------------------------------------------------
// Debug / Generic message
// ----------------------------------------------------------------------------------------------------
void app_debug (char * message)
{
  rat_debug_write_new_line();
  rat_debug_write_separator();
  rat_debug_write_start_line();
  rat_debug_write_string(message);
  rat_debug_write_separator();
}

// ----------------------------------------------------------------------------------------------------
// Debug temperature and humidity
// ----------------------------------------------------------------------------------------------------
void app_debug_humidity (float temperature,
                         float humidity)
{
  rat_debug_write_new_line();
  rat_debug_write_separator();
  rat_debug_write_start_line();
  rat_debug_write_string("Temperature  : ");
  rat_debug_write_float(temperature,5);
  rat_debug_write_string(" C");
  rat_debug_write_start_line();
  rat_debug_write_string("Humidity     : ");
  rat_debug_write_float(humidity,4);
  rat_debug_write_string(" %");
}

void app_debug_thermocouple (float thermocouple_temperature,
                             float internal_temperature,
                             uint8_t fault_flag,
                             uint8_t short_vcc_flag,
                             uint8_t short_gnd_flag,
                             uint8_t open_circuit_flag)
{

  rat_debug_write_new_line();
  rat_debug_write_separator();
  rat_debug_write_start_line();
  rat_debug_write_string("Fault flag               : ");
  rat_debug_write_boolean(fault_flag);
  rat_debug_write_start_line();
  rat_debug_write_string("Short to VCC flag        : ");
  rat_debug_write_boolean(short_vcc_flag);
  rat_debug_write_start_line();
  rat_debug_write_string("Short to GND flag        : ");
  rat_debug_write_boolean(short_gnd_flag);
  rat_debug_write_start_line();
  rat_debug_write_string("Open circuit flag        : ");
  rat_debug_write_boolean(open_circuit_flag);


  if (fault_flag        == 0 &&
      short_vcc_flag    == 0 &&
      short_gnd_flag    == 0 &&
      open_circuit_flag == 0) {
    rat_debug_write_start_line();
    rat_debug_write_string("Thermocouple temperature : ");
    rat_debug_write_float(thermocouple_temperature,4);
    rat_debug_write_string(" C");
    rat_debug_write_start_line();
    rat_debug_write_string("Internal temperature     : ");
    rat_debug_write_float(internal_temperature,4);
    rat_debug_write_string(" C");
  }
}

// ----------------------------------------------------------------------------------------------------
// Debug pressure
// ----------------------------------------------------------------------------------------------------
void app_debug_pressure (float pressure)
{
  rat_debug_write_new_line();
  rat_debug_write_separator();
  rat_debug_write_start_line();
  rat_debug_write_string("Pressure     : ");

  if (pressure < 100000) {
    rat_debug_write_float(pressure,5);
  } else {
    rat_debug_write_float(pressure,6);
  }

  rat_debug_write_string(" Pa");
}

// ----------------------------------------------------------------------------------------------------
// Debug payload
// ----------------------------------------------------------------------------------------------------
void app_debug_payload (uint8_t * payload,
                        uint8_t   size)
{
  uint8_t n;

  rat_debug_write_start_line();
  
  rat_debug_write_start_line();
  rat_debug_write_string("Payload size : ");
  rat_debug_write_unsigned_int(size);
  rat_debug_write_string(" B");
  rat_debug_write_start_line();
  
  rat_debug_write_string("Payload      : ");
  
  for (n = 0;n < size;n++) {
    rat_debug_write_unsigned_int_short_hex(payload[n]);
    rat_debug_write_string(" ");
  }
  rat_debug_write_separator();
}

// ----------------------------------------------------------------------------------------------------
// Debug transmit start
// ----------------------------------------------------------------------------------------------------
void app_debug_transmit_start (void)
{
  rat_debug_write_new_line();
  rat_debug_write_separator();
  rat_debug_write_start_line();
  rat_debug_write_string("Transmitting ...");
  rat_debug_write_separator();
}

// ----------------------------------------------------------------------------------------------------
// Debug transmit stop
// ----------------------------------------------------------------------------------------------------
void app_debug_transmit_stop (void)
{
  rat_debug_write_new_line();
  rat_debug_write_separator();
  rat_debug_write_start_line();
  rat_debug_write_string("Transmitted successfully");
  rat_debug_write_separator();
}

// ----------------------------------------------------------------------------------------------------
// Debug downlink
// ----------------------------------------------------------------------------------------------------
void app_debug_downlink (uint8_t * response)
{
  uint8_t n;

  rat_debug_write_new_line();
  rat_debug_write_separator();
  rat_debug_write_start_line();
  rat_debug_write_string("Response     : ");

  for (n = 0;n < 8;n++) {
    rat_debug_write_unsigned_int_short_hex(response[n]);
    rat_debug_write_string(" ");
  }
  rat_debug_write_separator();
}

// ----------------------------------------------------------------------------------------------------
// Debug sleep
// ----------------------------------------------------------------------------------------------------
void app_debug_sleep (uint8_t sleep_cycles)
{
  rat_debug_write_new_line();
  rat_debug_write_separator();
  rat_debug_write_start_line();
  rat_debug_write_string("Entering sleep mode ... ");
  rat_debug_write_unsigned_int_short(sleep_cycles);
  rat_debug_write_string(" cycles ...");
  rat_debug_write_separator();
}

// ----------------------------------------------------------------------------------------------------
// Debug wakeup
// ----------------------------------------------------------------------------------------------------
void app_debug_wakeup (void)
{
  rat_debug_write_new_line();
  rat_debug_write_separator();
  rat_debug_write_start_line();
  rat_debug_write_string("Leaving sleep mode ...");
  rat_debug_write_separator();
}

#endif