

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "app_heap.h"

#include "dfu.flash.h"

#include "app_flash_drv.h"
#include "dfu.settings.h"
#include "crc32.h"

#if 1 
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#define DFU_FLASH_LOG			NRF_LOG_INFO
#else
#defien DFU_FLASH_LOG(...) 
#endif 

#define DFU_WRITE_FIFO_SIZE		4 
#define DFU_WRITE_FIFO_MASK		( DFU_WRITE_FIFO_SIZE - 1 ) 

typedef struct{
	uint8_t *	pdata;
	uint32_t 	addr; 
	uint32_t 	size; 
}dfu_flash_write_data_t; 

typedef struct{
	dfu_flash_write_data_t		fifo[ DFU_WRITE_FIFO_SIZE ];
	uint32_t 	rp;
	uint32_t 	count;
}dfu_flash_write_t; 

static dfu_flash_write_t		m_dfu_write; 
static dfu_flash_step_t			m_dfu_step; 
static uint32_t 				dfu_write_offset = 0; 
static uint32_t 				dfu_flags; 


static bool dfu_flash_op_advance(void)
{
	return ( m_dfu_write.count > 0 ); 
}

static void dfu_flash_write_execute(void)
{
//	dfu_flags = DFU_FLAG_EXECUTING; 
	
	dfu_flash_write_data_t  * p_dfu_write = &m_dfu_write.fifo[ m_dfu_write.rp & DFU_WRITE_FIFO_MASK ];
	uint32_t ret = DFU_RET_SUCCESS; 
	
	switch( m_dfu_step )
	{
		case DFU_STEP_CHECK_BUSY: //wait flash is writable 
			if( !flash_write_completed() )
				break;;
			
			m_dfu_step = DFU_STEP_CHECK_SECTOR ; 
			dfu_write_offset = 0; 
			DFU_FLASH_LOG("[DFU]: idle\r\n"); 			
			
		case DFU_STEP_CHECK_SECTOR: // check sector is start address 
			if( (p_dfu_write->addr & FLASH_SECTOR_SIZE_MASK) == 0 ){
				flash_erase_sector( p_dfu_write->addr ); 
				m_dfu_step = DFU_STEP_WAIT_COMPLETED; 
				DFU_FLASH_LOG("[DFU]: wait write completed\r\n"); 
				break; 
			}
			
			m_dfu_step = DFU_STEP_CHECK_BEFORE_WRITE; 
			
		case DFU_STEP_CHECK_BEFORE_WRITE:
			if( !flash_write_completed() )
				break;
			
			m_dfu_step = DFU_STEP_WRITE; 
			
		case DFU_STEP_WRITE:
		{
			DFU_FLASH_LOG("[DFU]: write = 0x%X, data = 0x%X\r\n", p_dfu_write->addr, (uint32_t)p_dfu_write->pdata); 
			uint32_t remains = p_dfu_write->size - dfu_write_offset; 
			
			if( p_dfu_write->addr == 0x65100 ){
				__nop();
			}
			
			if( remains > FLASH_PAGE_SIZE ){
				flash_write_data( p_dfu_write->addr + dfu_write_offset, p_dfu_write->pdata + dfu_write_offset, FLASH_PAGE_SIZE );
				dfu_write_offset += FLASH_PAGE_SIZE; 				
			}
			else{
				flash_write_data( p_dfu_write->addr + dfu_write_offset, p_dfu_write->pdata + dfu_write_offset, remains ); 
				dfu_write_offset = p_dfu_write->size; 
				
			}	
			m_dfu_step = DFU_STEP_WAIT_COMPLETED ;			
			
		}
			break; 
		
		case DFU_STEP_WAIT_COMPLETED:
			if( flash_write_completed() ){
				if( dfu_write_offset < p_dfu_write->size ){
					m_dfu_step = DFU_STEP_CHECK_BEFORE_WRITE; 
				}
				else{	
					APP_FREE( p_dfu_write->pdata ); 
					
					dfu_write_offset = 0;
					m_dfu_write.rp ++; 
					m_dfu_write.count --; 
					ret = DFU_RET_WRITE_COMPLETED; 
					DFU_FLASH_LOG("[DFU]: write completed, remain=%d \r\n", m_dfu_write.count); 
				}
			}
			break; 
			
		default:
			break; 
	}
	
	if( ret != DFU_RET_SUCCESS ){
		// queue next write operation 
		if( dfu_flash_op_advance() ){
			m_dfu_step = DFU_STEP_CHECK_BUSY; 
		}
		else{
			dfu_flags = DFU_FLAG_IDLE; 
			m_dfu_step = DFU_STEP_CHECK_BUSY; 
		}
	}
}

void dfu_flash_process(void)
{
	if( dfu_flags == DFU_FLAG_IDLE )
		return; 
	
	dfu_flash_write_execute(); 
}

bool dfu_flash_write_update(uint32_t write_offset, const uint8_t* p_data, uint16_t size )
{
	if( m_dfu_write.count < DFU_WRITE_FIFO_SIZE ){
		uint32_t idx = (m_dfu_write.count + m_dfu_write.rp) & DFU_WRITE_FIFO_MASK; 
		
		m_dfu_write.fifo[ idx ].pdata = (uint8_t*)APP_MALLOC( DFU_WRITE_FLASH_TEXT, size ); 
		if( m_dfu_write.fifo[ idx ].pdata == NULL ){
			return false; 
		}
		memcpy( m_dfu_write.fifo[ idx ].pdata , p_data, size ); 
		
		m_dfu_write.fifo[ idx ].addr = write_offset; 		
		m_dfu_write.fifo[ idx ].size = size; 
		
		m_dfu_write.count ++; 		
		
		DFU_FLASH_LOG("[DFU]: addr = 0x%X, count = %d\r\n", write_offset, m_dfu_write.count ); 
		
		if( dfu_flags == DFU_FLAG_IDLE ){
			dfu_flags = DFU_FLAG_EXECUTING ; 
			dfu_flash_process();
		}
		return true; 
	}
	else{
		return false; 
	}
}

uint32_t dfu_flash_crc_compute( uint32_t addr, uint32_t size )
{
	uint32_t crc32 = 0; 
	uint32_t offset = 0; 
	
	uint8_t temp[ FLASH_PAGE_SIZE ]; 
	
	while( offset < size )
	{
		if( (offset + FLASH_PAGE_SIZE) < size ){
			flash_read_data( addr + offset, temp, FLASH_PAGE_SIZE ); 
			crc32 = crc32_compute( temp, FLASH_PAGE_SIZE, &crc32 ); 
			offset += FLASH_PAGE_SIZE; 
		}
		else{
			uint32_t bytes = size - offset; 
			flash_read_data( addr + offset, temp, bytes ); 
			crc32 = crc32_compute( temp, bytes, &crc32 ); 
			offset = size; 
		}
	}	
	
	return crc32; 
}


void dfu_flash_reset(void)
{
	for( uint32_t i = 0 ; i < m_dfu_write.count; i ++)
	{
		uint32_t rp = ( m_dfu_write.rp & DFU_WRITE_FIFO_MASK );
		//app_safe_free( &m_dfu_write.fifo[ rp ].addr ); 
        app_safe_free( &m_dfu_write.fifo[ rp ].pdata ); //chen added
		m_dfu_write.fifo[ rp ].pdata = NULL;   //chen  added
		m_dfu_write.rp ++; 
		m_dfu_write.count --; 
	}       
}


void dfu_flash_init(void)
{
	m_dfu_step = DFU_STEP_CHECK_BUSY; 
	dfu_flags = DFU_FLAG_IDLE; 
//	flash_enter_power_down_enable( false ); 
//	flash_release_power_down_mode();
    
    app_dfu_flash_init();      //chen
    dfu_setting_flash_init();  //chen
	
	
}


