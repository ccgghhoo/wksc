

#ifndef DEV_CONF_PB_H__
#define DEV_CONF_PB_H__

typedef enum{
    DEV_UPDATE_ID_BASE = 0, 
    DEV_UPDATE_WORK_MODE,
    DEV_UPDATE_NO_DISTURB, 
    DEV_UPDATE_TIMEZONE,
    DEV_UPDATE_SYSTEM_SWITCHES, 
    DEV_UPDATE_VOLUME_MIC,
    DEV_UPDATE_VOLUME_RINGTONE,
    DEV_UPDATE_VOLUME_SPEAKER, 
    DEV_UPDATE_BUTTONS_SENSE, 
    
    DEV_UPDATE_ALERT_MOTION,
    DEV_UPDATE_ALERT_STATIC, 
    DEV_UPDATE_ALERT_TILT, 
    DEV_UPDATE_ALERT_FALLDOWN, 
    
    
    
}dev_update_id_t; 


typedef enum{
	DEV_CONF_MODULE = 0x01,  
	DEV_CONF_FIRMWARE, 
	DEV_CONF_IMEI , 
	DEV_CONF_ICCID , 
	DEV_CONF_MAC,
	DEV_CONF_DATETIME, 
	DEV_CONF_RUNTIME, 
	DEV_CONF_FIRMWARE_INFO, 
	
	// System 
	DEV_CONF_SYSTEM_MODE = 0x10, 
	DEV_CONF_SYSTEM_WORK_TIME, 
	DEV_CONF_SYSTEM_NO_DISTURB, 
	DEV_CONF_SYSTEM_PASSWORD, 
	DEV_CONF_SYSTEM_TIMEZONE, 
	DEV_CONF_SYSTEM_SWITCHES, 
	DEV_CONF_SYSTEM_VOLUME, 
    DEV_CONF_SYSTEM_ALARMCLOCK, 
//	DEV_CONF_SYSTEM_DEVICE_NAME, 
	
	// Button 
	DEV_CONF_BUTTONS_SENSE = 0x20, 
	DEV_CONF_BUTTONS_OP, 
	
	// Phone 
	DEV_CONF_PHONE_CONTACTS, 
	DEV_CONF_PHONE_SMS_PREFIX, 
	DEV_CONF_PHONE_SOS_OPTION, 
	DEV_CONF_PHONE_SWITCHES, 
	
	// GPRS 0x26 
	DEV_CONF_GPRS_APN, 
	DEV_CONF_GPRS_SERVER_PARAM, 
	DEV_CONF_GPRS_TIME, 

	// ALERT 0x29 
	DEV_CONF_ALERT_POWER, 
	DEV_CONF_ALERT_GEO, 
	DEV_CONF_ALERT_MOTION, 
	DEV_CONF_ALERT_STATIC, 
	DEV_CONF_ALERT_SPEED, 
	DEV_CONF_ALERT_TILT, 
	DEV_CONF_ALERT_FALLDOWN, 
	
	DEV_CONF_READ_SETTINGS = 0xF0,		
	DEV_CONF_SAVE_PROFILE = 0xFE, 
	
}DEV_CONF_PB_ENUM; 



#endif 
