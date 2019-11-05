// Header:		Device Configuration 
// File Name: 	dev_conf.c 
// Author:		Carlton	
// Date:		2017.05.25 


#include "nrf.h"
#include <string.h>

#include "app_util.h"
#include "dev_conf.h"

//#include "conf_file.h"
#include "dev_config.factory.h"
//#include "app_rtc_drv.h"


dev_property_t dev_property; 
dev_config_t dev_settings; 
dev_settings_update_handler_t m_dev_cb; 


static void dev_conf_update_from_memory(uint32_t * address)
{
	dev_config_t * p_dev_config = (dev_config_t*)address; 
	
	dev_settings = * p_dev_config; 
}

static void dev_conf_update_from_factory_default(void)
{
	uint32_t i; 
	memset( &dev_settings, 0, sizeof( dev_settings )); 
	
#if 0	// system 
	// mode 
	dev_settings.system.mode.mode = SYSTEM_MODE_MODE; 
	dev_settings.system.mode.time = SYSTEM_MODE_TIME; 
	
	dev_settings.system.zone.offset = SYSTEM_TIME_ZONE_OFFSET; 
	
	dev_settings.system.password.enable = SYSTEM_PASSWORD_ENABLE; 
	dev_settings.system.password.number = SYSTEM_PASSWORD_VALUE;
	
	dev_settings.system.switches.leds = SYSTEM_LEDS_ENABLE; 
	dev_settings.system.switches.motor = SYSTEM_MOTOR_ENABLE; 
	dev_settings.system.switches.buzzer = SYSTEM_BUZZER_ENABLE; 
	dev_settings.system.switches.AGPS = SYSTEM_AGPS_ENABLE; 
	
	dev_settings.system.volume.ringtone = SYSTEM_VOLUME_RINGTONE_LEVEL; 
	dev_settings.system.volume.mic = SYSTEM_VOLUME_MIC_LEVEL; 
	dev_settings.system.volume.speaker = SYSTEM_VOLUME_SPEAKER_LEVEL; 
	
//	dev_settings.system.dev_name.size = strlen( SYSTEM_DEV_NAME_STRING ); 
//	if( dev_settings.system.dev_name.size > 0 )
//		strcpy( dev_settings.system.dev_name.val, SYSTEM_DEV_NAME_STRING ); 		
//	dev_settings.system.alarms[0].flag = 0 << 1; 
//    dev_settings.system.alarms[1].flag = 1 << 1; 
//    dev_settings.system.alarms[2].flag = 2 << 1; 
//    dev_settings.system.alarms[3].flag = 3 << 1; 
#endif 

#if 0 // Buttons 
	
	dev_settings.buttons.sense.click = BUTTONS_CLICK_THRESHOLD; 
	dev_settings.buttons.sense.double_click = BUTTONS_DOUBLE_CLICK_THRESHOLD; 
	dev_settings.buttons.sense.short_hold = BUTTONS_SHORT_HOLD_THRESHOLD; 
	dev_settings.buttons.sense.long_hold = BUTTONS_LONG_HOLD_THRESHOLD; 
	
	// button op initial here 	
	// SOS-Button Event 
	dev_settings.buttons.opcode.task[ 1 ][ BUTTONS_CLICK_IDX ] = SOS_CLICK_EVENT; 
	dev_settings.buttons.opcode.task[ 1 ][ BUTTONS_DOUBLE_CLICK_IDX ] = SOS_DOUBLE_CLICK_EVENT;
	dev_settings.buttons.opcode.task[ 1 ][ BUTTONS_SHORT_HOLD_IDX ] = SOS_SHORT_HOLD_EVENT; 
	dev_settings.buttons.opcode.task[ 1 ][ BUTTONS_LONG_HOLD_IDX ] = SOS_LONG_HOLD_EVENT; 
	
	// Side-Button Event 
	dev_settings.buttons.opcode.task[ 2 ][ BUTTONS_CLICK_IDX ] = SIDE_L_CLICK_EVENT; 
	dev_settings.buttons.opcode.task[ 2 ][ BUTTONS_DOUBLE_CLICK_IDX ] = SIDE_L_DOUBLE_CLICK_EVENT;
	dev_settings.buttons.opcode.task[ 2 ][ BUTTONS_SHORT_HOLD_IDX ] = SIDE_L_SHORT_HOLD_EVENT; 
	dev_settings.buttons.opcode.task[ 2 ][ BUTTONS_LONG_HOLD_IDX ] = SIDE_L_LONG_HOLD_EVENT; 
	
	dev_settings.buttons.opcode.task[ 3 ][ BUTTONS_SHORT_HOLD_IDX ] = BUTTONS_OP_POWER_OFF; 
#endif 
	
#if 0 // GPRS 
	
	// APN 
	dev_settings.gprs.apn.enable = GPRS_APN_ENABLE; 
	if( strlen( GPRS_APN_NAME_STRING ) > 0) 
		strcpy( dev_settings.gprs.apn.apns, GPRS_APN_NAME_STRING ); 
	if( strlen( GPRS_APN_USER_NAME_STRING ) > 0 )
		strcpy( dev_settings.gprs.apn.username, GPRS_APN_USER_NAME_STRING ); 
	if( strlen( GPRS_APN_PASSWORD_STRING ) > 0 ) 
		strcpy( dev_settings.gprs.apn.password, GPRS_APN_PASSWORD_STRING ); 
	
	// Server 
	if( strlen( GPRS_SERVER_IP_STRING ) > 0 ) 
		strcpy( dev_settings.gprs.server.ip, GPRS_SERVER_IP_STRING ); 
	dev_settings.gprs.server.port = GPRS_SERVER_PORT; 

	///  Time 
	dev_settings.gprs.interval.heartbeat = GPRS_HEARTBEAT_INTERVAL; 
	dev_settings.gprs.interval.upload = GPRS_UPLOAD_INTERVAL; 
	dev_settings.gprs.interval.lazy_upload = GPRS_LAZY_UPLOAD_INTERVAL; 
	
#endif

#if 0 // Phone 
	for( i = 2; i < PHONE_CONTACTS_NUM; i ++ ){
		dev_settings.phone.contacts[i].bit.idx = i; 
	}
	
	// contact  1st 
	dev_settings.phone.contacts[0].bit.enable = AUTH_CONTACT_1ST_ENABLE; 
	dev_settings.phone.contacts[0].bit.idx = 0; 
	dev_settings.phone.contacts[0].bit.sms = AUTH_CONTACT_1ST_ACCEPT_SMS; 
	dev_settings.phone.contacts[0].bit.phone = AUTH_CONTACT_1ST_ACCEPT_PHONE; 
	dev_settings.phone.contacts[0].bit.no_card = AUTH_CONTACT_1ST_NO_CARD; 
	if( strlen( AUTH_CONTACT_1ST_PHONE_NUMBER ) > 0 )
		strcpy( dev_settings.phone.contacts[0].number, AUTH_CONTACT_1ST_PHONE_NUMBER ); 
	
	dev_settings.phone.contacts[1].bit.enable = AUTH_CONTACT_2ND_ENABLE; 
	dev_settings.phone.contacts[1].bit.idx = 1; 
	dev_settings.phone.contacts[1].bit.sms = AUTH_CONTACT_2ND_ACCEPT_SMS; 
	dev_settings.phone.contacts[1].bit.phone = AUTH_CONTACT_2ND_ACCEPT_PHONE; 
	dev_settings.phone.contacts[1].bit.no_card = AUTH_CONTACT_2ND_NO_CARD; 
	if( strlen( AUTH_CONTACT_2ND_PHONE_NUMBER ) > 0 )
		strcpy( dev_settings.phone.contacts[1].number, AUTH_CONTACT_2ND_PHONE_NUMBER ); 	
		
	// prefix text 
	dev_settings.phone.sms_prefix.size = strlen( SMS_PREFIX_TEXT_STRING ); 
	if( dev_settings.phone.sms_prefix.size > 0 ) 
		strcpy( dev_settings.phone.sms_prefix.text, SMS_PREFIX_TEXT_STRING ); 
	
	// SOS 
	dev_settings.phone.SOS.holding_time = SOS_HOLDING_TIME_ALLOW; 
	dev_settings.phone.SOS.rings_time = SOS_RING_TIME_ALLOW; 
	dev_settings.phone.SOS.loops = SOS_CALL_LOOP_ALLOW; 
	
	// Switches 
	dev_settings.phone.switches.auto_answer_enable = AUTO_ANWER_INCOMING_CALL_ENABLE; 
	dev_settings.phone.switches.auto_answer_rings_delay = AUTO_ANSWER_RING_COUNTS; 
	dev_settings.phone.switches.intercept_except_auth = INTERCEPT_PHONE_CALL_ENABLE; 
	dev_settings.phone.switches.hangup_manual_not_allow = HANGUP_PHONE_CALL_ENABLE; 
	dev_settings.phone.switches.monitor_environment_enable = MONITOR_ENVIRONMENT_ENABLE; 
	dev_settings.phone.switches.sms_set_only_by_auth_number = SMS_COMMAND_ONLY_AUTH_CONTACT; 
	
#endif 	
	
#if 1 // Alert 
	
	// power alert 
	dev_settings.alert.power.low = POWER_LOW_ALERT_ENABLE; 
	dev_settings.alert.power.threshold = POWER_LOW_ALERT_THRESHOLD; 
	dev_settings.alert.power.off = POWER_OFF_ALERT_ENABLE; 
	dev_settings.alert.power.on = POWER_ON_ALERT_ENABLE; 
	
	// GEO Fence at No.0 
	for(i = 0; i < ALERT_GEO_DATA_SIZE_MIN; i++){
		dev_settings.alert.geos[i].idx = i; 
	}
	
	dev_settings.alert.geos[0].enable = GEO_FENCE_NO_0_ALERT_ENABLE; 
	dev_settings.alert.geos[0].dir = GEO_FENCE_NO_0_ALERT_DIR; 
	dev_settings.alert.geos[0].type = GEO_FENCE_NO_0_ALERT_TYPE; 
	dev_settings.alert.geos[0].radius = GEO_FENCE_NO_0_RADIUS; 
	dev_settings.alert.geos[0].count = GEO_FENCE_NO_0_ALERT_POINT_COUNT; 
	dev_settings.alert.geos[0].longitude = GEO_FENCE_NO_0_LONGITUDE_0; 
	dev_settings.alert.geos[0].latitude = GEO_FENCE_NO_0_LATITUDE_0;
	
	// Motion 
	dev_settings.alert.motion.enable = MOTION_ALERT_ENABLE; 
	dev_settings.alert.motion.setup_time = MOTION_ALERT_SETUP_TIME; 
	dev_settings.alert.motion.action_time = MOTION_ALERT_ACTION_TIME; 
	dev_settings.alert.motion.dial = 0;
	// Actionless 
	dev_settings.alert.actionless.enable = ACTIONLESS_ALERT_ENABLE; 
	dev_settings.alert.actionless.threshold = ACTIONLESS_ALERT_THRESHOLD; 
    dev_settings.alert.actionless.dial = 0;
	
	// Speed 
	dev_settings.alert.speed.enable = SPEED_ALERT_ENABLE; 
	dev_settings.alert.speed.threshold = SPEED_ALERT_THRESHOLD; 
	
	// Tilt 
	dev_settings.alert.tilt.enable = TILT_ALERT_ENABLE; 
	dev_settings.alert.tilt.angle = TILT_ALERT_ANGLE_THRESHOLD; 
	dev_settings.alert.tilt.time = TILT_ALERT_TIME_THRESHOLD; 
	
	// Fall down 
	//dev_settings.alert.falldown.mode = FALLDOWN_ALERT_MODE; 
    dev_settings.alert.falldown.enable = FALLDOWN_ALERT_MODE;
	dev_settings.alert.falldown.level = FALLDOWN_ALERT_LEVEL; 
	
#endif 
	
}


void dev_conf_reset_to_factory_profile(void)
{
//	uint32_t *pdest = conf_file_read( CONF_FILE_FACTORY ); 
//	if( pdest != NULL ){
//		dev_conf_update_from_memory( pdest ); 
//		conf_file_update_page( (uint32_t*)& dev_settings, sizeof( dev_config_t) ); 
//	}
}

void dev_conf_reset_to_factory(void)
{
//	uint32_t *pdest = conf_file_read( CONF_FILE_FACTORY ); 
//	if( pdest != NULL){
//		dev_conf_update_from_memory( pdest ); 
//		return ; 
//	}
//	
	dev_conf_update_from_factory_default();
}

void dev_conf_save_profile(void)
{
    //conf_file_update_page( (uint32_t*)& dev_settings, sizeof( dev_config_t) );
}

bool dev_conf_volume_speaker_inc(void)
{
    if( dev_settings.system.volume.speaker == 100 )
        return false; 
    
    if( dev_settings.system.volume.speaker > 90 )
        dev_settings.system.volume.speaker = 100;
    else 
        dev_settings.system.volume.speaker += 10; 
    
    return true; 
}
bool dev_conf_volume_speaker_dec(void)
{
    if( dev_settings.system.volume.speaker == 0 )
        return false; 
    
    if( dev_settings.system.volume.speaker < 10 ){
        dev_settings.system.volume.speaker = 0; 
    }
    else 
        dev_settings.system.volume.speaker -= 10; 
    
    return true; 
}



void dev_conf_init(dev_conf_init_t * p_conf_init)
{
	if( p_conf_init == NULL) //chen
		return; 
	
	m_dev_cb = p_conf_init->cb; 
	
//	conf_file_init();
//    alarms_init_t alarms_obj; 
//    alarms_obj.cb        = p_conf_init->clock_active_handle; 
//    alarms_obj.clocks[0] = &dev_settings.system.alarms[0]; 
//    alarms_obj.clocks[1] = &dev_settings.system.alarms[1]; 
//    alarms_obj.clocks[2] = &dev_settings.system.alarms[2]; 
//    alarms_obj.clocks[3] = &dev_settings.system.alarms[3]; 
//    alarmclock_init( &alarms_obj ); 
//	
//	uint32_t *pdest = conf_file_read( CONF_FILE_NORMAL ); 
//	if( pdest != NULL){
//		dev_conf_update_from_memory( pdest ); 
//		return ; 
//	}
//		
//	pdest = conf_file_read( CONF_FILE_BACKUP); 
//	if( pdest != NULL){
//		dev_conf_update_from_memory( pdest ); 
//		return ; 
//	}
	
//	pdest = conf_file_read( CONF_FILE_FACTORY ); 
//	if( pdest != NULL){
//		dev_conf_update_from_memory( pdest ); 
//		return; 
//	}
	
	dev_conf_update_from_factory_default();
}

