

#ifndef DEV_CONFIG_GET_H__
#define DEV_CONFIG_GET_H__


#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "app_util.h"

#include "dev_conf_defines.h"
#include "compiler_abstraction.h"

 uint32_t dev_config_get_module_id(void); 
//const sys_mode_t * dev_config_get_sys_mode(void); 

 char* dev_config_get_IMEI(void); 
 char* dev_config_get_fw_ver_string(void); 

 uint8_t dev_config_get_sys_mode_mode(void);
 uint32_t dev_config_get_sys_mode_time(void);
 int8_t dev_config_get_timezone_offset(void);
 
 uint16_t dev_config_get_work_start_time(void); 
 uint16_t dev_config_get_work_stop_time(void); 
 uint16_t dev_config_get_no_disturb_start_time(void); 
 uint16_t dev_config_get_no_disturb_stop_time(void); 

 bool dev_config_get_password_enable(void);
 uint32_t dev_config_get_password(void);

 bool dev_config_get_leds_switch_on(void);
 bool dev_config_get_buzzer_switch(void);
 bool dev_config_get_motor_switch(void);
 bool dev_config_get_agps_switch(void);

 uint8_t dev_config_get_ringtone_level(void);
 uint8_t dev_config_get_mic_level(void);
 uint8_t dev_config_get_speaker_level(void);

//const sys_dev_name_t * dev_config_get_dev_name(void);

const button_sense_t * dev_config_get_button_sense(void);
 uint8_t dev_config_get_buttons_task_by(uint8_t idx, uint8_t evt );
const button_action_t * dev_config_get_buttons_task(void);


const gprs_apn_t * dev_config_get_apn(void);
 bool dev_config_get_apn_enable(void); 
 const char* dev_config_get_apn_apn(void);
 const char* dev_config_get_apn_usrname(void);
 const char* dev_config_get_apn_password(void); 


const gprs_server_t * dev_config_get_server(void);
 bool dev_config_get_server_enable(void); 
 const char* dev_config_get_server_ip(void); 
 uint16_t dev_config_get_server_port(void);
 bool  dev_config_get_server_protocol_is_udp(void); 

const gprs_time_t * dev_config_get_gprs_time(void);
 uint32_t dev_config_get_gprs_heartbeat_time(void);
 uint32_t dev_config_get_gprs_upload_interval(void);
 uint32_t dev_config_get_gprs_lazy_upload_interval(void);

const phone_contact_t * dev_config_get_contact_by( uint8_t index );
 bool dev_config_get_contact_at_index_by( char* number, uint8_t * ret_index ); 
 bool dev_config_get_contact_enable_at( uint8_t index );
 bool dev_config_get_contact_sms_at(uint8_t index );
 bool dev_config_get_contact_phone_at(uint8_t index);
 bool dev_config_get_contact_no_card_at(uint8_t index);
 const char* dev_config_get_contact_number_at(uint8_t index );
 uint8_t dev_config_get_contact_phone_enable_from(uint8_t index ); 
 uint8_t dev_config_get_contact_sms_enable_from(uint8_t index );
 

 const char * dev_config_get_sms_prefix_text(void);
 bool dev_config_get_sms_prefix_text_enable(void); 
 uint8_t dev_config_get_sms_prefix_text_size(void); 

 uint16_t dev_config_get_sos_holding_time(void);
 uint8_t dev_config_get_sos_ring_time(void);
 uint8_t dev_config_get_sos_loops(void);
const phone_sos_t* dev_config_get_phone_sos(void);

 bool dev_config_get_auto_answer_enable(void);
 uint8_t dev_config_get_auto_answer_rings_need(void);
 bool dev_config_get_intercept_excl_auth_enable(void);
 bool dev_config_get_hangup_manual_enable(void);
 bool dev_config_get_monitor_environment_enable(void);
 bool dev_config_get_sms_set_only_by_auth_number(void);

 bool dev_config_get_alert_power_on(void);
 bool dev_config_get_alert_power_off(void);
 bool dev_config_get_alert_power_low(void);
 uint8_t dev_config_get_alert_power_low_threshold(void); 

const alert_geo_t * dev_config_get_geo_by(uint8_t index );
 bool dev_config_get_geo_enable_at(uint8_t index );
 bool dev_config_get_geo_in_fence_type(uint8_t index);
 uint8_t dev_config_get_geo_points_count(uint8_t index); 
 int32_t * dev_config_get_geo_points_addr(uint8_t index); 
 bool dev_config_get_geo_dir_inside_at( uint8_t index );
 uint16_t dev_config_get_geo_radius_at(uint8_t index );
 int32_t dev_config_get_geo_longitude_at(uint8_t index ); 
 int32_t dev_config_get_geo_latitude_at(uint8_t index );

const alert_motion_t * dev_config_get_alert_motion(void);
 bool dev_config_get_alert_motion_enable(void);
 uint16_t dev_config_get_alert_motion_setup_time(void);

 uint16_t dev_config_get_alert_motion_action_time(void);
 bool dev_config_get_alert_actionless_enable(void);
 uint32_t dev_config_get_alert_actionless_threshold(void);


 bool dev_config_get_alert_speed_enable(void);
 uint16_t dev_config_get_alert_speed_threshold(void);

 bool dev_config_get_alert_tilt_enable(void);
 uint8_t dev_config_get_alert_tilt_angle(void);
 uint8_t dev_config_get_alert_tilt_threshold(void);
 bool dev_config_get_alert_falldown_enable(void);
 bool dev_config_get_alert_falldown_call(void);
 uint8_t dev_config_get_alert_falldown_level(void);



#endif 

