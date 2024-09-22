// ----------------------------------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2020 - 2022 Rapiot Open Hardware Project
//
// Known limitations :
//
//   Only EU 868 MHz is supported.
//
//   Only RHF0M003-HF20 has been tested.
//
// Known issues :
//
//   There are no known issues.
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// RisingHF RHFX Header File
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../../rat_application/headers/defines.h"

// ----------------------------------------------------------------------------------------------------
// Conditional compilation
// ----------------------------------------------------------------------------------------------------
#ifdef RISINGHF_RHFX

// ----------------------------------------------------------------------------------------------------
// Defines
// ----------------------------------------------------------------------------------------------------
#define RAT_RADIO_MODULE_RESET_DELAY    1000   // 1,000 ms ... note that this is only for debugging!

#define RAT_RADIO_MODULE_JOIN_DELAY        3   // Three interrupts ... twelve seconds
#define RAT_RADIO_MODULE_DEFAULT_DELAY     2   // Two interrupts   ... eight seconds

// ----------------------------------------------------------------------------------------------------
// Pin names, types, and directions
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Reset
// ----------------------------------------------------------------------------------------------------
sbit RAT_RADIO_MODULE_RST_PIN at LATA.B0;
sbit RAT_RADIO_MODULE_RST_DIR at TRISA.B0;
sbit RAT_RADIO_MODULE_RST_TYP at ANSELA.B0;

// ----------------------------------------------------------------------------------------------------
// Wakeup
// ----------------------------------------------------------------------------------------------------
sbit RAT_RADIO_MODULE_WKP_PIN at LATB.B1;
sbit RAT_RADIO_MODULE_WKP_DIR at TRISB.B1;
sbit RAT_RADIO_MODULE_WKP_TYP at ANSELB.B1;

// ----------------------------------------------------------------------------------------------------
// Functions
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// Init
// ----------------------------------------------------------------------------------------------------
void rat_radio_module_init (void);

// ----------------------------------------------------------------------------------------------------
// Reset
// ----------------------------------------------------------------------------------------------------
void rat_radio_module_reset (void);

// ----------------------------------------------------------------------------------------------------
// Set the LoRaWAN parameters
//
// Returns true if the parameters were set successfully; false otherwise.
// ----------------------------------------------------------------------------------------------------
bool rat_radio_module_set_parameters (lorawan_mode mode);

// ----------------------------------------------------------------------------------------------------
// Set the LoRaWAN mode
//
// Returns true if the mode was set successfully; false otherwise.
// ----------------------------------------------------------------------------------------------------
bool rat_radio_module_set_mode (lorawan_mode mode);

// ----------------------------------------------------------------------------------------------------
// Join OTAA
//
// Returns true if the join process was successful; false otherwise.
// ----------------------------------------------------------------------------------------------------
bool rat_radio_module_join_otaa (void);

// ----------------------------------------------------------------------------------------------------
// Transmit and receive a message
// ----------------------------------------------------------------------------------------------------
bool rat_radio_module_transmit (uint8_t   uplink_port,        // Uplink port
                                uint8_t   uplink_length,      // Uplink payload length
                                uint8_t * uplink_data,        // Uplink payload data
                                bool    * uplink_status,      // Uplink transmission status

                                bool      acknowledge,        // Confirmed or unconfirmed message

                                uint8_t * downlink_port,      // Downlink port
                                uint8_t   downlink_length,    // Downlink data length
                                uint8_t * downlink_data,      // Downlink payload data
                                bool    * downlink_status);   // Downlink transmission status
                                                     
// ----------------------------------------------------------------------------------------------------
// Sleep
//
// Returns true if the response matches the reference; false otherwise.
// ----------------------------------------------------------------------------------------------------
bool rat_radio_module_sleep (void);   // TODO : Add empty templates to other modules if necessary

// ----------------------------------------------------------------------------------------------------
// Wakeup
//
// Returns true if the response matches the reference; false otherwise.
// ----------------------------------------------------------------------------------------------------
bool rat_radio_module_wakeup (void);   // TODO : Add empty templates to other modules if necessary

#endif