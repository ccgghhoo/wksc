

#ifndef DFU_SETTINGS_H__
#define DFU_SETTINGS_H__


#include <stdint.h>

#include "nrf_fstorage.h"
/*
---------------------------------------------------------
0x7F000-0x7FFFF Device Factory Configuration (4K)		|
---------------------------------------------------------
0x7E000-0x7EFFF DFU Settings (4K)						|
---------------------------------------------------------
0x7D000-0x7DFFF MBR Settings (4K)						|
---------------------------------------------------------
0x76000-0x7CFFF Bootloader(28K)							|
---------------------------------------------------------
0x18000-0x75FFF Application (376K) 
// include 
0x74000-0x75FFF Device Configuration(8K)
0x73000-0x73FFF Reserved for Meshtech(4K)

0x18000-0x72000 Application(364K)
dual bank mode - only accept 182K application 

---------------------------------------------------------
0x00000-0x17FFFF Softdevice(96K)						|
---------------------------------------------------------
*/

#define  NRF_DFU_SETTINGS_VERSION   01




//typedef fs_cb_t dfu_flash_callback_t;
typedef void (*dfu_flash_callback_t)(void);

uint32_t dfu_setting_update( const uint8_t* init_command, uint16_t size, dfu_flash_callback_t callback ); 
void dfu_setting_flash_init(void);

#endif 

