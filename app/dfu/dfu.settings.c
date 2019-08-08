
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "app_scheduler.h"
#include "dfu_req_handler.h"
#include "nrf_fstorage.h"
#include "nrf_nvmc.h"
#include "nrf_delay.h"

#include "app_heap.h"
#include "dfu.types.h"
#include "dfu.settings.h"
#include "crc32.h"
#include "app_flash_drv.h"


#define FLASH_FLAG_NONE                 (0)
#define FLASH_FLAG_OPER                 (1<<0)
#define FLASH_FLAG_FAILURE_SINCE_LAST   (1<<1)
#define FLASH_FLAG_SD_ENABLED           (1<<2)

static uint32_t m_flags = FLASH_FLAG_SD_ENABLED;  /*lint -e551*/
//lint -save -esym(551, flash_operation_pending)
static bool flash_operation_pending; // barrier for reading flash

static dfu_settings_t *		s_dfu_settings; 



static void dfu_setting_fs_storage_callback(nrf_fstorage_evt_t * p_evt)
{
    if (p_evt->result != NRF_SUCCESS)
    {
        NRF_LOG_INFO("--> dfu_setting_fs : ERROR while executing an fstorage operation.");        
        return;
    }

    switch (p_evt->id)
    {
        case NRF_FSTORAGE_EVT_WRITE_RESULT:
        {
            NRF_LOG_INFO("--> dfu_setting_fs: wrote %d bytes at address 0x%x.",p_evt->len, p_evt->addr);
                        
        } break;

        case NRF_FSTORAGE_EVT_ERASE_RESULT:
        {
            NRF_LOG_INFO("--> dfu_setting_fs: erased %d page from address 0x%x.",p_evt->len, p_evt->addr);
                        
        } break;

        default:
        	
            break;
    }    
       
}

NRF_FSTORAGE_DEF(nrf_fstorage_t dfu_setting_flash_fs_config) =
{
    .evt_handler = dfu_setting_fs_storage_callback,
    .start_addr = DFU_BL_SETTING_START_ADDR,
    .end_addr   = DFU_BL_SETTING_END_ADDR,
};




uint32_t dfu_settings_calculate_crc(void)
{
	return crc32_compute((uint8_t*)(s_dfu_settings + 4), sizeof(dfu_settings_t) - 4 - sizeof(s_dfu_settings->init_command), NULL);
}


uint32_t dfu_setting_update( const uint8_t* init_command, uint16_t size, dfu_flash_callback_t callback )
{
    
    s_dfu_settings = ( dfu_settings_t *) APP_MALLOC( DFU_SAVE_SETTING_BUFFER, sizeof( dfu_settings_t ) ); 
      
	if( s_dfu_settings == NULL ){
		return NRF_ERROR_NO_MEM; 
	}
    
//    nrf_fstorage_read(&dfu_setting_flash_fs_config,
//                              DFU_BL_SETTING_START_ADDR,//0X7F000 ,
//                              s_dfu_settings,  
//                              sizeof(dfu_settings_t)            
//                              );
    memcpy( s_dfu_settings, (uint32_t*)DFU_BL_SETTING_START_ADDR, sizeof(dfu_settings_t ));     
   
    uint32_t ret_code = nrf_fstorage_erase(&dfu_setting_flash_fs_config,
                              DFU_BL_SETTING_START_ADDR,//0X7F000 ,
                              1,                 //1*4096
                              NULL);
    if(ret_code!=NRF_SUCCESS)
    {
        return NRF_ERROR_BUSY;
    }
    
//    while(nrf_fstorage_is_busy(&dfu_setting_flash_fs_config))
//    {
//        ;
//    }
//	
	
	s_dfu_settings->bank_1.bank_code = NRF_DFU_BANK_READY_APP;//NRF_DFU_BANK_READY_APP;

    dfu_req_init_t	*temp_dfu_app;          //chen
    temp_dfu_app = get_firmware_info();    
    s_dfu_settings->bank_1.image_crc = temp_dfu_app->crc32;
    s_dfu_settings->bank_1.image_size = temp_dfu_app->size;
    NRF_LOG_INFO("update firmware crc: 0x%x  update firmware size 0x%x.",s_dfu_settings->bank_1.image_crc, s_dfu_settings->bank_1.image_size);
    
    s_dfu_settings->settings_version =  NRF_DFU_SETTINGS_VERSION ; 
    s_dfu_settings->write_offset = 0;  //chen    
	s_dfu_settings->progress.command_size = size; 
	s_dfu_settings->progress.command_crc = crc32_compute(init_command, size, NULL) ; 
    
	s_dfu_settings->progress.firmware_image_offset = (DFU_APP1_START_ADDR); //chen
    //s_dfu_settings->bank_current = NRF_DFU_CURRENT_BANK_1;
    
	memcpy( s_dfu_settings->init_command, init_command, size ); 
	
    s_dfu_settings->crc = crc32_compute( (uint8_t*)s_dfu_settings + 4, sizeof(dfu_settings_t) - 4 - sizeof(s_dfu_settings->init_command), NULL);
    

        ret_code = nrf_fstorage_write(&dfu_setting_flash_fs_config,
                                       DFU_BL_SETTING_START_ADDR,//0X7E000 ,
                                       s_dfu_settings,
                                       sizeof(dfu_settings_t),
                                       NULL);
     
    
     
    app_safe_free(s_dfu_settings);         //chen    
    app_safe_free((void*)init_command);    //chen  
//    dfu_complete_free_alloc_mem();                                    
    
	if(ret_code!=NRF_SUCCESS)
    {
        NRF_LOG_INFO("dfu_setting_fs  write fail !!!!");         
        return NRF_ERROR_BUSY;
        
    }
    
    if(callback!=NULL)
    {
        NRF_LOG_INFO("dfu_setting_fs  reset device ,, dfu sucessed !!!!");   
        callback();
    }
	return NRF_SUCCESS; 
    
}

void dfu_setting_flash_init(void)
{
    ret_code_t rc;
    nrf_fstorage_api_t * p_fs_api;
    p_fs_api = &nrf_fstorage_nvmc;    
    rc = nrf_fstorage_init(&dfu_setting_flash_fs_config, p_fs_api, NULL);
    APP_ERROR_CHECK(rc);
    
}



