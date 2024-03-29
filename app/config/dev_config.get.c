

#include "dev_config.get.h"

#include "dev_config.factory.h"


#if 1 // Property Get 
__INLINE uint32_t dev_config_get_module_id(void)
{
    return PROPERTY_MODULE_NAME; 
}


__INLINE char* dev_config_get_IMEI(void)
{
	return dev_property.imei; 
}

__INLINE char* dev_config_get_fw_ver_string(void)
{
	return PROPERTY_FW_VER_STRING; 
}

#endif 


#if 1 // System Configurate Get 
__INLINE uint8_t dev_config_get_sys_mode_mode(void)
{
	return dev_settings.system.mode.mode; 
}
__INLINE uint32_t dev_config_get_sys_mode_time(void)
{
	return dev_settings.system.mode.time; 
}

__INLINE uint16_t dev_config_get_work_start_time(void)
{
	return dev_settings.system.work_time.start_hm; 
}
__INLINE uint16_t dev_config_get_work_stop_time(void)
{
	return dev_settings.system.work_time.stop_hm; 
}
__INLINE uint16_t dev_config_get_no_disturb_start_time(void)
{
	return dev_settings.system.no_disturb.start_hm; 
}
__INLINE uint16_t dev_config_get_no_disturb_stop_time(void)
{
	return dev_settings.system.no_disturb.stop_hm; 
}


__INLINE int8_t dev_config_get_timezone_offset(void)
{
	return (int8_t)dev_settings.system.zone.offset; 
}

__INLINE bool dev_config_get_password_enable(void)
{
	return dev_settings.system.password.enable > 0; 
}
__INLINE uint32_t dev_config_get_password(void)
{
	return dev_settings.system.password.number; 
}


__INLINE bool dev_config_get_leds_switch_on(void)
{
	return dev_settings.system.switches.leds > 0; 
}
__INLINE bool dev_config_get_buzzer_switch(void)
{
	return dev_settings.system.switches.buzzer > 0;
}
__INLINE bool dev_config_get_motor_switch(void)
{
	return dev_settings.system.switches.motor > 0; 
}
__INLINE bool dev_config_get_agps_switch(void)
{
	return dev_settings.system.switches.AGPS > 0; 
}

__INLINE uint8_t dev_config_get_ringtone_level(void)
{
	return dev_settings.system.volume.ringtone; 
}
__INLINE uint8_t dev_config_get_mic_level(void)
{
	return dev_settings.system.volume.mic; 
}
__INLINE uint8_t dev_config_get_speaker_level(void)
{
	return dev_settings.system.volume.speaker; 
}

//const sys_dev_name_t * dev_config_get_dev_name(void)
//{
//	return &( dev_settings.system.dev_name ); 
//}


#endif 

#if 1 // Button Config 

const button_sense_t * dev_config_get_button_sense(void)
{
	return &( dev_settings.buttons.sense ); 
}

__INLINE uint8_t dev_config_get_buttons_task_by(uint8_t idx, uint8_t evt )
{	
	return dev_settings.buttons.opcode.task[ idx ][ evt ];
}

const button_action_t * dev_config_get_buttons_task(void)
{
	return &dev_settings.buttons.opcode; 
}



#endif 

#if 1 // GPRS 

const gprs_apn_t * dev_config_get_apn(void)
{
	return &( dev_settings.gprs.apn ); 
}
__INLINE bool dev_config_get_apn_enable(void)
{
	return dev_settings.gprs.apn.enable > 0; 
}
__INLINE const char* dev_config_get_apn_apn(void)
{
	return (dev_settings.gprs.apn.apns ); 
}
__INLINE const char* dev_config_get_apn_usrname(void)
{
	return (dev_settings.gprs.apn.username ); 
}
__INLINE const char* dev_config_get_apn_password(void)
{
	return (dev_settings.gprs.apn.password ); 
}


__INLINE bool dev_config_get_server_enable(void)
{
	return dev_settings.gprs.server.ip[0] != '\0'; 
}
__INLINE const char* dev_config_get_server_ip(void)
{
	return (dev_settings.gprs.server.ip ); 
}
__INLINE uint16_t dev_config_get_server_port(void)
{
	return dev_settings.gprs.server.port; 
}
__INLINE bool  dev_config_get_server_protocol_is_udp(void)
{
    return dev_settings.gprs.server.flag == 1; 
}

const gprs_server_t * dev_config_get_server(void)
{
	return &( dev_settings.gprs.server ); 
}

const gprs_time_t * dev_config_get_gprs_time(void)
{
	return &( dev_settings.gprs.interval ); 
}

__INLINE uint32_t dev_config_get_gprs_heartbeat_time(void)
{
	return dev_settings.gprs.interval.heartbeat; 
}
__INLINE uint32_t dev_config_get_gprs_upload_interval(void)
{
	return dev_settings.gprs.interval.upload; 
}
__INLINE uint32_t dev_config_get_gprs_lazy_upload_interval(void)
{
	return dev_settings.gprs.interval.lazy_upload; 
}

#endif 

#if 1 // Phone 

const phone_contact_t * dev_config_get_contact_by( uint8_t index )
{
	return &( dev_settings.phone.contacts[ index ] ); 
}

bool dev_config_get_contact_at_index_by( char* number, uint8_t * ret_index )
{
	for( uint32_t i = 0 ; i < PHONE_CONTACTS_NUM; i ++){
		if( dev_settings.phone.contacts[i].bit.enable == 0 )
			continue; 
		
		if( strcmp( dev_settings.phone.contacts[i].number, number ) == 0 ){
			*ret_index = i; 
			return true; 
		}			
	}
	return false; 
}
__INLINE bool dev_config_get_contact_enable_at( uint8_t index )
{
	return dev_settings.phone.contacts[index].bit.enable > 0; 
}
__INLINE bool dev_config_get_contact_sms_at(uint8_t index )
{
	return dev_settings.phone.contacts[index].bit.sms > 0; 
}
__INLINE bool dev_config_get_contact_phone_at(uint8_t index)
{
	return dev_settings.phone.contacts[index].bit.phone; 
}
__INLINE bool dev_config_get_contact_no_card_at(uint8_t index)
{
	return dev_settings.phone.contacts[index].bit.no_card; 
}
__INLINE const char* dev_config_get_contact_number_at(uint8_t index )
{
	return dev_settings.phone.contacts[index].number; 
}	

__INLINE uint8_t dev_config_get_contact_phone_enable_from(uint8_t index )
{
	while( index < PHONE_CONTACTS_NUM ){
		if( dev_settings.phone.contacts[ index].bit.enable > 0 
		&&	dev_settings.phone.contacts[ index ].bit.phone > 0 ){
			return index; 
		}
		index ++; 
	}
	return PHONE_CONTACTS_NUM; 
}

__INLINE uint8_t dev_config_get_contact_sms_enable_from(uint8_t index )
{
	while( index < PHONE_CONTACTS_NUM ){
		if( dev_settings.phone.contacts[index].bit.enable > 0 && dev_settings.phone.contacts[index].bit.sms > 0 ){
			return index; 
		}
		index ++; 
	}
	return PHONE_CONTACTS_NUM; 
}



const char * dev_config_get_sms_prefix_text(void)
{
	return dev_settings.phone.sms_prefix.text; 
}
__INLINE bool dev_config_get_sms_prefix_text_enable(void)
{
	return dev_settings.phone.sms_prefix.size > 0; 
}
__INLINE uint8_t dev_config_get_sms_prefix_text_size(void)
{
	return dev_settings.phone.sms_prefix.size; 
}


__INLINE uint16_t dev_config_get_sos_holding_time(void)
{
	return dev_settings.phone.SOS.holding_time; 
}
__INLINE uint8_t dev_config_get_sos_ring_time(void)
{
	return dev_settings.phone.SOS.rings_time; 
}
__INLINE uint8_t dev_config_get_sos_loops(void)
{
	return dev_settings.phone.SOS.loops; 
}

const phone_sos_t* dev_config_get_phone_sos(void)
{
	return &( dev_settings.phone.SOS ); 
}

__INLINE bool dev_config_get_auto_answer_enable(void)
{
	return dev_settings.phone.switches.auto_answer_enable > 0; 
}
__INLINE uint8_t dev_config_get_auto_answer_rings_need(void)
{
	return dev_settings.phone.switches.auto_answer_rings_delay ; 
}
__INLINE bool dev_config_get_intercept_excl_auth_enable(void)
{
	return dev_settings.phone.switches.intercept_except_auth > 0; 
}
__INLINE bool dev_config_get_hangup_manual_enable(void)
{
	return dev_settings.phone.switches.hangup_manual_not_allow > 0; 
}
__INLINE bool dev_config_get_monitor_environment_enable(void)
{
	return dev_settings.phone.switches.monitor_environment_enable > 0; 
}
__INLINE bool dev_config_get_sms_set_only_by_auth_number(void)
{
	return dev_settings.phone.switches.sms_set_only_by_auth_number > 0; 
}

#endif 

#if 1 // Alert 
__INLINE bool dev_config_get_alert_power_on(void)
{
	return dev_settings.alert.power.on > 0; 
}
__INLINE bool dev_config_get_alert_power_off(void)
{
	return dev_settings.alert.power.off > 0; 
}
__INLINE bool dev_config_get_alert_power_low(void)
{
	return dev_settings.alert.power.low > 0; 
}
__INLINE uint8_t dev_config_get_alert_power_low_threshold(void)
{
	return dev_settings.alert.power.threshold; 
}

const alert_geo_t * dev_config_get_geo_by(uint8_t index )
{
	return &( dev_settings.alert.geos[ index & ALERT_GEO_INDEX_MASK ] ); 
}
__INLINE bool dev_config_get_geo_enable_at(uint8_t index )
{
	return dev_settings.alert.geos[index & ALERT_GEO_INDEX_MASK].enable > 0; 
}
__INLINE bool dev_config_get_geo_in_fence_type(uint8_t index)
{
	return dev_settings.alert.geos[index & ALERT_GEO_INDEX_MASK].type > 0; 
}
__INLINE uint8_t dev_config_get_geo_points_count(uint8_t index){
	return dev_settings.alert.geos[index & ALERT_GEO_INDEX_MASK].count; 
}
__INLINE int32_t * dev_config_get_geo_points_addr(uint8_t index)
{
	return dev_settings.alert.geos[index & ALERT_GEO_INDEX_MASK].data; 
}
__INLINE bool dev_config_get_geo_dir_inside_at( uint8_t index )
{
	return dev_settings.alert.geos[index & ALERT_GEO_INDEX_MASK].dir > 0; 
}
__INLINE uint16_t dev_config_get_geo_radius_at(uint8_t index )
{
	return dev_settings.alert.geos[index & ALERT_GEO_INDEX_MASK ].radius; 
}
__INLINE int32_t dev_config_get_geo_longitude_at(uint8_t index )
{
	return dev_settings.alert.geos[index & ALERT_GEO_INDEX_MASK ].longitude; 
}
__INLINE int32_t dev_config_get_geo_latitude_at(uint8_t index )
{
	return dev_settings.alert.geos[index & ALERT_GEO_INDEX_MASK ].latitude; 
}


const alert_motion_t * dev_config_get_alert_motion(void)
{
	return &( dev_settings.alert.motion ); 
}
__INLINE bool dev_config_get_alert_motion_enable(void)
{
	return dev_settings.alert.motion.enable > 0; 
}
__INLINE uint16_t dev_config_get_alert_motion_setup_time(void)
{
	return dev_settings.alert.motion.setup_time; 
}
__INLINE uint16_t dev_config_get_alert_motion_action_time(void)
{
	return dev_settings.alert.motion.action_time; 
}

__INLINE bool dev_config_get_alert_actionless_enable(void)
{
	return dev_settings.alert.actionless.enable > 0 ; 
}
__INLINE uint32_t dev_config_get_alert_actionless_threshold(void)
{
	return dev_settings.alert.actionless.threshold; 
}

__INLINE bool dev_config_get_alert_speed_enable(void)
{
	return dev_settings.alert.speed.enable > 0; 
}
__INLINE uint16_t dev_config_get_alert_speed_threshold(void)
{
	return dev_settings.alert.speed.threshold; 
}

__INLINE bool dev_config_get_alert_tilt_enable(void)
{
	return dev_settings.alert.tilt.enable > 0; 
}
__INLINE uint8_t dev_config_get_alert_tilt_angle(void)
{
	return dev_settings.alert.tilt.angle ; 
}
__INLINE uint16_t dev_config_get_alert_tilt_threshold(void) //uint8_t ->uint16_t
{
	return dev_settings.alert.tilt.time; 
}

__INLINE bool dev_config_get_alert_falldown_enable(void)
{
	//return dev_settings.alert.falldown.mode & 0x03;
    return dev_settings.alert.falldown.enable;     
}
__INLINE bool dev_config_get_alert_falldown_call(void)
{
    //return dev_settings.alert.falldown.mode == 0x03;
    return dev_settings.alert.falldown.call;     
}
__INLINE uint8_t dev_config_get_alert_falldown_level(void)
{
	return dev_settings.alert.falldown.level; 
}


#endif 





