

#include "app_heap.h"

#include "protocol.h"
#include "proto.pack.h"

#include "dev_conf.h"
#include "dev_conf.pb.h"
#include "dev_config.write.h"
#include "dev_config.read.h"

#include "crc16.h"
#include "record.h"
#include "alert.h"

#include "mode.h"


#if 1
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#define PB_LOG				NRF_LOG_DEBUG
#else
#define PB_LOG(...)
#endif 

// payload sequence 
#define COMMAND_OFFSET			0
#define LENGTH_OFFSET			1 
#define KEY_OFFSET				2 
#define VALUE_OFFSET			3  

extern resp_callback_t			m_resp_cb; 
extern msg_packet_t* 			mp_msg_ptr;


__WEAK void server_ack_alert_msg(void)
{
    // do nothing 
}

static void dev_record_data_consume( const uint8_t * p_data, uint16_t len )
{
	if( len < 3 ){
		__negative_resp( EPB_ERR_INVALID_LENGTH ); 
		return ; 
	}
	
	uint8_t field_len = p_data[ LENGTH_OFFSET ]; 
	uint8_t field_type = p_data[ KEY_OFFSET ]; 
	
	
	switch( field_type )
	{
		case DATA_KEY_GPS_LATEST_LOC:
			if( !proto_pack_gps_latest_data()){
				__negative_resp( EPB_ERR_GPS_IS_NOT_READY ); 
			}
			break;
		case DATA_KEY_HISTORICAL_DATA:
			if( field_len == 5 ){
				uint32_t addr = uint32_decode( p_data + VALUE_OFFSET );
				if( !record_data_is_received( addr ) ){
					__negative_resp( EPB_ERR_ADDR_RESP ); 
					break; 
				}
			}
			else if( field_len != 1 ){
				__negative_resp( EPB_ERR_INVALID_FORMAT );
				break; 
			}
			else{
				// do nothing 
			}
			if( !proto_pack_historical_data() ){
				__negative_resp( EPB_ERR_NO_MEM ) ;
			}
			break; 
            
        case DATA_KEY_GPS_ALERT_NOTIFY:
        case DATA_KEY_GSM_ALERT_NOTIFY:
            server_ack_alert_msg(); 
            break; 
		default:
			break; 
	}
}


static void dev_config_data_consume( const uint8_t * p_data, uint16_t len ) 
{
	if( len < 3){ // command/len/key as least 
		__negative_resp( EPB_ERR_INVALID_LENGTH ); 
		return ; 
	}		
	
	uint8_t field_len = p_data[ LENGTH_OFFSET ]; 
	uint8_t field_type = p_data[ KEY_OFFSET ]; 
	if( field_type == DEV_CONF_READ_SETTINGS ){
		uint8_t * p_read_settings = (uint8_t*)APP_MALLOC(PROTO_READ_CONFIG_SETTING,  dev_config_read_max_size() + 1  ); 
		if( p_read_settings == NULL ){
			__negative_resp( EPB_ERR_RESOURCE_INSUFFICIENT );
			return; 
		}
		
		if( field_len == 0 ){
			__negative_resp( EPB_ERR_INVALID_LENGTH ); 
			return; 
		}
		
		p_read_settings[ 0 ] = COMMAND_ID_CONFIG; 
		uint16_t ret_len = dev_config_read( p_data + VALUE_OFFSET, field_len - 1, p_read_settings + 1, dev_config_read_max_size() + 1 ); 
		if( ret_len > 0 ){
			__data_pack( p_read_settings, ret_len + 1, FLAG_NORMAL_RESP ); 
			app_safe_free( p_read_settings ); 
		}
		else{
			__negative_resp( EPB_ERR_SUB_FUNCTION_NOT_SUPPORTED ); 
		}
	}
	else{
		bool success = dfw_settings_update( p_data + LENGTH_OFFSET, len - 1 );
		if( !success ){
			__negative_resp( EPB_ERR_SUB_FUNCTION_NOT_SUPPORTED ); 
		}
	}
		
}

__WEAK bool loc_gps_util_enable_movement(uint32_t threshold)
{
    return true; 
}

static void dev_service_data_consume( const uint8_t * p_data, uint16_t len )
{
	uint8_t field_len = p_data[ LENGTH_OFFSET ]; 
	uint8_t field_type = p_data[ KEY_OFFSET ]; 
	if( ( field_len + 1 + 1 ) > len) {
		__negative_resp( EPB_ERR_INVALID_LENGTH ); 
		return ;
	}
	
	switch( field_type )
	{
		case SERV_KEY_ADDR_PARSE:
			if( field_len < 2 ){
				__negative_resp( EPB_ERR_INVALID_FORMAT );
				break; 
			}
			long_lan_parsed_from_server( p_data + VALUE_OFFSET, field_len - 1 ); 
			break; 
		case SERV_KEY_CONN_SERVER:
			if( field_len != 1 ){
				__negative_resp( EPB_ERR_INVALID_FORMAT );
				break; 
			}
			connect_to_server();
			break; 
        case SERV_KEY_DISTANCE_ALERT:{
            if( field_len == 5 ){
                uint32_t threshold = uint32_decode( p_data + VALUE_OFFSET ); 
                if( !loc_gps_util_enable_movement( threshold ) ){
                    __negative_resp( EPB_ERR_INVALID_FORMAT );
                }
            }
            break; }
		case SERV_KEY_FIND_ME:
			if( field_len != 1 ){
				__negative_resp( EPB_ERR_INVALID_FORMAT );
				break; 
			}
			// find me alert 
			alert_drv_evt_update( ALERT_TYPE_FINDME ); 
			break; 
		default:
			break; 
	}
}

static void dev_system_control_data_consume( const uint8_t * p_data, uint16_t len)
{
	uint8_t field_len = p_data[ LENGTH_OFFSET ]; 
	uint8_t field_type = p_data[ KEY_OFFSET ]; 
	if( ( field_len + 1 + 1 ) > len) {
		__negative_resp( EPB_ERR_INVALID_LENGTH ); 
		return ;
	}
	
	switch( field_type )
	{
		case SYS_KEY_RESET_RECORD:
			record_data_flush();
			break;
		
		case SYS_KEY_FACTORY_RECOVERY:
			dev_conf_reset_to_factory();
			break; 		
		
		case SYS_KEY_DEVICE_RESET:
			device_mode_set( RESET_AS_NORMAL ); 
			break; 
		
		case SYS_KEY_DEVICE_RESET_DIRECTLY:
			device_mode_set( RESET_AS_DIRECTLY ); 
			break; 		
		
		case SYS_KEY_ENTER_DFU:
			device_mode_set( RESET_TO_DFU_MODE ); 
			break; 
		
		case SYS_KEY_POWER_OFF:
			device_mode_set( ENTER_POWER_OFF_MODE ); 
			break; 
		
		default:
			break; 
	}
}

static void dev_memory_read_consume( const uint8_t * p_data, uint16_t len )
{
	uint8_t field_len = p_data[ LENGTH_OFFSET ] ; 
	uint8_t field_type = p_data[ KEY_OFFSET ]; 
	if( field_len != 7 )
	{
		__negative_resp( EPB_ERR_INVALID_LENGTH ); 
		return; 
	}
	
	proto_pack_memory_reader_process( field_type, p_data + VALUE_OFFSET, field_len - 1 ); 
}

static void dev_firmware_data_consume( const uint8_t * p_data, uint16_t len )
{
	// cmd - key - value ( no length of this command ) 
//	uint8_t field_len = p_data[ LENGTH_OFFSET ]; 
	
	if( len < 2) {
		__negative_resp( EPB_ERR_INVALID_LENGTH ); 
		return ;
	}
	uint8_t field_type = p_data[ 1 ]; 
	
	// command id / field len / keyword / value 
	proto_pack_dfu_process( field_type, p_data + 2, len - 2 ); 
}

static void dev_factory_test_data_consume( const uint8_t * p_data, uint16_t len )
{
	uint8_t field_len = p_data[ LENGTH_OFFSET ]; 
	uint8_t field_type = p_data[ KEY_OFFSET ]; 
	if( ( field_len + 1 + 1 ) > len) {
		__negative_resp( EPB_ERR_INVALID_LENGTH ); 
		return ;
	}
	
	switch( field_type )
	{
		case FACTORY_KEY_START_TEST:
			break;
		case FACTORY_KEY_STOP_TEST:
			break; 
		
		default:
			break; 
	}
}


void protocol_data_receive_consume(msg_packet_t * p_msg, resp_callback_t evt_handle)
{
	if( p_msg == NULL || p_msg->payload == NULL || evt_handle == NULL)
		return; 
	
	uint16_t crc16 = crc16_compute( p_msg->payload, p_msg->len, 0);
	
	if( crc16 != p_msg->crc )
		return; 
	
	m_resp_sent = false; 
	m_resp_cb =  evt_handle ; 
	mp_msg_ptr = p_msg ;
	
	switch( p_msg->payload[ COMMAND_OFFSET ] )
	{
		default:
			break; 
		
		case COMMAND_ID_DATA:
			dev_record_data_consume( p_msg->payload, p_msg->len ); 
			break; 
		
		case COMMAND_ID_CONFIG:
			dev_config_data_consume( p_msg->payload, p_msg->len ); 
			break; 
		
		case COMMAND_ID_SERVICES:
			dev_service_data_consume( p_msg->payload, p_msg->len ); 
			break; 
		
		case COMMAND_ID_SYS_CONTROL:
			dev_system_control_data_consume( p_msg->payload, p_msg->len ); 
			break; 
		
		case COMMAND_ID_MEMORY_READER:
			dev_memory_read_consume( p_msg->payload, p_msg->len ); 
			break; 
		
		case COMMAND_ID_FIRMWARE:
			dev_firmware_data_consume( p_msg->payload, p_msg->len ); 
			break; 
		
		case COMMAND_ID_FACOTRY_TEST:
			dev_factory_test_data_consume( p_msg->payload, p_msg->len ); 
			break; 
	}
	
	// if need ack, send an ack response when no response has been sent 
	if( p_msg->flag.ack > 0 && !m_resp_sent ){
		__ack_resp(); 
	}
}


