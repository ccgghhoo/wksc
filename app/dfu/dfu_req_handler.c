
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "app_util.h"
#include "app_heap.h"

//#include "app_rtc_drv.h"
#include "app_utc_time.h"
//#include "flash_config.h"
#include "app_flash_drv.h"
#include "dfu.types.h"
#include "dfu_req_handler.h"
#include "dfu.flash.h"
#include "dfu.settings.h"

#include "dfu.res.h"
#include "app_evt.h"
#include "dfu.flash.h"
//#include "mode.h"


//typedef struct{   
//	uint32_t crc32; 
//	uint32_t size; 
//	uint32_t write_offset; 
//}dfu_req_init_t; 

#if 1
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#define DFU_REQ_LOG				        NRF_LOG_DEBUG
#else
#define DFU_REQ_LOG(...)
#endif


static dfu_req_init_t	m_dfu_app; 
static uint32_t  		m_dfu_state; 

static uint8_t*  		mp_init_command = NULL;
static uint32_t	 		m_init_command_len = 0; 

static uint32_t  		m_dfu_timeout; 
static uint32_t 		m_firmware_start_addr; 
static bool             m_dfu_setting_updated_completed = false; 
static bool             m_dfu_request_reset = false; 


dfu_req_init_t* get_firmware_info(void) //chen
{
    return  &m_dfu_app;
}

static void on_dfu_setting_write_completed(void)//(fs_evt_t const * const evt, fs_ret_t result)
{
	//if dfu settting write completed, set device reset 
    m_dfu_setting_updated_completed = true; 
    //if( m_dfu_request_reset )  //chen 2019/07/06  for test
    {
        //device_mode_set( RESET_AS_NORMAL ); 
        set_app_evt(APP_EVT_DFU_RESET); //chen
    }
}

void dfu_init(void)
{		
	mp_init_command = NULL; 
	m_init_command_len = 0; 
	m_dfu_state = DFU_INIT_COMMAND; 
	m_dfu_timeout = 0; 
}

static void dfu_req_init( dfu_req_t * p_dfu_req, dfu_res_t * p_dfu_res )
{
	p_dfu_res->p_data[ 0 ]  = FIRM_KEY_REQ_UPDATE; 
	p_dfu_res->p_data[ 1 ]  = DFU_RES_SUCCESS; 
	p_dfu_res->size 		= 2; 
	
	pb_create_command_t req_init = {0}; 
	
	if( dfu_pb_init_decode( p_dfu_req->p_data, p_dfu_req->size, &req_init ) ) {
		
		mp_init_command = (uint8_t*)APP_MALLOC( DFU_REQ_INIT_COMMAND, p_dfu_req->size ); 
		if( mp_init_command == NULL ){
			p_dfu_res->p_data[ 1 ] = DFU_RES_ERROR_NO_MEM; 
			return; 
		}
		memcpy( mp_init_command, p_dfu_req->p_data, p_dfu_req->size ); 
		m_init_command_len = p_dfu_req->size; 
		
		m_dfu_app.crc32 = req_init.init.crc32; 
		m_dfu_app.size = req_init.init.app_size; 
		m_dfu_app.write_offset = 0; 
		
		m_dfu_state = DFU_WRITE_FIRMWARE; 
		m_firmware_start_addr = DFU_APP1_START_ADDR;//FIRMWARE_START_ADDR;  //chen
		dfu_flash_init();		
	}
	else{
		p_dfu_res->p_data[ 1 ] = DFU_RES_ERROR_INVALID_PARAM; 
	}
}
static void dfu_req_write( dfu_req_t * p_dfu_req, dfu_res_t * p_dfu_res )
{
	p_dfu_res->p_data[ 0 ]  = FIRM_KEY_XFER_CONTENT; 
	p_dfu_res->p_data[ 1 ]  = DFU_RES_SUCCESS; 
	p_dfu_res->size 		= 2; 
    	
	if( m_dfu_state != DFU_WRITE_FIRMWARE){
		p_dfu_res->p_data[ 1 ]  = DFU_RES_ERROR_STATE; 
        DFU_REQ_LOG("dfu state %x", m_dfu_state);
		return; 
	}
	
	if( p_dfu_req->size > 0 && (p_dfu_req->size & 0x03) == 0 && ( p_dfu_req->size > 7 )){
		uint32_t offset = uint32_decode( p_dfu_req->p_data ); 
		if( m_dfu_app.write_offset != offset) {
			p_dfu_res->p_data[ 1 ]  = DFU_RES_ERROR_ADDR; 
            
            DFU_REQ_LOG("req offset: %x mcu offset: %x", offset, m_dfu_app.write_offset);
            
//            uint32_encode( m_dfu_app.write_offset, p_dfu_res->p_data + 2);
//            uint32_encode( offset, p_dfu_res->p_data + 6);
//            p_dfu_res->size = 10;
			return; 
		}
		
		if( dfu_flash_write_update( offset + m_firmware_start_addr, p_dfu_req->p_data + 4, p_dfu_req->size - 4 ) ){ 
			m_dfu_app.write_offset += (p_dfu_req->size - 4) ; 
			
			if( m_dfu_app.write_offset >= m_dfu_app.size ){ // firmware receive completed 
				m_dfu_state = DFU_EXECUTE_VERIFY; 
                p_dfu_res->rsp_neg_flag = 1;  //chen    
			}
			else{
				p_dfu_res->size = 5; 
				uint32_encode( m_dfu_app.write_offset, p_dfu_res->p_data + 1); 
                
			}
		}
		else{
			p_dfu_res->p_data[ 1 ]  = DFU_RES_ERROR_NO_MEM; 
		}
	}
	else{
		p_dfu_res->p_data[ 1 ]  = DFU_RES_ERROR_DATA_SIZE; 
        DFU_REQ_LOG("req size: %x", p_dfu_req->size);
	}
}
static void dfu_req_validate( dfu_res_t * p_dfu_res )
{
	p_dfu_res->size = 2; 
	p_dfu_res->p_data[0] = FIRM_KEY_VALIDATE; 
	p_dfu_res->p_data[1] = DFU_RES_SUCCESS; 
	
	if( m_dfu_state != DFU_EXECUTE_VERIFY ){
		p_dfu_res->p_data[1] = DFU_RES_ERROR_STATE; 
		return; 
	}
	
	uint32_t err_code ; 
	err_code = dfu_setting_update( mp_init_command, m_init_command_len, on_dfu_setting_write_completed); 
	if( err_code != NRF_SUCCESS ){
		p_dfu_res->p_data[1] = DFU_RES_ERROR_NO_MEM; 
	}
}
static void dfu_req_query_state( dfu_res_t * p_dfu_res )
{	
	p_dfu_res->p_data[ 0 ] = FIRM_KEY_QUERY; 
	switch( m_dfu_state )
	{
		case DFU_INIT_COMMAND:
			p_dfu_res->size = 2; 
			p_dfu_res->p_data[ 1 ] = FIRM_KEY_REQ_UPDATE; 
			break; 
		case DFU_WRITE_FIRMWARE:
			p_dfu_res->size = 6; 
			p_dfu_res->p_data[ 1 ] = FIRM_KEY_XFER_CONTENT; 
			uint32_encode( m_dfu_app.write_offset, p_dfu_res->p_data + 2 );
			break; 
		case DFU_EXECUTE_VERIFY:
			p_dfu_res->size = 2; 
			p_dfu_res->p_data[ 1 ] = FIRM_KEY_VALIDATE; 
			break; 
		default:
			p_dfu_res->size = 2; 
			p_dfu_res->p_data[ 1 ] = DFU_RES_UNKNOW_STATE; 
			break; 
	}	
}

static void dfu_req_reset_device( dfu_res_t * p_dfu_res )
{
	p_dfu_res->size = 2; 
	p_dfu_res->p_data[ 0 ] = FIRM_KEY_RESET_DEVICE; 
	p_dfu_res->p_data[ 1 ] = DFU_RES_SUCCESS; 		
    
    m_dfu_request_reset = true; 
    if( m_dfu_setting_updated_completed )
    {
//        device_mode_set( RESET_AS_NORMAL );
          set_app_evt(APP_EVT_DFU_RESET); //chen  
    }
}


bool dfu_req_decode( dfu_req_t * p_dfu_req, dfu_res_t * p_dfu_res )
{
	m_dfu_timeout = app_utc_get_current_time_senconds();
	
	bool success = true; 
	
	switch( p_dfu_req->cmd )
	{
		case FIRM_KEY_REQ_UPDATE:
			dfu_req_init( p_dfu_req, p_dfu_res ); 
            if(p_dfu_res->p_data[1]!=DFU_RES_SUCCESS)  //CHEN
               success = false;   
			break; 
		case FIRM_KEY_XFER_CONTENT:
			dfu_req_write( p_dfu_req, p_dfu_res );
            if(p_dfu_res->p_data[1]!=DFU_RES_SUCCESS) //chen
               success = false;  
			break;
		case FIRM_KEY_VALIDATE:
			dfu_req_validate( p_dfu_res ); 
			break; 
		case FIRM_KEY_QUERY:
			dfu_req_query_state( p_dfu_res ); 
			break;
		case FIRM_KEY_RESET_DEVICE:
			dfu_req_reset_device( p_dfu_res );
        DFU_REQ_LOG("DFU REQ: reset device!!");
			break; 
		default:
            p_dfu_res->p_data[1]=DFU_RES_ERROR_SUB_FUNCTION_NOT_SUPPORTED;//chen
			success = false; 
			break; 
	}
	
	
	return success ; 
}


void dfu_init_cmd_free_alloc_mem(void)//chen 
{
     app_safe_free(mp_init_command);    
}

void dfu_complete_free_alloc_mem(void)
{
    dfu_init_cmd_free_alloc_mem(); 
    dfu_flash_reset();             
}


void dfu_process(void)
{
	if( m_dfu_state != DFU_INIT_COMMAND )
	{
		dfu_flash_process();
		
		if( ( m_dfu_timeout + 10 ) < app_utc_get_current_time_senconds() )
        {
			m_dfu_state = DFU_INIT_COMMAND; 
            dfu_complete_free_alloc_mem();  //chen
		}
	}
}







