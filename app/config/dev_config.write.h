

#ifndef DEV_CONF_WRITE_H__
#define DEV_CONF_WRITE_H__


void dfw_property_imei_update( char * imei ); 
void dfw_property_iccid_update( char* iccid ); 

// System 
bool dfw_system_mode_update(uint8_t mode, uint32_t time );
bool dfw_system_timezone_update( int8_t offset);
bool dfw_system_password_update(bool enable, uint32_t value);
bool dfw_system_password_protect(bool enable); 

void dfw_system_switch_leds( bool on ); 
void dfw_system_switch_motor( bool on );
void dfw_system_switch_buzzer( bool on );
void dfw_system_switch_agps(bool on );
bool dfw_system_switches_update( uint32_t value );

bool dfw_system_volume_ringtone_update( uint8_t level );
bool dfw_system_volume_mic_update( uint8_t level );
bool dfw_system_volume_speaker_update( uint8_t level );
bool dfw_system_dev_name_update( const uint8_t * p_data, uint8_t len);

bool dfw_buttons_sense_update( const uint8_t * p_data, uint8_t size );
bool dfw_buttons_op_update( uint8_t idx, uint8_t evt_type, uint8_t task ); 
bool dfw_buttons_side_button_dial_no( uint8_t contact ); 

bool dfw_gprs_apn_enable_set( bool enable );
bool dfw_gprs_apn_apn_update( const char* apnStr );
bool dfw_gprs_apn_user_name_update( const char* usrStr );
bool dfw_gprs_apn_password_update( const char* pwdStr );
bool dfw_gprs_apn_update( bool enable, const char* apn, const char* usrname, const char* password); 
bool dfw_gprs_apn_write( const uint8_t * p_data, uint8_t len );

bool dfw_gprs_server_update( const char* ip, uint16_t port);
bool dfw_gprs_server_write( const uint8_t * p_data, uint8_t len );

bool dfw_gprs_heartbeat_update( uint32_t interval );
bool dfw_gprs_upload_interval_update( uint32_t interval );
bool dfw_gprs_upload_lazy_interval_update( uint32_t interval );

bool dfw_phone_contact_delete_at( uint8_t no );
bool dfw_phone_contact_update(uint8_t no, bool sms, bool phone, const char* number);
bool dfw_phone_contact_write( const uint8_t * p_data, uint8_t len );

bool dfw_phone_sms_prefix_update( bool enable, const char* prefix_text); 
bool dfw_phone_sms_prefix_write( const uint8_t * p_data, uint8_t len );

bool dfw_phone_sos_holding_time_update( uint16_t holding_time); 
bool dfw_phone_sos_ring_time_update( uint8_t ring_time);
bool dfw_phone_sos_loops_update( uint8_t loops ); 
bool dfw_phone_sos_write( const uint8_t * p_data, uint8_t len );

bool dfw_phone_switch_auto_answer_update( uint8_t enable, uint8_t rings);
bool dfw_phone_switch_intercept_update( uint8_t enable );
bool dfw_phone_switch_hangup_update( uint8_t enable );
bool dfw_phone_switch_monitor_update( uint8_t enable );
bool dfw_phone_switch_set_update( uint8_t enable );
bool dfw_phone_switches_update( const uint8_t * p_data, uint8_t len );

bool dfw_alert_power_low_update( uint8_t enable, uint8_t threshold );
bool dfw_alert_power_on_update( uint8_t enable );
bool dfw_alert_power_off_update( uint8_t enable );
bool dfw_alert_power_update( const uint8_t * p_data, uint8_t len);

bool dfw_alert_geo_delete_at( uint8_t no ); 
bool dfw_alert_geo_update( uint8_t no, uint8_t enable, uint8_t dir, uint16_t radius, int32_t longitude, int32_t latitude ); 
bool dfw_alert_geo_write( const uint8_t * p_data, uint8_t len );

bool dfw_alert_motion_update( bool enable, uint16_t setup_time, uint16_t action_time);
bool dfw_alert_motion_write( const uint8_t * p_data, uint8_t len );

bool dfw_alert_actionless_update( bool enable, uint32_t threshold);
bool dfw_alert_actionless_write( const uint8_t * p_data, uint8_t len);

bool dfw_alert_speed_update( bool enable, uint32_t threshold);
bool dfw_alert_speed_write( const uint8_t * p_data, uint8_t len);

bool dfw_alert_tilt_update( bool enable, uint8_t angle, uint16_t time);
bool dfw_alert_tilt_write( const uint8_t * p_data, uint8_t len );

bool dfw_alert_falldown_update( uint8_t mode, uint8_t level );
bool dfw_alert_falldown_write( const uint8_t * p_data, uint8_t len );

bool dfw_settings_update( const uint8_t * p_data, uint16_t size );

#endif 

