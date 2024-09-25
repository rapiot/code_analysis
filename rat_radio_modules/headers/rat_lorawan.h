// -----------------------------------------------------------------------------
// Except when otherwise noted, this file is licensed under
// Creative Commons Attributions ShakeAlike 4.0 License (CC-BY-SA 4.0)
//
// https://creativecommons.org/licenses/by-sa/4.0/legalcode
//
// Copyright (c) 2020 - 2024 Rapiot Open Hardware Project
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Defines
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Device EUI
// -----------------------------------------------------------------------------
#define DEVEUI_BASE 0x00

// -----------------------------------------------------------------------------
// APB parameters
// -----------------------------------------------------------------------------
#define DEVADD_BASE 0x10
#define DEVNSK_BASE 0x20
#define DEVASK_BASE 0x30

// -----------------------------------------------------------------------------
// Device EUI
// -----------------------------------------------------------------------------
#define DEVEUI_BITS  64

// -----------------------------------------------------------------------------
// APB parameters
// -----------------------------------------------------------------------------
#define DEVADD_BITS  32
#define DEVNSK_BITS 128
#define DEVASK_BITS 128

// -----------------------------------------------------------------------------
// Read the parameters of the ABP
//
//   - Device address,                           32 bits - Addresses 0x00 - 0x03
//   - Device extended unique identifier (EUI),  64 bits - Addresses 0x10 - 0x17
//   - Network session key,                     128 bits - Addresses 0x20 - 0x2F
//   - Application session key,                 128 bits - Addresses 0x30 - 0x3F
// -----------------------------------------------------------------------------
void lorawan_read_abp_parameters (char * device_address,
                                  char * device_eui,
                                  char * network_session_key,
                                  char * application_session_key);
                                  
void lorawan_read_device_address (char * device_address);
void lorawan_read_device_eui (char * device_eui);
void lorawan_read_network_session_key (char * network_session_key);
void lorawan_read_application_session_key (char * application_session_key);
