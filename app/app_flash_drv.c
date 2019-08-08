
// flash --- app flash driver 


#include <stdint.h>
#include <stdbool.h>

#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "crc16.h"

//#include "app_spi_drv.h"
#include "app_flash_drv.h"
//#include "mx25l8006.h"
#include "boards.h"

//#include "nrf_log.h"

static  uint32_t 				m_flash_in_used_count ; 
static  uint32_t                m_write_flash_result = 0xffffffff; 



static void fs_storage_callback(nrf_fstorage_evt_t * p_evt)
{
    if (p_evt->result != NRF_SUCCESS)
    {
        NRF_LOG_INFO("--> Event received: ERROR while executing an fstorage operation.");        
        return;
    }

    switch (p_evt->id)
    {
        case NRF_FSTORAGE_EVT_WRITE_RESULT:
        {
            NRF_LOG_INFO("--> Event received: wrote %d bytes at address 0x%x.",p_evt->len, p_evt->addr);
             m_write_flash_result = 0xaaaaaaaa;             
        } break;

        case NRF_FSTORAGE_EVT_ERASE_RESULT:
        {
            NRF_LOG_INFO("--> Event received: erased %d page from address 0x%x.",p_evt->len, p_evt->addr);
            m_write_flash_result = 0x55555555;             
        } break;

        default:
        	
            break;
    }    
       
}

NRF_FSTORAGE_DEF(nrf_fstorage_t app_flash_fs_config) =
{
    .evt_handler = fs_storage_callback,
    .start_addr = DFU_FLASH_START_ADDR,
    .end_addr   = DFU_FLASH_END_ADDR,
};




__INLINE bool flash_addr_in_same_sector(uint32_t addr1, uint32_t addr2)
{
	return ( addr1 & FLASH_SECTOR_NUM_MASK ) == (addr2 & FLASH_SECTOR_NUM_MASK); 
}

__INLINE uint32_t flash_addr_to_next_sector(uint32_t addr)
{
	return ( addr + FLASH_SECTOR_SIZE ) & FLASH_SECTOR_NUM_MASK;
}

__INLINE bool flash_addr_is_beginning_of_sector( uint32_t addr)
{
	return (addr & FLASH_SECTOR_SIZE_MASK) == 0;
}

//__INLINE void flash_enter_power_down_enable( bool enable )
//{
//	m_flash_enable_power_down = enable; 
//}



//__STATIC_INLINE void flash_read_bytes(uint32_t addr, uint8_t* p_dst, uint16_t len)
//{

//}

//__STATIC_INLINE void flash_write_bytes( uint32_t addr, uint8_t* p_src, uint16_t len)
//{

//}


bool flash_write_completed(void)
{
    //return true; //chen
    return !nrf_fstorage_is_busy(&app_flash_fs_config);
}

bool flash_write_data(uint32_t addr, uint8_t *data, uint16_t len)
{
    //m_write_flash_result = 0xffffffff; 
    uint32_t ret_code = nrf_fstorage_write(&app_flash_fs_config,
                              addr,
                              data,
                              len,
                              NULL);
    return ret_code;
}

bool flash_read_data(uint32_t addr, uint8_t *data, uint16_t len)
{
	//memcpy(data , (uint8_t*)addr, len);
    uint32_t ret_code = nrf_fstorage_read(&app_flash_fs_config,
                             addr,
                             data,
                             len);
    return ret_code;                         
}

bool flash_erase_sector(uint32_t addr)
{
    //m_write_flash_result = 0xffffffff; 
    uint32_t ret_code = nrf_fstorage_erase(&app_flash_fs_config,
                              addr,
                              1,                 //1*4096
                              NULL);
    return ret_code;
}

void app_dfu_flash_init(void)
{
    
    ret_code_t rc;
    nrf_fstorage_api_t * p_fs_api;
    p_fs_api = &nrf_fstorage_nvmc;    
    rc = nrf_fstorage_init(&app_flash_fs_config, p_fs_api, NULL);
    APP_ERROR_CHECK(rc);  //chen	

}

void flash_op_test(void)
{
     NRF_LOG_INFO("erase flash test starting !!");
    uint32_t addr=(DFU_APP1_START_ADDR);
              
    uint32_t ret_code = nrf_fstorage_erase(&app_flash_fs_config,
                              addr,
                              60,                 //1*4096
                              NULL);
    NRF_LOG_INFO("erase flash test end  result:%d !!", ret_code);
    
    uint8_t buff[]={"123456781234"};
    ret_code = flash_write_data(DFU_APP1_START_ADDR, buff, 12);
    
    NRF_LOG_INFO("write flash result:%d !!", ret_code);
    
    uint8_t readbuff[20];
    flash_read_data(DFU_APP1_START_ADDR, readbuff, 20);
    
    for(uint32_t i=0; i<20; i++)
    {
        NRF_LOG_INFO("read flash:%d !!", readbuff[i]);
    }
    
    
}

