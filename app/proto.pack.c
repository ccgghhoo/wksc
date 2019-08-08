
#include <string.h>
#include <stdbool.h>

#include "protocol.h"
#include "proto.pack.h"
#include "proto.pb.h"

//#include "dev_conf.pb.h"
//#include "dev_config.get.h"

#include "dfu_req_handler.h"

//#include "ls_api.h"
//#include "app_flash_drv.h"
//#include "record.h"

//#include "app_rtc_drv.h"
#include "app_heap.h"
#include "crc16.h"
//#include "battery.h"
//#include "gsflag.h"

#include "app_i2c_slave.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

resp_callback_t			m_resp_cb; 
msg_packet_t* 			mp_msg_ptr; 
//bool 					m_resp_sent; 
static uint32_t 		m_send_count = 0; 





#if 1 
void __data_pack(uint8_t *data, uint16_t len, uint8_t flag)
{
 
	m_resp_sent = true; 
	m_resp_len = len+8;
	uint8_t * data_buff = m_i2c_slave_txbuff;
//(uint8_t*)APP_MALLOC( PROTO_PACK_DATA_PACK, len + 8 ); //2019/06/03 m_i2c_slave_txbuff
//	if( data_buff == NULL){	//no more heap to alloc 
//		return; 
//	}
	        
	msg_packet_t 	respHead; 
	respHead.magic	= MAGIC_NUMBER; 
	respHead.len	= len; 
	respHead.val	= flag; 
       
    respHead.id		= mp_msg_ptr->id; 	
    m_send_count	= mp_msg_ptr->id;
               		
	if( len > 0){			
		uint16_t crc16 = crc16_compute( data, len, 0); 
		respHead.crc = crc16; 
	}else{
		respHead.crc = 0; 
	}
	
	memcpy( data_buff, (uint8_t*)&respHead, 8); 
	memcpy( data_buff + 8, data, len); 
    
    NRF_LOG_INFO(" resp data len: %x !!", m_resp_len); 
	
	if( m_resp_cb != NULL ){
		m_resp_cb( m_i2c_slave_txbuff, len + 8); 
	}	
}

void __ack_resp(void)
{
	__data_pack( NULL, 0, FLAG_ACK_RESP);
}

void __negative_resp(uint8_t code)
{
	uint8_t 	data[3]; 
	data[0] = 	COMMAND_ID_NEGATIVE;
    data[1] =   1;
	data[2] = 	code; 
	
	__data_pack( data, 3, FLAG_ERROR_RESP); 
}

//static 
    uint8_t * __send_pack_msg(uint8_t *data, uint16_t len, uint8_t flag)
{
//	uint8_t * data_buff = (uint8_t*)APP_MALLOC( PROTO_PACK_SEND_MSG_PACK, len + 8 ); 
//	if( data_buff == NULL){	//no more heap to alloc 
//		return NULL; 
//	}
    
	m_resp_sent = true; 
	m_resp_len = len+8;
	uint8_t * data_buff = m_i2c_slave_txbuff;
	
	msg_packet_t 	respHead; 
	respHead.magic	= MAGIC_NUMBER; 
	respHead.len	= len; 
	respHead.val	= flag; 
	respHead.id		= ++m_send_count; 	
		
	if( len > 0){			
		uint16_t crc16 = crc16_compute( data, len, 0); 
		respHead.crc = crc16; 
	}else{
		respHead.crc = 0; 
	}
	
	memcpy( data_buff, (uint8_t*)&respHead, 8); 
	memcpy( data_buff + 8, data, len); 
	
	return data_buff; 
}
#endif 

#if 1 // record data request => response 

__WEAK void historical_data_has_been_tx_completed(void)
{
	// define strong reference somewhere 
}

static void proto_pack_historical_tx_completed( void )
{
//	uint8_t temp[20]; 
//	uint16_t chunk_len = 0; 
//	
//	temp[0] = COMMAND_ID_DATA; 
//	
//	temp[1] = 16;
//	temp[2] = DEV_CONF_IMEI;
//	memcpy( temp + 2, (uint8_t*)dev_config_get_IMEI(), 15 );  
//	chunk_len += 18; 
//	
//	temp[ chunk_len + 0] = 1; 
//	temp[ chunk_len + 1] = DATA_KEY_TX_COMPLETED;
//	chunk_len +=2; 
//	
//	__data_pack( temp, chunk_len, FLAG_NORMAL_RESP ); 
//	
//	historical_data_has_been_tx_completed();
}

bool proto_pack_gps_latest_data( void )
{
//	uint8_t temp[ 50 ]; 
//	uint16_t chunk_len = 0; 
//	
//	temp[0] = COMMAND_ID_DATA;
//	
//	temp[1] = 16; 
//	temp[2] = DEV_CONF_IMEI;
//	memcpy( temp + 3, (uint8_t*)dev_config_get_IMEI(), 15 );  
//	chunk_len += 18; 
//	
//	temp[chunk_len + 1] = DATA_KEY_GPS_LATEST_LOC; 
//	temp[chunk_len + 0] = ls_api_get_final_locate_packet( temp + chunk_len + 2 ) + 1; 	
//	if( temp[ chunk_len + 0 ] == 0 )
//		return false; 
//	
//	chunk_len += temp[ chunk_len + 0] + 1; 
//	
//	__data_pack( temp, chunk_len, FLAG_NORMAL_RESP); 
//	return true; 
}
bool proto_pack_historical_data(void)
{
//	if( record_data_is_empty() ){
//		proto_pack_historical_tx_completed();		
//		return true; 
//	}
//	
//	uint8_t * tx_buff = APP_MALLOC( PROTO_PACK_RESP_HISTORICAL_DATA, EPB_PAYLOAD_MAX_LEN ); 
//	if( tx_buff == NULL ){
//		return false; 
//	}
//	
//	uint16_t chunk_len = 0; 
//	
//	tx_buff[0] = COMMAND_ID_DATA; 
//    
//    tx_buff[1] = 16; 
//	tx_buff[2] = DEV_CONF_IMEI;
//	memcpy( tx_buff + 3, (uint8_t*)dev_config_get_IMEI(), 15 );  
//	chunk_len += 18; 
//    
//	tx_buff[18] = 5; 
//	tx_buff[19] = DATA_KEY_HISTORICAL_DATA; 
//	
//	chunk_len += record_data_fetch( tx_buff + 20, EPB_PAYLOAD_MAX_LEN - 3 ); 
//	__data_pack( tx_buff, chunk_len + 3, FLAG_NORMAL_RESP); 
//	
//	APP_FREE( tx_buff );
	
	return true; 
}
#endif 

void proto_pack_memory_reader_process( uint8_t field_type, const uint8_t * p_data, uint16_t len )
{
	uint32_t addr = uint32_decode( p_data ); 
	uint16_t size = uint16_decode( p_data + 4 ); 
	
	if( size > 512 || size == 0)
		return; 
	
	uint8_t * buff = APP_MALLOC( PROTO_PACK_MEMORY_READER, size + 9 ); 
	if( buff == NULL )
		return; 
	
	buff[ 0 ] = COMMAND_ID_MEMORY_READER; 
	buff[ 1 ] = 7; 
	buff[ 2 ] = field_type; 
	
	uint32_encode( addr, buff + 3 ); 
	uint16_encode( size, buff + 7 ); 
	
	switch( field_type )
	{
		case MEMO_KEY_INNER_RAM:
			if( addr > 0x2000000  && addr < 0x20010000 ){
				memcpy( buff + 8, (uint8_t*)addr, size ); 
			}
			else if( addr >= 0x70000 && addr < 0x80000){
				memcpy( buff + 8, (uint8_t*)addr, size ); 
			}
			break; 
		
//		case MEMO_KEY_EXTERNAL_FLASH:
//			if( addr < FLASH_END_ADDR ) {
//				flash_release_power_down_mode(); 
//				flash_read_data( addr, buff + 8, size ); 
//				flash_enter_power_down_mode();
//			}			
//			break; 
		
		case MEMO_KEY_MEMS:
			break; 
	}
	
	__data_pack( buff, size + 8, FLAG_NORMAL_RESP ); 
	
	APP_FREE( buff ); 
}


void proto_pack_dfu_process(uint8_t field_type, const uint8_t * p_data, uint16_t len )
{
	dfu_req_t	dfu_req; 
	dfu_req.cmd = field_type; 
	dfu_req.p_data = (uint8_t*)p_data; 
	dfu_req.size = len; 
	
	
	uint8_t 	temp[ 12 ]; 
	dfu_res_t 	dfu_res; 
//	dfu_res.p_data = temp + 1; 
    dfu_res.p_data = temp + 2;
	dfu_res.size = 0; 
    dfu_res.rsp_neg_flag = 0;
    
	temp[ 0 ] = COMMAND_ID_FIRMWARE; 
    temp[ 1 ] = 0;  //len aways 0
    
    
    if(field_type==FIRM_KEY_REQ_UPDATE || field_type==FIRM_KEY_VALIDATE || field_type==FIRM_KEY_RESET_DEVICE)//no param resp
    { 
         dfu_req_decode( & dfu_req, & dfu_res );   
        __negative_resp( temp[3] );         
	}
    else
    {
        if( dfu_req_decode( & dfu_req, & dfu_res ) ){
            //temp[ 1 ] = dfu_res.size; 
            //__data_pack( temp, dfu_res.size + 1, FLAG_NORMAL_RESP ); 
            if(dfu_res.rsp_neg_flag ==0){
                //NRF_LOG_INFO("dfu_res.size : %d!!", dfu_res.size);
               __data_pack( temp, dfu_res.size + 2, FLAG_NORMAL_RESP );
                
            }
            else{               
               __negative_resp(DFU_RES_SUCCESS);  
            }                
        }
        else{        
            //__negative_resp( EPB_ERR_INVALID_FORMAT ); 
            __negative_resp( temp[3] ); 
        }
    }
}


#if 1 // GSM send data active
/*
1. heartbeat data 
2. alert data 
3. historical data 
*/

/*
<command> service data 
<imei>	len-key-value
<heartbeat> len-key-value 
*/
// send heart beat keep connect alive 
void proto_pack_get_heart_beat_msg( uint8_array_t * targ )
{
//	// imei 
//	// heartrate beat data 
//	uint8_t temp[30]; 
//	uint16_t chunk_len = 0; 
//	
//	temp[0] = COMMAND_ID_SERVICES;
//	
//	temp[1] = 16; 
//	temp[2] = DEV_CONF_IMEI;
//	memcpy( temp + 3, (uint8_t*)dev_config_get_IMEI(), 15 );  
//	chunk_len += 18; 
//	
//	temp[ chunk_len + 0] = 2;
//	temp[ chunk_len + 1] = SERV_KEY_KEEP_ALIVE; 
//	temp[ chunk_len + 2] = EPB_HEART_BEAT_MAGIC_CODE; 
//	chunk_len += 3; 
//	
//	targ->p_data = 	__send_pack_msg( temp, chunk_len, FLAG_NORMAL_RESP );
//	targ->size = chunk_len + EPB_HEAD_DATA_SIZE ; 
}



// send current gps data & current event 
void proto_pack_get_gps_curr_data_msg( uint8_array_t * targ )
{
//	uint8_t temp[ 50 ]; 
//	uint16_t chunk_len = 0 ; 
//	
//	temp[0] = COMMAND_ID_DATA;
//	
//    // IMEI 
//	temp[1] = 16; 
//	temp[2] = DEV_CONF_IMEI;
//	memcpy( temp + 3, (uint8_t*)dev_config_get_IMEI(), 15 );  
//	chunk_len += 18; 
//	
//    //final location 
//	temp[chunk_len + 1] = DATA_KEY_GPS_LATEST_LOC; 
//	temp[chunk_len + 0] = ls_api_get_final_locate_packet( temp + chunk_len + 2 ) + 1; 	
//	chunk_len += temp[ chunk_len + 0] + 1; 
//    
//    //general data 
//    temp[chunk_len + 0] = 7; 
//    temp[chunk_len + 1] = DATA_KEY_GENERAL_DATA; 
//    temp[chunk_len + 2] = battery_level_get(); 
//    temp[chunk_len + 3] = ls_api_get_gsm_csq(); 
//    uint32_t time = app_rtc_drv_get_run_time(); 
//    temp[chunk_len + 4] = (uint8_t)(time >> 0); 
//    temp[chunk_len + 5] = (uint8_t)(time >> 8); 
//    temp[chunk_len + 6] = (uint8_t)(time >> 16); 
//    temp[chunk_len + 7] = (uint8_t)(time >> 24 ); 
//    chunk_len += 8; 
//	
//	targ->p_data = __send_pack_msg( temp ,chunk_len, FLAG_NORMAL_RESP ); 
//	targ->size = chunk_len + EPB_HEAD_DATA_SIZE; 	
}

void proto_pack_get_alert_msg( uint8_array_t * targ )
{
//    uint8_t temp[ 100 ]; 
//    uint16_t chunk_len = 0; 
//    
//    temp[0] = COMMAND_ID_DATA; 
//    
//    // imei 
//    temp[1] = 16; 
//    temp[2] = DEV_CONF_IMEI; 
//    memcpy( temp + 3, (uint8_t*)dev_config_get_IMEI(), 15 ); 
//    chunk_len += 18; 
//    
//    // GPS location 
//    if( ls_api_get_has_valid_gps_for_upload() ){
//        temp[chunk_len + 1] = DATA_KEY_GPS_ALERT_NOTIFY;  
//        temp[chunk_len + 0] = ls_api_get_final_locate_packet( temp + chunk_len + 2 ) + 1; 	
//        chunk_len += temp[ chunk_len + 0] + 1; 
//    }
//    else{//GSM location 
//        temp[chunk_len + 1] = DATA_KEY_GSM_ALERT_NOTIFY; 
//        temp[chunk_len + 0] = ls_api_get_cell_locator_data( temp + chunk_len + 2) + 1; //56max 
//        chunk_len += temp[ chunk_len + 0] + 1; 
//        
//        // General Flag 
//        temp[chunk_len + 0] = 5; 
//        temp[chunk_len + 1] = DATA_KEY_GENERAL_FLAG;
//        temp[chunk_len + 2] = (uint8_t)( gsflag_get() >> 0);
//        temp[chunk_len + 3] = (uint8_t)( gsflag_get() >> 8); 
//        temp[chunk_len + 4] = (uint8_t)( gsflag_get() >> 16); 
//        temp[chunk_len + 5] = (uint8_t)( gsflag_get() >> 24); 
//        chunk_len += 6; 
//    }
//        
//    //General Data 
//    temp[chunk_len + 0] = 7; 
//    temp[chunk_len + 1] = DATA_KEY_GENERAL_DATA; 
//    temp[chunk_len + 2] = battery_level_get(); 
//    temp[chunk_len + 3] = ls_api_get_gsm_csq(); 
//    uint32_t time = app_rtc_drv_get_run_time(); 
//    temp[chunk_len + 4] = (uint8_t)(time >> 0); 
//    temp[chunk_len + 5] = (uint8_t)(time >> 8); 
//    temp[chunk_len + 6] = (uint8_t)(time >> 16); 
//    temp[chunk_len + 7] = (uint8_t)(time >> 24 ); 
//    chunk_len += 8; 
//    
//    targ->p_data = 	__send_pack_msg( temp, chunk_len, FLAG_ACK_RESP );
//	targ->size = chunk_len + EPB_HEAD_DATA_SIZE ; 
}

// send historical data 
void proto_pack_get_historical_data_msg( uint8_array_t * targ )
{
	uint8_t * tx_buff = (uint8_t*)APP_MALLOC( PROTO_PACK_SEND_HISTORICAL_DATA,  EPB_PAYLOAD_MAX_LEN ); 
	if( tx_buff == NULL ){
		targ->p_data = NULL; 
		return; 
	}
	
	uint16_t chunk_len = 0; 
	
	tx_buff[0] = COMMAND_ID_DATA; 
	
	tx_buff[1] = 5; 
	tx_buff[2] = DATA_KEY_HISTORICAL_DATA; 
	
	//chunk_len = record_data_fetch( tx_buff + 3, EPB_PAYLOAD_MAX_LEN - 3 ); 
	if( chunk_len > 0 ){
		targ->p_data = __send_pack_msg(tx_buff, chunk_len + 3, FLAG_NORMAL_RESP); 
		targ->size = chunk_len + 3 + EPB_HEAD_DATA_SIZE; 
	}
	else{
		targ->p_data = NULL; 
	}
	app_safe_free( tx_buff); 
	tx_buff = NULL; 
}

// parse langtitude & longitude address text 
void proto_pack_req_server_parse_lat_long(uint8_array_t * targ)
{
	int32_t lat, lng; 
//	if( !ls_api_get_final_lng_lat( &lng, &lat ) ){
//		targ->p_data = NULL; 
//		return ; 
//	}
//	
	uint8_t temp[20]; 
	
	temp[0] = COMMAND_ID_SERVICES; 
	 
	temp[1] = 9; 
	temp[2] = SERV_KEY_ADDR_PARSE; 
	uint32_encode( lat, temp + 3); 
	uint32_encode( lng, temp + 7); 
	
	targ->p_data = __send_pack_msg( temp, 11, FLAG_NORMAL_RESP ); 
	targ->size = 11 + EPB_HEAD_DATA_SIZE; 
}

// weak reference 
// define your own handle somewhere 
__WEAK void long_lan_parsed_from_server(const uint8_t* addr, uint16_t len){}
__WEAK void connect_to_server(void){}

#endif 


