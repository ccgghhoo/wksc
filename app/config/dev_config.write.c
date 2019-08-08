

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "app_util.h"

#include "dev_conf_defines.h"
#include "dev_conf.pb.h"
//#include "conf_file.h"

#if 0
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#define SMS_LOG			NRF_LOG_INFO
#define SMS_STR_LOG		NRF_LOG_HEXDUMP_INFO
#else
#define SMS_LOG(...)
#define SMS_STR_LOG(...)
#endif 


#define HOUR_VALIDATE(x)	( (x)  < 24 )
#define MINUTE_VALIDATE(x)	( (x) <  60 ) 


#if 1 // property update 

void dfw_property_imei_update( char * imei )
{
	uint32_t len = strlen( imei ); 
	if( len != DEV_IMEI_DATA_SIZE )
		return; 
	
	strcpy( dev_property.imei, imei ); 
	SMS_LOG("[DFW]: imei update!!!\r\n"); 
}
void dfw_property_iccid_update( char* iccid )
{
	uint32_t len = strlen( iccid );
	if( len != DEV_ICCID_DATA_SIZE) {
		return; 
	}
	strcpy( dev_property.iccid, iccid );
	SMS_LOG("[DFW]: iccid update!!!\r\n"); 
}


#endif 

#if 1  // System Settings 
bool dfw_system_mode_update(uint8_t mode, uint32_t time )
{
	if( mode < SYSTEM_MODES_MODE_START 
		|| mode > SYSTEM_MODES_MODE_END){
		return false; 
	}
	
	if( time < SYSTEM_MODE_TIME_MIN || time > SYSTEM_MODE_TIME_MAX){
		return false; 
	}
	
	dev_settings.system.mode.mode = mode; 
	dev_settings.system.mode.time = time; 
	m_dev_cb( DEV_UPDATE_WORK_MODE ); 
	
	SMS_LOG("[DFW]: system mode update!!!\r\n"); 
	return true; 
}

bool dfw_system_work_time_update( uint8_t start_hour, uint8_t start_minute, uint8_t stop_hour, uint8_t stop_minute )
{
	if( !HOUR_VALIDATE( start_hour ) || !HOUR_VALIDATE( stop_hour ) 
	||	!MINUTE_VALIDATE(start_minute ) || !MINUTE_VALIDATE( stop_minute )) {
		return false; 
	}
	
	dev_settings.system.work_time.start_hour = start_hour; 
	dev_settings.system.work_time.start_minute = start_minute; 
	dev_settings.system.work_time.end_hour = stop_hour; 
	dev_settings.system.work_time.end_minute = stop_minute; 
	
	return true; 
}

bool dfw_system_do_not_disturb_update( uint8_t start_hour, uint8_t start_minute, uint8_t stop_hour, uint8_t stop_minute )
{
	if( !HOUR_VALIDATE( start_hour ) || !HOUR_VALIDATE( stop_hour ) 
	||	!MINUTE_VALIDATE(start_minute ) || !MINUTE_VALIDATE( stop_minute )) {
		return false; 
	}
	
	dev_settings.system.no_disturb.start_hour = start_hour; 
	dev_settings.system.no_disturb.start_minute = start_minute; 
	dev_settings.system.no_disturb.end_hour = stop_hour; 
	dev_settings.system.no_disturb.end_minute = stop_minute; 
    
    m_dev_cb( DEV_UPDATE_NO_DISTURB ); 
		
	return true; 
}


bool dfw_system_timezone_update(int8_t offset)
{	
	if( offset > SYSTEM_TIMEZONE_MAX_OFFSET || offset < SYSTEM_TIMEZONE_MIN_OFFSET )
			return false; 
		
    if( dev_settings.system.zone.offset != offset){
        m_dev_cb( DEV_UPDATE_TIMEZONE ); 
    }
	dev_settings.system.zone.offset = offset; 	
	
	SMS_LOG("[DFW]: timezone update!!!\r\n"); 
	return true; 
}
bool dfw_system_password_update(bool enable, uint32_t value)
{
	if( enable ){
		if( value > SYSTEM_PASSWORD_MAX_NUMBER )
			return false; 
		
		dev_settings.system.password.enable = 1; 
		if( value > 0 )
			dev_settings.system.password.value = value; 
	}
	else{
		dev_settings.system.password.enable = 0; 
	}

	SMS_LOG("[DFW]: password update!!!\r\n"); 
	return true; 
}
bool dfw_system_password_protect(bool enable)
{
	dev_settings.system.password.enable = enable; 
	SMS_LOG("[DFW]: password update!!!\r\n"); 
	return true; 
}

void dfw_system_switch_leds( bool on ){
	dev_settings.system.switches.leds = on ? 1 : 0; 
//	m_dev_cb( DEV_CONF_SYSTEM_SWITCHES ); 
	
	SMS_LOG("[DFW]: leds update = %d !!!\r\n", on); 
}
void dfw_system_switch_motor( bool on ){
	dev_settings.system.switches.motor = on ? 1 : 0; 
//	m_dev_cb( DEV_CONF_SYSTEM_SWITCHES ); 
	SMS_LOG("[DFW]: motor update = %d !!!\r\n", on); 
}
void dfw_system_switch_buzzer( bool on ){
	dev_settings.system.switches.buzzer = on ? 1 : 0; 
//	m_dev_cb( DEV_CONF_SYSTEM_SWITCHES ); 
	SMS_LOG("[DFW]: buzzer update = %d !!!\r\n", on); 
}
void dfw_system_switch_agps(bool on ){
	dev_settings.system.switches.AGPS = on ? 1 : 0 ;
//	m_dev_cb( DEV_CONF_SYSTEM_SWITCHES ); 
	SMS_LOG("[DFW]: AGPS update = %d !!!\r\n", on); 
}

bool dfw_system_switches_update( uint32_t value )
{
	dev_settings.system.switches.value = value; 
//	m_dev_cb( DEV_CONF_SYSTEM_SWITCHES ); 
	
	SMS_LOG("[DFW]: system switches update = 0x%X!!!\r\n", value); 
	return true; 
}

bool dfw_system_volume_ringtone_update( uint8_t level )
{
	if( level <= SYSTEM_RINGTONE_LEVEL_MAX ){
        if( dev_settings.system.volume.ringtone != level ){
            m_dev_cb( DEV_UPDATE_VOLUME_RINGTONE );
        }
		dev_settings.system.volume.ringtone = level; 		
		return true; 
	}
	return false; 	
}
bool dfw_system_volume_mic_update( uint8_t level )
{
	if( level <= SYSTEM_MIC_LEVEL_MAX )
	{
        if( dev_settings.system.volume.mic != level ){
            m_dev_cb( DEV_UPDATE_VOLUME_MIC ); 
        }
		dev_settings.system.volume.mic = level; 
		
		return true; 
	}
	return false; 	
}
bool dfw_system_volume_speaker_update( uint8_t level )
{
	if( level <= SYSTEM_SPEAKER_LEVEL_MAX )
	{
        if( dev_settings.system.volume.speaker != level ){
            m_dev_cb( DEV_UPDATE_VOLUME_SPEAKER ); 
        }
		dev_settings.system.volume.speaker = level; 		
		return true;
	}
	return false; 	 
}

/*
bool dfw_system_dev_name_update( const uint8_t * p_data, uint8_t len)
{
	if( len > SYSTEM_DEVICE_NAME_LEN_MAX )
		return false; 
	
	if( len > 0 && p_data == NULL )
		return false; 
	
	dev_settings.system.dev_name.size = len; 
	memcpy( dev_settings.system.dev_name.val, p_data, len ); 
	m_dev_cb( DEV_CONF_SYSTEM_DEVICE_NAME ); 
	return true; 
}*/

//bool dfw_system_alarmclock_update( alarm_clock_t * alarm )
//{
//    if( alarm == NULL )
//        return false; 
//    
//    if( !( HOUR_VALIDATE( alarm->hour ) && MINUTE_VALIDATE( alarm->minute ))){
//        return false; 
//    }
//    
//    if( (alarm->flag & CLOCKS_NUM_MASK ) >= CLOCKS_NUM )
//        return false; 
//    
//    uint32_t idx = alarm->flag & CLOCKS_NUM_MASK; 
//    dev_settings.system.alarms[ idx ] = * alarm; 
//    
//    return true; 
//}

#endif // !System 

#if 1 //Buttons 

bool dfw_buttons_sense_update( const uint8_t * p_data, uint8_t size )
{
	if( size != BUTTONS_SENSE_SIZE) 
		return false; 
	
	uint16_t click = uint16_decode( p_data ); 
	uint16_t double_click = uint16_decode( p_data + 2 ); 
	uint16_t short_hold = uint16_decode( p_data + 4 ); 
	uint16_t long_hold = uint16_decode( p_data + 6 ); 
	
	if( click < BUTTONS_CLICK_TIME_MIN || click > short_hold )
		return false; 
	if( double_click > BUTTONS_DOUBLE_CLICK_TIME_MAX )
		return false; 
	if( short_hold < BUTTONS_SHORT_HOLD_TIME_MIN || short_hold > long_hold )
		return false; 
	if( long_hold < BUTTONS_LONG_HOLD_TIME_MIN || long_hold > BUTTONS_LONG_HOLD_TIME_MAX )
		return false; 
	
	dev_settings.buttons.sense.click = click; 
	dev_settings.buttons.sense.double_click = double_click; 
	dev_settings.buttons.sense.short_hold = short_hold; 
	dev_settings.buttons.sense.long_hold = long_hold; 
	
	m_dev_cb( DEV_UPDATE_BUTTONS_SENSE ); 
	return true; 
}


bool dfw_buttons_sos_active_ways_as( uint8_t button_evt )
{
	if( button_evt != BUTTONS_EVT_DOUBLE_CLICK || button_evt != BUTTONS_EVT_SHORT_HOLD )
		return false; 
	
	uint8_t other = BUTTONS_EVT_SHORT_HOLD; 
	if( button_evt == BUTTONS_EVT_SHORT_HOLD )
		other = BUTTONS_EVT_DOUBLE_CLICK; 
	
	dev_settings.buttons.opcode.task[ BUTTONS_IDX_PRIM ][ button_evt] = BUTTONS_OP_SOS_ALERT; 
	if( dev_settings.buttons.opcode.task[ BUTTONS_IDX_PRIM ][ other ] == BUTTONS_OP_SOS_ALERT ){
		dev_settings.buttons.opcode.task[ BUTTONS_IDX_PRIM ][ other ] = BUTTONS_OP_DO_NOTHINGS; 
	}
	
	return true; 
}
bool dfw_buttons_side_button_dial_no( uint8_t contact )
{
	if( contact > PHONE_CONTACTS_NUM )
		return false; 
	
	dev_settings.buttons.opcode.task[ BUTTONS_IDX_SIDEL ][ BUTTONS_EVT_CLICK ] = contact +  BUTTONS_OP_DIAL_1ST_NUMBER;
	
	return true; 
}

bool dfw_buttons_op_update( uint8_t idx, uint8_t evt_type, uint8_t task )
{
//	button_action_t action ;
//	action.value = op; 
//	uint8_t idx, evt, task; 
//	idx = (op >> 12) & 0x0f;
//	evt = (op >> 8) & 0x0f; 
//	task = (op & 0xff); 
	
	if( idx < BUTTONS_OP_INDEX_MIN || idx > BUTTONS_OP_INDEX_MAX )
		return false; 
	if( evt_type < BUTTONS_OP_EVENT_MIN || evt_type > BUTTONS_OP_EVENT_MAX )
		return false; 
	
	dev_settings.buttons.opcode.task[ idx ][ evt_type ] = task; 
//	m_dev_cb( DEV_CONF_BUTTONS_SENSE ); 
	
	return true; 
}

#endif // !Buttons 

#if 1 // GPRS 
bool dfw_gprs_apn_enable_set( bool enable )
{
	dev_settings.gprs.apn.enable = enable ? 1 : 0; 
	return true; 
}
bool dfw_gprs_apn_apn_update( const char* apnStr )
{
	if( apnStr == NULL ){
		dev_settings.gprs.apn.apns[0] = '\0';
		return true; 
	}
	else{
		uint32_t len = strlen( apnStr ); 
		if( len > GPRS_APN_APN_LEN_MAX)
			return false; 
		strcpy( dev_settings.gprs.apn.apns, apnStr );
	}	
	return true; 
}
bool dfw_gprs_apn_user_name_update( const char* usrStr )
{
	if( usrStr == NULL ){
		dev_settings.gprs.apn.username[0] = '\0';
		return true; 
	}
	else{
		uint32_t len = strlen( usrStr ); 
		if( len > GPRS_APN_USERNAME_LEN_MAX)
			return false; 
		strcpy( dev_settings.gprs.apn.username, usrStr );
	}
	return true; 	
}
bool dfw_gprs_apn_password_update( const char* pwdStr)
{
	if( pwdStr == NULL ){
		dev_settings.gprs.apn.password[0] = '\0';
		return true; 
	}
	else{
		uint32_t len = strlen( pwdStr ); 
		if( len > GPRS_APN_PASSWORD_LEN_MAX)
			return false; 
		strcpy( dev_settings.gprs.apn.password, pwdStr );
	}
	return true; 
}

bool dfw_gprs_apn_update( bool enable, const char* apn, const char* usrname, const char* password)
{
	if( ! dfw_gprs_apn_enable_set( enable )
	||	! dfw_gprs_apn_apn_update( apn )
	||  ! dfw_gprs_apn_user_name_update( usrname ) 
	||  ! dfw_gprs_apn_password_update( password )) {
		return false; 
	}
	return true; 
}

bool dfw_gprs_apn_write( const uint8_t * p_data, uint8_t len )
{
	if( len < 3 || p_data == NULL )
		return false; 
	
	uint32_t offset = 0; 
	uint8_t apnLen = p_data[0]; 
	if( apnLen > GPRS_APN_APN_LEN_MAX)
		return false; 
	
	if( apnLen > 0 )
		memcpy( dev_settings.gprs.apn.apns, p_data + 1, apnLen); 	
	else 
		dev_settings.gprs.apn.apns[0] = 0; 
	offset += apnLen + 1; 
	dev_settings.gprs.apn.apnLen = apnLen;
	
	uint8_t userLen = p_data[ offset + 0]; 
	if( userLen > GPRS_APN_USERNAME_LEN_MAX )
		return false; 
	if( userLen > 0 )
		memcpy( dev_settings.gprs.apn.username, p_data + 1 + offset, userLen ); 
	else 
		dev_settings.gprs.apn.username[0] = 0; 
	offset += userLen + 1; 
	dev_settings.gprs.apn.userNameLen = userLen; 

	uint8_t pwLen = p_data[ offset + 0]; 
	if( pwLen > GPRS_APN_PASSWORD_LEN_MAX )
		return false; 
	if( pwLen > 0 )
		memcpy( dev_settings.gprs.apn.password, p_data + 1 + offset , pwLen); 	
	else 
		dev_settings.gprs.apn.password[0] = 0; 
	
	dev_settings.gprs.apn.passwordLen = pwLen; 
	
	return true; 
}

bool dfw_gprs_server_update( const char* ip, uint16_t port)
{
	if( ip == NULL )
		return false; 
	
	uint32_t ipLen = strlen( ip ); 
	if(ipLen > GPRS_SERVER_IP_LEN_MAX || ipLen < GPRS_SERVER_IP_LEM_MIN)
		return false; 
	
	strcpy( dev_settings.gprs.server.ip, ip ); 
	dev_settings.gprs.server.port = port; 
	return true ;
}

bool dfw_gprs_server_write( const uint8_t * p_data, uint8_t len )
{
	if( len > GPRS_SERVER_IP_LEN_MAX || len < GPRS_SERVER_IP_LEM_MIN )
		return false; 
	
	if( p_data == NULL )
		return false; 
	
	uint16_t flag = uint16_decode( p_data + 0 ); 
    uint16_t port = uint16_decode( p_data + 2 ); 
    
	uint8_t ipLen = len - 4;// strlen( (char*)(p_data + 4) ); 
//	if( (ipLen + 4) != len )
//		return false; 
	
    dev_settings.gprs.server.flag = flag;
	dev_settings.gprs.server.port = port; 
	memcpy( dev_settings.gprs.server.ip, p_data + 4, ipLen ); 
	
	return true; 
}
bool dfw_gprs_heartbeat_update( uint32_t interval )
{
	if( interval < GPRS_HEARTBEAT_INTERVAL_MIN 
	||  interval > GPRS_HEARTBEAT_INTERVAL_MAX )
	{
		return false; 
	}
	
	dev_settings.gprs.interval.heartbeat = interval; 
	
	return true; 
}
bool dfw_gprs_upload_interval_update( uint32_t interval )
{
	if( interval < GPRS_UPLOAD_INTERVAL_MIN 
	||  interval > GPRS_UPLOAD_INTERVAL_MAX )
	{
		return false; 
	}
	
	dev_settings.gprs.interval.upload = interval; 
	return true; 
}
bool dfw_gprs_upload_lazy_interval_update( uint32_t interval )
{
	if( interval < GPRS_UPLOAD_LAZY_MIN 
	||  interval > GPRS_UPLOAD_LAZY_MAX )
	{
		return false; 
	}
	
	dev_settings.gprs.interval.lazy_upload = interval; 
	return true; 
}

#endif // !GPRS 

#if 1 // Phone 
bool dfw_phone_contact_delete_at( uint8_t no )
{
	if( no < PHONE_CONTACT_NO_MIN || no > PHONE_CONTACT_NO_MAX)
		return false; 
	
	dev_settings.phone.contacts[ no - 1].bit.enable = 0; 	
	return true; 
}
bool dfw_phone_contact_update(uint8_t no, bool sms, bool phone, const char* number)
{
	if( no < PHONE_CONTACT_NO_MIN || no > PHONE_CONTACT_NO_MAX)
		return false; 
	
	if( number == NULL )
		return false; 
	
	uint32_t numLen = strlen( number ); 
	if( numLen > PHONE_CONTACT_LEN_MAX || numLen < PHONE_CONTACT_LEN_MIN){
		return false; 
	}
	
	uint8_t index = no - 1; 
	dev_settings.phone.contacts[index].bit.value = 0; 
	dev_settings.phone.contacts[index].bit.enable = 1; 
	dev_settings.phone.contacts[index].bit.sms = sms ? 1 : 0; 
	dev_settings.phone.contacts[index].bit.phone = phone ? 1 : 0; 
	strcpy( dev_settings.phone.contacts[index].number, number ); 
	
	return true; 
}

bool dfw_phone_contact_write( const uint8_t * p_data, uint8_t len )
{
	if( p_data == NULL ) 
		return false; 
	
	if( len < PHONE_CONTACT_LEN_MIN || len > PHONE_CONTACT_LEN_MAX )
		return false; 
	
	phone_contact_t contact; 
	contact.bit.value = p_data[ 0 ]; 
	if( contact.bit.idx > PHONE_CONTACT_INDEX_MAX) 
		return false; 
	
	uint8_t index = contact.bit.idx; 	
	dev_settings.phone.contacts[ index ].bit.value = contact.bit.value; 
	memcpy( dev_settings.phone.contacts[ index ].number, p_data + 1, len - 1 ); 
//	if( contact.bit.enable == 0 ){
//		dev_settings.phone.contacts[ index ].bit.enable = 0; 
//		//memset( dev_settings.phone.contacts[ index ].number, 0xff, PHONE_CONTACT_NUMBER_SIZE ); 
//	}
//	else{
//		dev_settings.phone.contacts[ index ].bit.value = contact.bit.value; 
//		memcpy( dev_settings.phone.contacts[ index ].number, p_data + 1, len - 1 ); 
//	}
	
	return true; 
}

bool dfw_phone_sms_prefix_update( bool enable, const char* prefix_text)
{
	if( !enable ){
		dev_settings.phone.sms_prefix.size = 0; 
		return true; 
	}
	
	if( prefix_text == NULL )
		return false; 
	
	uint32_t textLen = strlen( prefix_text );
	if( textLen > PHONE_SMS_PREFIX_LEN_MAX )
		return false; 
	
	dev_settings.phone.sms_prefix.size = (uint8_t)textLen; 
	strcpy( dev_settings.phone.sms_prefix.text, prefix_text );
	return true; 
}

bool dfw_phone_sms_prefix_write( const uint8_t * p_data, uint8_t len )
{
	if( p_data == NULL )
		return false; 
	
	if( len > PHONE_SMS_PREFIX_LEN_MAX )
		return false; 
	
	if( len > 0 ){
		dev_settings.phone.sms_prefix.size = len; 
		memcpy( dev_settings.phone.sms_prefix.text, p_data, len ); 
	}
	else{
		dev_settings.phone.sms_prefix.size = 0; 
		memset( dev_settings.phone.sms_prefix.text, 0xff, PHONE_SMS_PREFIX_LEN_MAX ); 
	}	
	
	return true; 
}

bool dfw_phone_sos_holding_time_update( uint16_t holding_time)
{
	if( holding_time < PHONE_SOS_HOLDING_TIME_MIN || holding_time > PHONE_SOS_HOLDING_TIME_MAX )
		return false; 
	
	dev_settings.phone.SOS.holding_time = holding_time; 
	return true; 
}
bool dfw_phone_sos_ring_time_update( uint8_t ring_time)
{
	if( ring_time < PHONE_SOS_RING_TIME_MIN || ring_time > PHONE_SOS_RING_TIME_MAX )
		return false; 
	dev_settings.phone.SOS.rings_time = ring_time; 
	return true; 
}
bool dfw_phone_sos_loops_update( uint8_t loops )
{
	if( loops > PHONE_SOS_LOOP_TIMES_MAX )
		return false; 
	dev_settings.phone.SOS.loops = loops; 
	return true; 
}

bool dfw_phone_sos_write( const uint8_t * p_data, uint8_t len )
{
	if( p_data == NULL ) 
		return false; 
	
	if( len != PHONE_SOS_DATA_SIZE ) 
		return false; 
	
	uint16_t holding_time = uint16_decode( p_data ); 
	uint8_t ring_time = p_data[ 2 ]; 
	uint8_t loops = p_data[ 3 ]; 
	
	if( holding_time < PHONE_SOS_HOLDING_TIME_MIN || holding_time > PHONE_SOS_HOLDING_TIME_MAX )
		return false; 
	if( ring_time < PHONE_SOS_RING_TIME_MIN || ring_time > PHONE_SOS_RING_TIME_MAX )
		return false; 
	if( loops > PHONE_SOS_LOOP_TIMES_MAX )
		return false; 
	
	dev_settings.phone.SOS.holding_time = holding_time; 
	dev_settings.phone.SOS.rings_time = ring_time; 
	dev_settings.phone.SOS.loops = loops; 
	return true; 	
}

bool dfw_phone_switch_auto_answer_update( uint8_t enable, uint8_t rings)
{
	if( rings > PHONE_AUTO_ANSWER_RINGS_MAX) 
		return false; 
	
	dev_settings.phone.switches.auto_answer_enable = enable  ; 
	dev_settings.phone.switches.auto_answer_rings_delay = rings; 
	return true; 
}
bool dfw_phone_switch_intercept_update( uint8_t enable )
{
	dev_settings.phone.switches.intercept_except_auth = enable ; 
	return true; 
}
bool dfw_phone_switch_hangup_update( uint8_t enable )
{
	dev_settings.phone.switches.hangup_manual_not_allow = enable ; 
	return true; 
}
bool dfw_phone_switch_monitor_update( uint8_t enable )
{
	dev_settings.phone.switches.monitor_environment_enable = enable ; 
	return true; 
}
bool dfw_phone_switch_set_update( uint8_t enable )
{
	dev_settings.phone.switches.sms_set_only_by_auth_number = enable ; 
	return true; 
}

bool dfw_phone_switches_update( const uint8_t * p_data, uint8_t len )
{
	if( len != PHONE_SWITCHES_DATA_SIZE )
		return false; 
	
	uint32_t value = uint32_decode( p_data ); 
	phone_swithes_t * pswitch = (phone_swithes_t*)&value; 
	if( pswitch->auto_answer_rings_delay > PHONE_AUTO_ANSWER_RINGS_MAX )
		return false; 
	
	dev_settings.phone.switches.value = value; 	
	return true; 
}

#endif // !Phone 

#if 1 // Alert 
bool dfw_alert_power_low_update( uint8_t enable, uint8_t threshold )
{
	if( threshold > 30 )
		return false; 
	
	dev_settings.alert.power.low = enable; 
	if( threshold > 0 )
		dev_settings.alert.power.threshold = threshold; 
	return true; 
}
bool dfw_alert_power_on_update( uint8_t enable )
{
	dev_settings.alert.power.on = enable; 
	return true; 
}
bool dfw_alert_power_off_update( uint8_t enable )
{
	dev_settings.alert.power.off = enable; 
	return true; 
}
bool dfw_alert_power_update( const uint8_t * p_data, uint8_t len)
{
	if( len != ALERT_POWER_DATA_SIZE )
		return false; 
	
	alert_power_t power;
	power.value =  p_data[0]; 
	if( power.threshold > ALERT_POWER_LOW_LEVEL_MAX )
		return false; 
	
	dev_settings.alert.power = power; 
	return true; 	
}
bool dfw_alert_geo_delete_at( uint8_t no )
{
	if( no < ALERT_GEO_START_INDEX || no > ALERT_GEO_END_INDEX)
		return false; 
	
	dev_settings.alert.geos[ no - 1 ].enable = 0; 
	return true; 
}
bool dfw_alert_geo_update( uint8_t no, uint8_t enable, uint8_t dir, uint16_t radius, int32_t longitude, int32_t latitude )
{	
	if( no >= ALERT_GEO_NUMBERS )
		return false; 
	
	if( enable == 0 ){
		dev_settings.alert.geos[ no ].value = 0; 
		memset( dev_settings.alert.geos[no].data, 0xff, (ALERT_GEO_FENCE_POINTS << 1 )); 
		return true; 
	}
	
	if( radius > ALERT_GEO_RADIUS_MAX || radius < ALERT_GEO_RADIUS_MIN )
		return false; 
	
	alert_geo_t GEO; 
	GEO.idx = no + 1; 
	GEO.enable = 1; 
	GEO.dir = dir; 
	GEO.radius = radius; 
	GEO.type = 0; 	// use radius 
	GEO.count = 1; 
	GEO.data[ 0 ] = longitude;
	GEO.data[ 1 ] = latitude; 
	
	dev_settings.alert.geos[ no ].value = GEO.value; 
	memset( dev_settings.alert.geos[no].data, 0xff, (ALERT_GEO_FENCE_POINTS << 1 )); 
	return true; 
}
bool dfw_alert_geo_write( const uint8_t * p_data, uint8_t len )
{
	if( len < ALERT_GEO_DATA_SIZE_MIN || len > ALERT_GEO_DATA_SIZE_MAX )
		return false; 
	
	if( p_data == NULL ) 
		return false; 
	
	alert_geo_t Geo; 
	memset( Geo.data, 0xff, (ALERT_GEO_FENCE_POINTS << 1 ) );
	
	Geo.value = uint32_decode( p_data ); 
	
	if( Geo.idx > ALERT_GEO_NUMBERS )
		return false; 
	
	if( Geo.enable == 0 ){
		//Geo.value = 0; 
		memset( Geo.data, 0xff, (ALERT_GEO_FENCE_POINTS << 1 ) ); 
		dev_settings.alert.geos[ Geo.idx ] = Geo;
		return true; 
	}
		
	if( Geo.count == 0 )
		return false; 
	if( (Geo.count * 8) != (len - ALERT_GEO_DATA_SIZE_MIN ) ) 
		return false; 
	
	if( Geo.type == 0 ){ //Radius Mode 
		if( Geo.radius <  ALERT_GEO_RADIUS_MIN || Geo.radius > ALERT_GEO_RADIUS_MAX )
			return false; 			
	}
	else{ // Fence Mode 
		if( Geo.count < 3 )
			return false; 
	}
	
	// parse lat&long point 
	uint32_t index = 0; 
	uint32_t offset = 4; 
	for( uint32_t i = 0 ; i < Geo.count; i++){
		Geo.data[ index ++ ] = uint32_decode( p_data + offset ); 
		Geo.data[ index ++ ] = uint32_decode( p_data + offset + 4 ); 
		offset += 8; 
	}
	
	dev_settings.alert.geos[ Geo.idx ] = Geo; 
	//memcpy( (uint8_t*)&dev_settings.alert.geos[Geo.idx],  (uint8_t*)&Geo, sizeof( alert_geo_t )) ;
	
	return true; 
}

bool dfw_alert_motion_update( bool enable, uint16_t setup_time, uint16_t action_time)
{
	if( enable ){
		if( setup_time > 0 ){
			if( setup_time < ALERT_MOTION_SETUP_TIME_MIN 
			||	setup_time > ALERT_MOTION_SETUP_TIME_MAX )
			{
				return false; 
			}
		}
		if( action_time > 0 ){
			if( action_time < ALERT_MOTION_ACTION_TIME_MIN 
			|| action_time > ALERT_MOTION_ACTION_TIME_MAX )
			{
				return false; 
			}
		}
				
		dev_settings.alert.motion.enable = 1;
		if( setup_time > 0 )
			dev_settings.alert.motion.setup_time = setup_time;
		if( action_time > 0 )
			dev_settings.alert.motion.action_time = action_time; 
	}
	else{
		dev_settings.alert.motion.enable = 0;		 
	}
    
    
	return true;
}

bool dfw_alert_motion_write( const uint8_t * p_data, uint8_t len )
{
	if( len != ALERT_MOTION_DATA_SIZE ) 
		return false; 
	
	alert_motion_t motion; 
	motion.value = uint32_decode( p_data ); 
	
	if( motion.enable > 0 ){
		if( motion.setup_time < ALERT_MOTION_SETUP_TIME_MIN 
		||	motion.setup_time > ALERT_MOTION_SETUP_TIME_MAX )
		{
			return false; 
		}
		
		if( motion.action_time < ALERT_MOTION_ACTION_TIME_MIN 
		|| motion.action_time > ALERT_MOTION_ACTION_TIME_MAX )
		{
			return false; 
		}
	}
	
	dev_settings.alert.motion = motion; 	
    m_dev_cb( DEV_UPDATE_ALERT_MOTION ); 
	return true; 
}
bool dfw_alert_actionless_update( bool enable, uint32_t threshold)
{
	if( enable ){
		if( threshold > ALERT_STATIC_THRESHOLD_MAX 
		|| threshold < ALERT_STATIC_THRESHOLD_MIN)
			return false; 
		
		dev_settings.alert.actionless.enable = 1;
		dev_settings.alert.actionless.threshold = threshold; 
		
	}
	else{
		dev_settings.alert.actionless.enable = 0;
	}
	return true; 
}
bool dfw_alert_actionless_write( const uint8_t * p_data, uint8_t len)
{
	if( len != ALERT_STATIC_DATA_SIZE )
		return false; 
	
	alert_static_t actionless; 
	actionless.value = uint32_decode( p_data ); 
	if( actionless.enable > 0 ){
		if( actionless.threshold > ALERT_STATIC_THRESHOLD_MAX 
		|| actionless.threshold < ALERT_STATIC_THRESHOLD_MIN)
			return false; 
	}
	
	dev_settings.alert.actionless = actionless; 
	m_dev_cb( DEV_UPDATE_ALERT_STATIC ); 
	return true; 
}
bool dfw_alert_speed_update( bool enable, uint32_t threshold)
{
	if( enable ){
		if( threshold > ALERT_SPEED_THRESHOLD_MAX 
		||  threshold < ALERT_SPEED_THRESHOLD_MIN )
			return false; 
		
		dev_settings.alert.speed.enable = 1; 
		dev_settings.alert.speed.threshold = threshold; 
	}
	else{
		dev_settings.alert.speed.enable = 0; 
	}
	return true; 
}
bool dfw_alert_speed_write( const uint8_t * p_data, uint8_t len)
{
	if( len != ALERT_SPEED_DATA_SIZE )
		return false; 
	
	alert_speed_t speed; 
	speed.value = uint32_decode( p_data ); 
	if( speed.enable > 0 ){
		if( speed.threshold > ALERT_SPEED_THRESHOLD_MAX 
		||  speed.threshold < ALERT_SPEED_THRESHOLD_MIN )
			return false; 
	}
	
	dev_settings.alert.speed = speed; 	
	return true; 
}
bool dfw_alert_tilt_update( bool enable, uint8_t angle, uint16_t time)
{
	if( enable ){
		if( angle > 0 ){
			if( angle > ALERT_TILT_ANGLE_MAX || angle < ALERT_TILT_ANGLE_MIN )
				return false; 
		}
		if( time > 0 ){
			if( time > ALERT_TILT_TIME_MAX || time < ALERT_TILT_TIME_MIN )
				return false; 
		}
		
		dev_settings.alert.tilt.enable = 1; 
		if( angle > 0 )
			dev_settings.alert.tilt.angle = angle; 
		if( time > 0 ) 
			dev_settings.alert.tilt.time = time; 
	}
	else{
		dev_settings.alert.tilt.enable = 0; 
	}
	return true; 
}
bool dfw_alert_tilt_write( const uint8_t * p_data, uint8_t len )
{
	if( len != ALERT_TILT_DATA_SIZE )
		return false; 
	
	alert_tilt_t tilt; 
	tilt.value = uint32_decode( p_data ); 
	if( tilt.enable > 0 ){
		if( tilt.angle > ALERT_TILT_ANGLE_MAX || tilt.angle < ALERT_TILT_ANGLE_MIN )
			return false; 
		if( tilt.time > ALERT_TILT_TIME_MAX || tilt.time < ALERT_TILT_TIME_MIN )
			return false; 
	}
	
	dev_settings.alert.tilt = tilt; 	
    m_dev_cb( DEV_UPDATE_ALERT_TILT ); 
	return true; 
}
bool dfw_alert_falldown_update( uint8_t mode, uint8_t level )
{
	if( mode >= ALERT_FALLDOWN_MODE_NUMS) 
		return false; 
	if( level > 0 ){
		if( level > ALERT_FALLDOWN_LEVEL_MAX || level < ALERT_FALLDOWN_LEVEL_MIN )
			return false; 
	}
	
	//dev_settings.alert.falldown.mode = mode; 
    dev_settings.alert.falldown.enable = mode; 
	if(level > 0 )
		dev_settings.alert.falldown.level = level; 
	return true; 
}
bool dfw_alert_falldown_write( const uint8_t * p_data, uint8_t len )
{
//	if( len != ALERT_FALLDOWN_DATA_SIZE )
//		return false; 
//	
//	alert_falldown_t falldown; 
//	falldown.value = uint32_decode( p_data ); 
//	if( falldown.mode > 0 ){
//		if( falldown.mode >= ALERT_FALLDOWN_MODE_NUMS) 
//			return false; 
//		if( falldown.level > ALERT_FALLDOWN_LEVEL_MAX || falldown.level < ALERT_FALLDOWN_LEVEL_MIN )
//			return false; 
//	}
//    
    if( len != ALERT_FALLDOWN_DATA_SIZE )
		return false; 
	
	alert_falldown_t falldown; 
	falldown.value = p_data[0]; 
	if( falldown.enable > 0 ){
		if( falldown.level > ALERT_FALLDOWN_LEVEL_MAX || falldown.level < ALERT_FALLDOWN_LEVEL_MIN )
			return false; 
	}
	
	dev_settings.alert.falldown = falldown; 	
    m_dev_cb( DEV_UPDATE_ALERT_FALLDOWN ); 
	return true; 
}
#endif // !Alert 


bool dfw_settings_update( const uint8_t * p_data, uint16_t size )
{
	uint16_t index = 0; 
	uint8_t field_type, field_len; 
	bool success = true; 
//	uint32_t value; 
	
	while( index < size )
	{
		field_len = p_data[ index + 0 ]; 
		field_type = p_data[ index + 1 ] ; 
		
		if( (field_len + index + 1 ) > size)
			break; 
		
		switch( field_type )
		{
			default:
				success = false;  // not allow to write 
				break; 
						
#if 1	// System Update 
			case DEV_CONF_SYSTEM_MODE:{
				if( field_len == 5 ){
					sys_mode_t sys_mode; 
					sys_mode.value = uint32_decode( p_data + index + 2 ); 
					if( !dfw_system_mode_update( sys_mode.mode, sys_mode.time ) ) {
						success = false; 
					}
				}
				else{
					success = false; 
				}
				break; }
			case DEV_CONF_SYSTEM_WORK_TIME:{
				if( field_len == 5 ){
					sys_work_time_t work_time;
					work_time.value = uint32_decode( p_data + index + 2 ); 
					if( !dfw_system_work_time_update( 	work_time.start_hour,  work_time.start_minute, 
														work_time.end_hour, work_time.end_minute )
					){
						success = false; 
					}
				}
				else{
					success = false; 
				}
				break;}
			case DEV_CONF_SYSTEM_NO_DISTURB:{
				if( field_len == 5 ){
					system_no_disturb_t no_disturb;
					no_disturb.value = uint32_decode( p_data + index + 2 ); 
					if( ! dfw_system_do_not_disturb_update ( 	 no_disturb.start_hour,  no_disturb.start_minute, 
														no_disturb.end_hour, no_disturb.end_minute )
					){
						success = false; 
					}
				}
				else{
					success = false; 
				}
				break;}
			case DEV_CONF_SYSTEM_PASSWORD:{
				if( field_len == 5 ){
					sys_password_t password; 
					password.value = uint32_decode( p_data + index + 2 ); 
					if( !dfw_system_password_update( password.enable , password.number ) ){
						success = false; 
					}
				}
				else{
					success = false; 
				}
				break; }
			case DEV_CONF_SYSTEM_TIMEZONE:{
				if( field_len == 2 ){
					uint8_t offset = p_data[ index + 2 ] ; 
					if( !dfw_system_timezone_update( (int8_t)offset ) ) {
						success = false; 
					}
				}
				else{
					success = false; 
				}
				break; }
			case DEV_CONF_SYSTEM_SWITCHES:{
				if( field_len == 5 ){
					uint32_t value = uint32_decode( p_data + index + 2 ); 
					if( !dfw_system_switches_update( value )) {
						success = false; 
					}
				}
				else{
					success = false; 
				}
				break; }
			case DEV_CONF_SYSTEM_VOLUME:{
				if( field_len == 4 ){
					sys_volume_t volume;
					volume.value = uint32_decode( p_data + index + 2 ); 
					if( !dfw_system_volume_mic_update( volume.mic )
					||  !dfw_system_volume_ringtone_update( volume.ringtone ) 
					||  !dfw_system_volume_speaker_update( volume.speaker )) {
						success = false; 
					}
				}
				else{
					success = false; 
				}
				break; }
            case DEV_CONF_SYSTEM_ALARMCLOCK:{
                if( field_len == 5 ){
//                    alarm_clock_t alarm; 
//                    alarm.flag = p_data[ index + 2]; 
//                    alarm.hour = p_data[ index + 3]; 
//                    alarm.minute = p_data[ index + 4]; 
//                    alarm.weekday = p_data[ index + 5]; 
//                    alarm.ring_time = p_data[ index + 6 ];
//                    if( !dfw_system_alarmclock_update( &alarm ) ){
//                        success = false; 
//                    }
                }
                else{
                    success = false; 
                }
                break; }
#endif 
			
#if 1 	// Buttons 
			case DEV_CONF_BUTTONS_SENSE:{
				success = dfw_buttons_sense_update( p_data + index + 2, field_len - 1 ); 
				break; }
			case DEV_CONF_BUTTONS_OP:
				if( field_len == 29 ){
					//uint16_t op; 
					uint32_t offset = index + 2; 
					for( uint32_t i = 1 ; i < 7; i ++){
						for( uint32_t j = 0; j < 4; j ++ ){
//							op = ( i << 12 ) | ( j << 8) | p_data[ offset ++ ]; 
							success = dfw_buttons_op_update( i, j, p_data[ offset ++ ] ); 							
						}
						if( !success ){
							break; 
						}
					}
				}
				else{
					success = false; 
				}
				break; 
#endif 	//!Buttons 
			
#if 1 	// GPRS
			case DEV_CONF_GPRS_APN:
				success = dfw_gprs_apn_write( p_data + index + 2, field_len - 1 ); 
				break; 
			case DEV_CONF_GPRS_SERVER_PARAM:
				success = dfw_gprs_server_write( p_data + index + 2, field_len - 1 ); 
				break; 
			case DEV_CONF_GPRS_TIME:
				if( field_len == 13 ){
					uint32_t heartbeat = uint32_decode( p_data + index + 2 ); 
					uint32_t upload = uint32_decode( p_data + index + 6 ); 
					uint32_t lazy_upload = uint32_decode( p_data + index + 10 ); 
					
					success = 	dfw_gprs_heartbeat_update(  heartbeat  ) 
							 && dfw_gprs_upload_interval_update( upload )
							 && dfw_gprs_upload_lazy_interval_update( lazy_upload ) ; 
				}
				else{
					success = false; 
				}
				break; 
			
#endif 	//!GPRS
			
#if 1 	// Phone 
			case DEV_CONF_PHONE_CONTACTS:
				success = dfw_phone_contact_write( p_data + index + 2, field_len - 1 ); 
				break; 
			case DEV_CONF_PHONE_SMS_PREFIX:
				success = dfw_phone_sms_prefix_write( p_data + index + 2, field_len - 1 );
				break; 
			case DEV_CONF_PHONE_SOS_OPTION:
				success = dfw_phone_sos_write( p_data + index + 2 ,field_len - 1 ); 
				break; 
			case DEV_CONF_PHONE_SWITCHES:
				success = dfw_phone_switches_update( p_data + index + 2, field_len - 1 ); 
				break; 
#endif 	//!Phone 
			
#if 1 	// Alert 
			case DEV_CONF_ALERT_POWER:
				success = dfw_alert_power_update( p_data + index +  2, field_len - 1 ); 
				break; 
			case DEV_CONF_ALERT_GEO:
				success = dfw_alert_geo_write( p_data + index + 2, field_len - 1 ); 
				break; 
			case DEV_CONF_ALERT_MOTION:
				success = dfw_alert_motion_write( p_data + index + 2, field_len - 1 ); 
				break; 
			case DEV_CONF_ALERT_STATIC:
				success = dfw_alert_actionless_write( p_data + index + 2, field_len - 1 ); 
				break; 
			case DEV_CONF_ALERT_SPEED:
				success = dfw_alert_speed_write( p_data + index + 2, field_len - 1 ); 
				break; 
			case DEV_CONF_ALERT_TILT:
				success = dfw_alert_tilt_write( p_data + index + 2, field_len - 1 ); 
				break; 
			case DEV_CONF_ALERT_FALLDOWN:
				success = dfw_alert_falldown_write( p_data + index + 2, field_len - 1); 
				break; 
#endif 	//!Alert 
			
			case DEV_CONF_SAVE_PROFILE:
//				conf_file_update_page( (uint32_t*)& dev_settings, sizeof( dev_config_t) ); 
				break; 
		}
		
		
		if( success == false )
			break; 
		
		index += field_len + 1; 
	}
	
	return success ; 
}

