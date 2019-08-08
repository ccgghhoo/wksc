

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>
#include <string.h>
//#include "ble_gap.h"
#include "app_util.h"
//#include "app_rtc_drv.h"

#include "dev_conf_defines.h"
#include "dev_conf.pb.h"

#include "dev_config.read.h"
#include "dev_config.factory.h"

//#include "app_fw_info.h"


#if 1 // DEV_CONF PB READ ALL KEYS 
static const uint8_t all_dev_read[] = 
{
	DEV_CONF_MODULE,
	DEV_CONF_FIRMWARE, 
	DEV_CONF_IMEI, 
	DEV_CONF_ICCID, 
	DEV_CONF_MAC,
	DEV_CONF_DATETIME, 
	DEV_CONF_RUNTIME, 
	
	DEV_CONF_SYSTEM_MODE, 
	DEV_CONF_SYSTEM_WORK_TIME,
	DEV_CONF_SYSTEM_NO_DISTURB,
	DEV_CONF_SYSTEM_PASSWORD, 
	DEV_CONF_SYSTEM_TIMEZONE, 
	DEV_CONF_SYSTEM_SWITCHES, 
	DEV_CONF_SYSTEM_VOLUME,
    DEV_CONF_SYSTEM_ALARMCLOCK, 
	
	DEV_CONF_BUTTONS_SENSE, 
	DEV_CONF_BUTTONS_OP, 
	
	DEV_CONF_PHONE_CONTACTS, 
	DEV_CONF_PHONE_SMS_PREFIX, 
	DEV_CONF_PHONE_SOS_OPTION, 
	DEV_CONF_PHONE_SWITCHES, 
	
	DEV_CONF_GPRS_APN, 
	DEV_CONF_GPRS_SERVER_PARAM, 
	DEV_CONF_GPRS_TIME, 
	
	DEV_CONF_ALERT_POWER, 
	DEV_CONF_ALERT_GEO, 
	DEV_CONF_ALERT_MOTION, 
	DEV_CONF_ALERT_STATIC, 
	DEV_CONF_ALERT_SPEED, 
	DEV_CONF_ALERT_TILT, 
	DEV_CONF_ALERT_FALLDOWN, 
}; 
#endif 

uint16_t dev_config_read_max_size(void)
{
	return DEV_CONFIG_DATA_SIZE_MAX; 
}

uint16_t dev_config_read( const uint8_t * p_data, uint8_t len, uint8_t * p_read_out, uint16_t buff_max_size )
{
	if( p_read_out == NULL )
		return 0; 
	
	const uint8_t * pb_keys;
	uint8_t size = 0; 
	
	if( len > 0 ){
		pb_keys = p_data; 
		size = len; 
	}
	else {
		pb_keys = all_dev_read ; 
		size = sizeof( all_dev_read ) ; 
	}
	
	uint8_t index = 0; 
	uint16_t chunk_len = 0; 
	uint16_t remains = buff_max_size; 
	uint8_t * read_data = p_read_out; 
	
	while( index < size )
	{
		switch( pb_keys[ index ++] )
		{
#if 1// Property 			
			case DEV_CONF_MODULE:{
				if( remains < ( 1 + 1 + MODULE_NUMBER_SIZE ) ){
					goto DEV_CONFIG_READ_COMPLETED_LABEL; 
				}
				read_data[ chunk_len + 0] = 5; 
				read_data[ chunk_len + 1] = DEV_CONF_MODULE;
				uint32_encode( PROPERTY_MODULE_NAME, read_data + chunk_len + 2 ); 
				chunk_len += 6; 
				remains -= 6; 
				break; }
			case DEV_CONF_FIRMWARE:{
				if( remains < ( 1 + 1 + FIRMWARE_VERSION_SIZE )){
					goto DEV_CONFIG_READ_COMPLETED_LABEL; 
				}
				
				read_data[ chunk_len + 0] = 5; 
				read_data[ chunk_len + 1] = DEV_CONF_FIRMWARE; 
				uint32_encode( PROPERTY_FIRMWARE_VER, read_data + chunk_len + 2 ); 
				chunk_len += 6; 
				remains -= 6; 
				break; }
			case DEV_CONF_IMEI:{
				if( remains < ( 1 + 1 + DEV_IMEI_DATA_SIZE )){
					goto DEV_CONFIG_READ_COMPLETED_LABEL; 
				}
				
				read_data[ chunk_len + 1] = DEV_CONF_IMEI; 
				if( dev_property.imei[0] == 0 ){
					read_data[ chunk_len + 0] = 1; 
					chunk_len += 2; 
					remains -= 2; 
				}
				else{
					read_data[ chunk_len + 0] = DEV_IMEI_DATA_SIZE + 1; 
					memcpy( read_data + chunk_len + 2, dev_property.imei, DEV_IMEI_DATA_SIZE );  
					chunk_len += 2 + DEV_IMEI_DATA_SIZE; 
					remains -= (2 + DEV_IMEI_DATA_SIZE); 
				}
				
				break; }
			case DEV_CONF_ICCID:{
				if( remains < ( 1 + 1 + DEV_ICCID_DATA_SIZE)) {
					goto DEV_CONFIG_READ_COMPLETED_LABEL; 
				}
				
				read_data[ chunk_len + 1] = DEV_CONF_ICCID; 
				if( dev_property.iccid[0] == 0 ){
					read_data[ chunk_len + 0] = 1; 
					chunk_len += 2; 
					remains -= 2; 
				}
				else{
					read_data[ chunk_len + 0 ] = DEV_ICCID_DATA_SIZE + 1;
					memcpy( read_data + chunk_len + 2, dev_property.iccid, DEV_ICCID_DATA_SIZE) ;
					chunk_len += 2 + DEV_ICCID_DATA_SIZE; 
					remains -= (2 + DEV_ICCID_DATA_SIZE); 
				}				
				break; }
			case DEV_CONF_MAC:{
				if( remains < ( 1 + 1 + 6 )){
					goto DEV_CONFIG_READ_COMPLETED_LABEL; 
				}
//				
//				ble_gap_addr_t gap_addr; 
//				sd_ble_gap_addr_get( &gap_addr ); 
//				
//				read_data[ chunk_len + 0] = 7;
//				read_data[ chunk_len + 1] = DEV_CONF_MAC; 
//				memcpy( read_data + chunk_len + 2, gap_addr.addr, 6 ); 
//				
//				chunk_len += 8; 
//				remains -= 8; 
//				
				break; }
			case DEV_CONF_DATETIME:{
				if( remains < ( 1 + 1 + 4 )){
					goto DEV_CONFIG_READ_COMPLETED_LABEL; 
				}
				
//				read_data[ chunk_len + 0 ] = 5; 
//				read_data[ chunk_len + 1 ] = DEV_CONF_DATETIME; 
//				uint32_encode( app_rtc_drv_get_timestamp(), read_data + chunk_len + 2 ); 
//				chunk_len += 2 + 4; 
//				remains -= 2 + 4; 				
//				break; }
//			case DEV_CONF_RUNTIME:{
//				if( remains < ( 1 + 1 + 4 )){
//					goto DEV_CONFIG_READ_COMPLETED_LABEL; 
//				}
//				
//				read_data[ chunk_len + 0 ] = 5; 
//				read_data[ chunk_len + 1 ] = DEV_CONF_RUNTIME; 
//				uint32_encode( app_rtc_drv_get_run_time(), read_data + chunk_len + 2 ); 
//				chunk_len += 2 + 4; 
//				remains -= 2 + 4; 
//				break;}
//			case DEV_CONF_FIRMWARE_INFO:{
//				if( remains < (1 + 1 + sizeof( app_infor_t ))){
//					goto DEV_CONFIG_READ_COMPLETED_LABEL; 
//				}
//				
//				extern const app_infor_t appl_info; 
//				read_data[ chunk_len + 0 ] = sizeof( app_infor_t ) + 1; 
//				read_data[ chunk_len + 1 ] = DEV_CONF_FIRMWARE_INFO; 
//				memcpy( read_data  + chunk_len + 2, (uint8_t*)&appl_info, sizeof(app_infor_t));
//				
//				chunk_len += sizeof( app_infor_t ) + 2;
//				remains -= 2 + sizeof( app_infor_t ); 
				break; }
#endif // !Property  
#if 1 // System Config	
			case DEV_CONF_SYSTEM_MODE:{
				if( remains < ( 1 + 1 + 4 )) {
					goto DEV_CONFIG_READ_COMPLETED_LABEL; 
				}
				
				read_data[ chunk_len + 0] = 5; 
				read_data[ chunk_len + 1] = DEV_CONF_SYSTEM_MODE; 
				uint32_encode( dev_settings.system.mode.value, read_data + chunk_len + 2 ); 
				
				chunk_len += 2 + 4; 
				remains -= 2 + 4; 
				break; }
			case DEV_CONF_SYSTEM_WORK_TIME:{
				if( remains < ( 1 + 1 + 4 )){
					goto DEV_CONFIG_READ_COMPLETED_LABEL; 
				}
				
				read_data[ chunk_len + 0 ] = 5; 
				read_data[ chunk_len + 1 ] = DEV_CONF_SYSTEM_WORK_TIME; 
				uint16_encode( dev_settings.system.work_time.start_hm, read_data + chunk_len + 2 ); 
				uint16_encode( dev_settings.system.work_time.stop_hm, read_data + chunk_len + 4 ); 
				
				chunk_len += 2 + 4; 
				remains -= 2 + 4; 
				break;}
			case DEV_CONF_SYSTEM_NO_DISTURB:{
				if( remains < ( 1 + 1 + 4 )){
					goto DEV_CONFIG_READ_COMPLETED_LABEL; 
				}
				
				read_data[ chunk_len + 0 ] = 5; 
				read_data[ chunk_len + 1 ] = DEV_CONF_SYSTEM_NO_DISTURB; 
				uint16_encode( dev_settings.system.no_disturb.start_hm, read_data + chunk_len + 2 ); 
				uint16_encode( dev_settings.system.no_disturb.stop_hm, read_data + chunk_len + 4 ); 
				
				chunk_len += 2 + 4; 
				remains -= 2 + 4; 
				break;}
			case DEV_CONF_SYSTEM_PASSWORD:{
				if( remains < ( 1 + 1 + 4 )){
					goto DEV_CONFIG_READ_COMPLETED_LABEL; 
				}
				
				read_data[ chunk_len + 0] = 5; 
				read_data[ chunk_len + 1] = DEV_CONF_SYSTEM_PASSWORD; 
				uint32_encode( dev_settings.system.password.value, read_data + chunk_len + 2 ); 
				
				chunk_len += 2 + 4; 
				remains -= 2 + 4; 
				break; }
			case DEV_CONF_SYSTEM_TIMEZONE:{
				if( remains < ( 1 + 1 + 1 )){
					goto DEV_CONFIG_READ_COMPLETED_LABEL; 
				}
				
				read_data[ chunk_len + 0]= 2; 
				read_data[ chunk_len + 1] = DEV_CONF_SYSTEM_TIMEZONE; 
				read_data[ chunk_len + 2] = (uint8_t)(dev_settings.system.zone.offset );
				
				chunk_len += 2 + 1; 
				remains -= 2 + 1; 
				break; }
			case DEV_CONF_SYSTEM_SWITCHES:{
				if( remains < (1 + 1 + 4 )){
					goto DEV_CONFIG_READ_COMPLETED_LABEL; 
				}
				
				read_data[ chunk_len + 0] = 5;
				read_data[ chunk_len + 1] = DEV_CONF_SYSTEM_SWITCHES; 
				uint32_encode( dev_settings.system.switches.value, read_data + chunk_len + 2 ); 
				
				chunk_len += 2 + 4; 
				remains -= 2 + 4; 				
				break; }
			case DEV_CONF_SYSTEM_VOLUME:{
				if( remains < ( 1 + 1 + 3 )){
					goto DEV_CONFIG_READ_COMPLETED_LABEL; 
				}
				
				read_data[ chunk_len + 0] = 4; 
				read_data[ chunk_len + 1] = DEV_CONF_SYSTEM_VOLUME; 
				read_data[ chunk_len + 2] = dev_settings.system.volume.ringtone;
				read_data[ chunk_len + 3] = dev_settings.system.volume.mic; 
				read_data[ chunk_len + 4] = dev_settings.system.volume.speaker;				
				
				chunk_len += 2 + 3; 
				remains -= 2 + 3; 				
				break; }

            case DEV_CONF_SYSTEM_ALARMCLOCK:{
                uint32_t i = 0; 
                
//                while( i < CLOCKS_NUM ){
//                    if( remains < ( 1 + 1 + 5 )){
//                        goto DEV_CONFIG_READ_COMPLETED_LABEL; 
//                    }
//                    
//                    read_data[ chunk_len + 0] = 6; 
//                    read_data[ chunk_len + 1] = DEV_CONF_SYSTEM_ALARMCLOCK; 
//                    read_data[ chunk_len + 2] = dev_settings.system.alarms[i].flag;
//                    read_data[ chunk_len + 3] = dev_settings.system.alarms[i].hour; 
//                    read_data[ chunk_len + 4] = dev_settings.system.alarms[i].minute; 
//                    read_data[ chunk_len + 5] = dev_settings.system.alarms[i].weekday; 
//                    read_data[ chunk_len + 6] = dev_settings.system.alarms[i].ring_time;    
//                    chunk_len += 2 + 5; 
//                    remains -= 2 + 5; 
//                    i ++; 
//                }
                break;}
#endif // !System Config 
#if 1 // Buttons Config 			
			case DEV_CONF_BUTTONS_SENSE:{
				if( remains < (1 + 1 + 8) ){
					goto DEV_CONFIG_READ_COMPLETED_LABEL; 
				}
				
				read_data[ chunk_len + 0] = 9; 
				read_data[ chunk_len + 1] = DEV_CONF_BUTTONS_SENSE; 
				uint16_encode( dev_settings.buttons.sense.click,  read_data + chunk_len + 2 ); 
				uint16_encode( dev_settings.buttons.sense.double_click, read_data + chunk_len + 4 ); 
				uint16_encode( dev_settings.buttons.sense.short_hold, read_data + chunk_len + 6 ); 
				uint16_encode( dev_settings.buttons.sense.long_hold, read_data + chunk_len + 8 ); 
				
				chunk_len += 2 + 8; 
				remains -= 2 + 8; 				
				break; }
			case DEV_CONF_BUTTONS_OP:{
				if( remains < ( 1 + 1 + 28 ) ){
					goto DEV_CONFIG_READ_COMPLETED_LABEL; 
				}
				
				read_data[ chunk_len + 0] = 28 + 1; 
				read_data[ chunk_len + 1] = DEV_CONF_BUTTONS_OP; 
				
				// add 4 bytes offset 
				memcpy( read_data + chunk_len + 2, &dev_settings.buttons.opcode.task[1][0], 28 ); 
				
				chunk_len += 2 + 28; 
				remains -= 2 + 28; 				
				break; }
#endif // !Buttons Config 
#if 1 // Phone Config 
			case DEV_CONF_PHONE_CONTACTS:{
				uint32_t i = 0; 
				
				while( i < PHONE_CONTACTS_NUM ){
					if( remains < ( 1 + 1 + 20 )){
						goto DEV_CONFIG_READ_COMPLETED_LABEL;
					}
										
					read_data[ chunk_len + 1] = DEV_CONF_PHONE_CONTACTS; 
					read_data[ chunk_len + 2] = dev_settings.phone.contacts[i].bit.value;
					uint32_t len = strlen( dev_settings.phone.contacts[i].number ); 
					if( len > PHONE_CONTACT_NUMBER_SIZE){
						memcpy( read_data + chunk_len + 3, dev_settings.phone.contacts[i].number, PHONE_CONTACT_NUMBER_SIZE);
						read_data[ chunk_len + 0] = 20 + 1; 
						chunk_len += 20 + 2; 
						remains -= 20 + 2; 
					}
					else{
						memcpy( read_data + chunk_len + 3, dev_settings.phone.contacts[i].number, len ); 
						read_data[ chunk_len + 0] = len + 1 + 1; 
						chunk_len += len + 1 + 2; 
						remains -= len + 1 + 2; 
					}
					i ++; 
				}				
				break; }
			case DEV_CONF_PHONE_SMS_PREFIX:{
				if( remains < ( 1 + 1 + 19 )){
					goto DEV_CONFIG_READ_COMPLETED_LABEL; 
				}
				
				if( dev_settings.phone.sms_prefix.size > PHONE_SMS_PREFIX_LEN_MAX )
					dev_settings.phone.sms_prefix.size = PHONE_SMS_PREFIX_LEN_MAX; 
				
				read_data[ chunk_len + 0] = dev_settings.phone.sms_prefix.size + 1; 
				read_data[ chunk_len + 1] = DEV_CONF_PHONE_SMS_PREFIX; 
				if( dev_settings.phone.sms_prefix.size > 0 ){
					memcpy(read_data + chunk_len + 2, dev_settings.phone.sms_prefix.text, dev_settings.phone.sms_prefix.size ); 
				}
				
				chunk_len += dev_settings.phone.sms_prefix.size + 2;
				remains -= dev_settings.phone.sms_prefix.size + 2;
				break; }
			case DEV_CONF_PHONE_SOS_OPTION:{
				if( remains < ( 1 + 1 + 4 )){
					goto DEV_CONFIG_READ_COMPLETED_LABEL; 
				}
				
				read_data[ chunk_len + 0] = 5; 
				read_data[ chunk_len + 1] = DEV_CONF_PHONE_SOS_OPTION; 
				uint32_encode( dev_settings.phone.SOS.value, read_data + chunk_len + 2 ); 				
				
				chunk_len += 2 + 4; 
				remains -= 2 + 4; 				
				break; }
			case DEV_CONF_PHONE_SWITCHES:{
				if( remains < ( 1 + 1 + 4 )) {
					goto DEV_CONFIG_READ_COMPLETED_LABEL; 
				}
				
				read_data[ chunk_len + 0] = 5; 
				read_data[ chunk_len + 1] = DEV_CONF_PHONE_SWITCHES; 
				uint32_encode( dev_settings.phone.switches.value, read_data + chunk_len + 2 ); 
				
				chunk_len += 2 + 4; 
				remains -= 2 + 4; 				
				break; }
#endif // !Phone Config 
#if 1 // GPRS Config 
			case DEV_CONF_GPRS_APN:{
				if( remains < ( 1 + 1 + GPRS_APN_APN_LEN_MAX + GPRS_APN_USERNAME_LEN_MAX + GPRS_APN_PASSWORD_LEN_MAX))
				{
					goto DEV_CONFIG_READ_COMPLETED_LABEL; 
				}
				
				if( dev_settings.gprs.apn.apnLen > GPRS_APN_APN_LEN_MAX )
					dev_settings.gprs.apn.apnLen = GPRS_APN_APN_LEN_MAX; 
				if( dev_settings.gprs.apn.userNameLen > GPRS_APN_USERNAME_LEN_MAX )
					dev_settings.gprs.apn.userNameLen = GPRS_APN_USERNAME_LEN_MAX; 
				if( dev_settings.gprs.apn.passwordLen > GPRS_APN_PASSWORD_LEN_MAX )
					dev_settings.gprs.apn.passwordLen = GPRS_APN_PASSWORD_LEN_MAX; 
				
				read_data[ chunk_len + 0] = 1 + 3 + dev_settings.gprs.apn.apnLen + dev_settings.gprs.apn.userNameLen + dev_settings.gprs.apn.passwordLen;
				read_data[ chunk_len + 1] = DEV_CONF_GPRS_APN; 
				
				read_data[ chunk_len + 2] = dev_settings.gprs.apn.apnLen; 
				if( dev_settings.gprs.apn.apnLen > 0 ){
					memcpy( read_data + chunk_len + 3, dev_settings.gprs.apn.apns, dev_settings.gprs.apn.apnLen ); 
				}
				chunk_len += 2 + dev_settings.gprs.apn.apnLen + 1 ; 
				remains -= 2 + dev_settings.gprs.apn.apnLen + 1; 
				
				read_data[ chunk_len + 0] = dev_settings.gprs.apn.userNameLen; 
				if( dev_settings.gprs.apn.userNameLen > 0){
					memcpy( read_data + chunk_len + 1, dev_settings.gprs.apn.username, dev_settings.gprs.apn.userNameLen ); 					
				}
				chunk_len += dev_settings.gprs.apn.userNameLen + 1; 
				remains -= dev_settings.gprs.apn.userNameLen + 1; 
				
				read_data[ chunk_len + 0] = dev_settings.gprs.apn.passwordLen; 
				if( dev_settings.gprs.apn.passwordLen > 0){
					memcpy( read_data + chunk_len + 1, dev_settings.gprs.apn.password, dev_settings.gprs.apn.passwordLen ); 
				}
				chunk_len += dev_settings.gprs.apn.passwordLen + 1; 
				remains -= dev_settings.gprs.apn.passwordLen + 1; 			
								
				break; }
			case DEV_CONF_GPRS_SERVER_PARAM:{
				if( remains < ( 1 + 1 + 32 )){
					goto DEV_CONFIG_READ_COMPLETED_LABEL; 
				}			
				
				uint32_t ipLen = strlen( dev_settings.gprs.server.ip ); 
				if( ipLen > 30)
					ipLen = 30; 
				
				read_data[ chunk_len + 0] = ipLen + 1 + 4; 
				read_data[ chunk_len + 1] = DEV_CONF_GPRS_SERVER_PARAM; 
				
                uint16_encode( dev_settings.gprs.server.flag, read_data + chunk_len + 2 ); 
				uint16_encode( dev_settings.gprs.server.port, read_data + chunk_len + 4 ); 
				if( ipLen > 0 ){
					memcpy( read_data + chunk_len + 6, dev_settings.gprs.server.ip, ipLen ); 
				}
				chunk_len += ipLen + 6; 
				remains -= ipLen + 6; 				
				break; }
			case DEV_CONF_GPRS_TIME:{
				if( remains < (1 + 1 + 12 )){
					goto DEV_CONFIG_READ_COMPLETED_LABEL; 
				}
				
				read_data[ chunk_len + 0] = 13; 
				read_data[ chunk_len + 1] = DEV_CONF_GPRS_TIME; 
				uint32_encode( dev_settings.gprs.interval.heartbeat, read_data + chunk_len + 2 ); 
				uint32_encode( dev_settings.gprs.interval.upload, read_data + chunk_len + 6 ); 
				uint32_encode( dev_settings.gprs.interval.lazy_upload, read_data + chunk_len + 10 ); 
				
				chunk_len += 1 + 13; 
				remains -= 1 + 13; 				
				break; }
#endif // !GPRS Config 
#if 1 // Alert Config 
			case DEV_CONF_ALERT_POWER:{
				if( remains < ( 1 + 1 + 1 )){
					goto DEV_CONFIG_READ_COMPLETED_LABEL; 
				}
				
				read_data[ chunk_len + 0] = 2; 
				read_data[ chunk_len + 1] = DEV_CONF_ALERT_POWER; 
				read_data[ chunk_len + 2] = (uint8_t)dev_settings.alert.power.value; 
				
				chunk_len += 1 + 2; 
				remains -= 1 + 2; 				
				break; }
			case DEV_CONF_ALERT_GEO:{
				uint32_t i = 0; 
				uint16_t offset = 0; 
				
				while( i < ALERT_GEO_NUMBERS )
				{
					if( remains < ( 1 + 1 + ALERT_GEO_DATA_SIZE_MAX )) {
						goto DEV_CONFIG_READ_COMPLETED_LABEL; 
					}
					
					if( dev_settings.alert.geos[i].count > ALERT_GEO_FENCE_POINTS )
						dev_settings.alert.geos[i].count = 0; 
					
					read_data[ chunk_len + 1] = DEV_CONF_ALERT_GEO; 
					uint32_encode( dev_settings.alert.geos[i].value, read_data + chunk_len + 2 ); 
					
					offset = 1 + 1 + 4; 					
					for( uint32_t j = 0 ; j < dev_settings.alert.geos[i].count; j++){
						uint32_encode(dev_settings.alert.geos[i].data[ 2 * j],  read_data + chunk_len + offset);
						uint32_encode(dev_settings.alert.geos[i].data[ 2 * j + 1], read_data + chunk_len + offset + 4 ); 
						offset += 8; 
					}
					
					read_data[ chunk_len + 0] =  offset - 1; 
					chunk_len += offset;
					remains -= offset; 		
					
					i ++; 
				}
				break; }
			case DEV_CONF_ALERT_MOTION:{
				if( remains < ( 1 + 1 + 4 )) {
					goto DEV_CONFIG_READ_COMPLETED_LABEL; 
				}
				
				read_data[ chunk_len + 0 ] = 5; 
				read_data[ chunk_len + 1 ] = DEV_CONF_ALERT_MOTION; 
				uint32_encode( dev_settings.alert.motion.value, read_data + chunk_len + 2 ); 
				
				chunk_len += 1 + 5; 
				remains -= 1 + 5; 				
				break; }
			case DEV_CONF_ALERT_STATIC:{
				if( remains < ( 1 + 1 + 4 )) {
					goto DEV_CONFIG_READ_COMPLETED_LABEL; 
				}
				
				read_data[ chunk_len + 0 ] = 5; 
				read_data[ chunk_len + 1 ] = DEV_CONF_ALERT_STATIC; 
				uint32_encode( dev_settings.alert.actionless.threshold, read_data + chunk_len + 2 ); 
				
				chunk_len += 1 + 5; 
				remains -= 1 + 5; 				
				break; }
			case DEV_CONF_ALERT_SPEED:{
				if( remains < ( 1 + 1 + 4 )) {
					goto DEV_CONFIG_READ_COMPLETED_LABEL; 
				}
				
				read_data[ chunk_len + 0 ] = 5; 
				read_data[ chunk_len + 1 ] = DEV_CONF_ALERT_SPEED; 
				uint32_encode( dev_settings.alert.speed.threshold, read_data + chunk_len + 2 ); 
				
				chunk_len += 1 + 5; 
				remains -= 1 + 5; 
				break; }
			case DEV_CONF_ALERT_TILT:{
				if( remains < ( 1 + 1 + 4 )) {
					goto DEV_CONFIG_READ_COMPLETED_LABEL; 
				}
			
				read_data[ chunk_len + 0 ] = 5; 
				read_data[ chunk_len + 1 ] = DEV_CONF_ALERT_TILT; 
				uint32_encode( dev_settings.alert.tilt.value, read_data + chunk_len + 2 ); 
				
				chunk_len += 1 + 5 ;
				remains -= 1 + 5; 				
				break; }
			case DEV_CONF_ALERT_FALLDOWN:{
				if( remains < ( 1 + 1 + 4 )) {
					goto DEV_CONFIG_READ_COMPLETED_LABEL; 
				}
				
				read_data[ chunk_len + 0 ] = 5; 
				read_data[ chunk_len + 1 ] = DEV_CONF_ALERT_FALLDOWN; 
				uint32_encode( dev_settings.alert.falldown.value, read_data + chunk_len + 2 ); 
				
				chunk_len += 1 + 5; 
				remains -= 1 + 5; 
				
				break; }
#endif // !Alert Config 
			default:
				goto DEV_CONFIG_READ_COMPLETED_LABEL; 
				//break; 
		}
	}
	
DEV_CONFIG_READ_COMPLETED_LABEL:

	
	return chunk_len ; 
}

