// -----------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2021 - 2024 Rapiot Open Hardware Project
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// RAK Wireless RAKX Header File
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// -----------------------------------------------------------------------------
// Defines
// -----------------------------------------------------------------------------
#define RAT_RADIO_MODULE_RESET_DELAY    1000   // 1,000 ms

#define RAT_RADIO_MODULE_RESPONSE_DELAY    2   // Two interrupts
#define RAT_RADIO_MODULE_JOIN_DELAY        3   // Three interrupts

#define RAT_RADIO_MODULE_UPLINK_PORT   "1"
#define RAT_RADIO_MODULE_DOWNLINK_PORT "1"

// -----------------------------------------------------------------------------
// Pin names, types, and directions
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Reset
// -----------------------------------------------------------------------------
sbit RAT_RADIO_MODULE_RST_PIN at LATA.B0;
sbit RAT_RADIO_MODULE_RST_DIR at TRISA.B0;
sbit RAT_RADIO_MODULE_RST_TYP at ANSELA.B0;

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Init
// -----------------------------------------------------------------------------
void rat_radio_module_init (void);

// -----------------------------------------------------------------------------
// Reset
// -----------------------------------------------------------------------------
void rat_radio_module_reset (void);

// -----------------------------------------------------------------------------
// Set the ABP mode
// -----------------------------------------------------------------------------
bool rat_radio_module_set_abp_mode (void);

// -----------------------------------------------------------------------------
// Set the ABP parameters
// -----------------------------------------------------------------------------
bool rat_radio_module_set_abp_parameters (void);

// -----------------------------------------------------------------------------
// Transmit and receive a message
// -----------------------------------------------------------------------------
bool rat_radio_module_transmit (uint8_t   uplink_length,      // Uplink payload length
                                uint8_t * uplink_data,        // Uplink payload data
                                bool    * uplink_status,      // Uplink status

                                uint8_t   downlink_length,    // Downlink data length
                                uint8_t * downlink_data,      // Downlink payload data
                                bool    * downlink_status);   // Downlink status