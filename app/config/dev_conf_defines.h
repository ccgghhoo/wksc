


#ifndef DEV_CONF_DEFINES_H__
#define DEV_CONF_DEFINES_H__


#include <stdint.h>
//#include "conf_file.h"

//#include "alarm_clock.h"

#define CONFIG_MAGIC_CODE 	0xDEF1C0DE // device config code 

#define DEV_IMEI_DATA_SIZE 	15 
#define DEV_ICCID_DATA_SIZE 20 

#define MODULE_NUMBER_SIZE		4 
#define FIRMWARE_VERSION_SIZE	4 


#define SYSTEM_MODES_MODE_START	    1 
#define SYSTEM_MODES_MODE_END	    5 
#define SYSTEM_MODE_TIME_MIN 	    30 
#define SYSTEM_MODE_TIME_MAX	    (60*60*10)

#define SYSTEM_TIMEZONE_UNIT		15 // minutes 
#define SYSTEM_TIMEZONE_MAX_OFFSET 	(14*(60/SYSTEM_TIMEZONE_UNIT))
#define SYSTEM_TIMEZONE_MIN_OFFSET  -(12*(60/SYSTEM_TIMEZONE_UNIT))

#define SYSTEM_PASSWORD_MAX_NUMBER	999999 

#define SYSTEM_RINGTONE_LEVEL_MIN   0 
#define SYSTEM_RINGTONE_LEVEL_MAX   100 
#define SYSTEM_SPEAKER_LEVEL_MIN    0 
#define SYSTEM_SPEAKER_LEVEL_MAX    100 
#define SYSTEM_MIC_LEVEL_MIN        0 
#define SYSTEM_MIC_LEVEL_MAX        15 

//#define SYSTEM_DEVICE_NAME_LEN_MAX 	15 

#define BUTTONS_SENSE_SIZE		    8 

#define BUTTONS_CLICK_TIME_MIN	        5  //unit: 10ms 
#define BUTTONS_CLICK_TIME_MAX          20 //
#define BUTTONS_DOUBLE_CLICK_TIME_MIN   70 
#define BUTTONS_DOUBLE_CLICK_TIME_MAX   200// 
#define BUTTONS_SHORT_HOLD_TIME_MIN     100 
#define BUTTONS_SHORT_HOLD_TIME_MAX     200 
#define BUTTONS_LONG_HOLD_TIME_MIN      200 
#define BUTTONS_LONG_HOLD_TIME_MAX      500 

#define BUTTONS_OP_EVENT_MIN	2
#define BUTTONS_OP_EVENT_MAX 	5 
#define BUTTONS_OP_INDEX_MIN 	1 
#define BUTTONS_OP_INDEX_MAX 	7 

#define BUTTONS_CLICK_IDX		    0
#define BUTTONS_DOUBLE_CLICK_IDX	1 
#define BUTTONS_SHORT_HOLD_IDX		2 
#define BUTTONS_LONG_HOLD_IDX		3 

#define GPRS_APN_APN_LEN_MAX	31 
#define GPRS_APN_USERNAME_LEN_MAX 16 
#define GPRS_APN_PASSWORD_LEN_MAX 16 

#define GPRS_SERVER_IP_LEM_MIN 5 
#define GPRS_SERVER_IP_LEN_MAX 32

#define GPRS_HEARTBEAT_INTERVAL_MIN		10 // unit: second 
#define GPRS_HEARTBEAT_INTERVAL_MAX 	(60*60*2) // half an  hour 
#define GPRS_UPLOAD_INTERVAL_MIN		30 // unit: second 
#define GPRS_UPLOAD_INTERVAL_MAX 		(24*60*60) // 24 hour 
#define GPRS_UPLOAD_LAZY_MIN			(60*10) // 10minute 
#define GPRS_UPLOAD_LAZY_MAX 			(24*60*60) 

#define PHONE_CONTACTS_NUM				10 
#define PHONE_CONTACT_NO_MIN			1 
#define PHONE_CONTACT_NO_MAX			10 
#define PHONE_CONTACT_LEN_MIN			1 
#define PHONE_CONTACT_LEN_MAX 			20 
#define PHONE_CONTACT_NUMBER_SIZE		19 
#define PHONE_CONTACT_INDEX_MAX 		9 //0~9  

#define PHONE_SMS_PREFIX_LEN_MAX		19 

#define PHONE_SOS_DATA_SIZE				4 
#define PHONE_SOS_HOLDING_TIME_MIN		120 //unit: second 
#define PHONE_SOS_HOLDING_TIME_MAX 		(24*60*60)// 5 hours 
#define PHONE_SOS_RING_TIME_MIN			5 
#define PHONE_SOS_RING_TIME_MAX 		60 
#define PHONE_SOS_LOOP_TIMES_MAX		10 

#define PHONE_SWITCHES_DATA_SIZE 		4 
#define PHONE_AUTO_ANSWER_RINGS_MAX 	10 

#define ALERT_POWER_DATA_SIZE 			1 
#define ALERT_POWER_LOW_LEVEL_MIN       0
#define ALERT_POWER_LOW_LEVEL_MAX 		30 

#define ALERT_GEO_DATA_SIZE_MIN			4 
#define ALERT_GEO_DATA_SIZE_MAX 		(4 + (16*4))
#define ALERT_GEO_START_INDEX			1 
#define ALERT_GEO_END_INDEX				4 
#define ALERT_GEO_NUMBERS				4
#define ALERT_GEO_INDEX_MASK			3 
#define ALERT_GEO_RADIUS_MAX 			(50000)	// unit: meter 
#define ALERT_GEO_RADIUS_MIN 			( 100 ) 
#define ALERT_GEO_CIRCLE_POINTS         ( 1 ) 
#define ALERT_GEO_FENCE_POINTS			( 8 ) 

#define ALERT_MOTION_DATA_SIZE 			4 
#define ALERT_MOTION_SETUP_TIME_MIN		60 
#define ALERT_MOTION_SETUP_TIME_MAX 	(10*3600 ) // 10 hours //chen
#define ALERT_MOTION_ACTION_TIME_MIN	1 
#define ALERT_MOTION_ACTION_TIME_MAX 	(60 )      //chen 

#define ALERT_STATIC_DATA_SIZE 			4 
#define ALERT_STATIC_THRESHOLD_MIN		60 
#define ALERT_STATIC_THRESHOLD_MAX		(10*3600) 

#define ALERT_SPEED_DATA_SIZE 			4 
#define ALERT_SPEED_THRESHOLD_MIN		(2*1000)
#define ALERT_SPEED_THRESHOLD_MAX 		(400*1000) // 100km/h 

#define ALERT_TILT_DATA_SIZE 			4 
#define ALERT_TILT_ANGLE_MIN			1//30 
#define ALERT_TILT_ANGLE_MAX 			90 
#define ALERT_TILT_TIME_MIN				10//1 
#define ALERT_TILT_TIME_MAX 			3600//250 //chen

#define ALERT_FALLDOWN_DATA_SIZE 		1//4  //chen
#define ALERT_FALLDOWN_MODE_NUMS 		3 
#define ALERT_FALLDOWN_LEVEL_MIN		1 
#define ALERT_FALLDOWN_LEVEL_MAX		9 

#define DEV_CONFIG_DATA_SIZE_MAX		sizeof(dev_config_t) + ( 25 * 2 )
#define DEV_CONFIG_DATA_LEN				(sizeof(dev_config_t) - 12 )
	

typedef enum{
	BUTTONS_OP_DO_NOTHINGS = 0, 
	
	BUTTONS_OP_SOS_ALERT, 
	
	BUTTONS_OP_DIAL_1ST_NUMBER = 0x10, 
	BUTTONS_OP_DIAL_2ND_NUMBER, 
	BUTTONS_OP_DIAL_3RD_NUMBER, 
	BUTTONS_OP_DIAL_4TH_NUMBER, 
	BUTTONS_OP_DIAL_5TH_NUMBER, 
	BUTTONS_OP_DIAL_6TH_NUMBER, 
	BUTTONS_OP_DIAL_7TH_NUMBER, 
	BUTTONS_OP_DIAL_8TH_NUMBER, 
	BUTTONS_OP_DIAL_9TH_NUMBER, 
	BUTTONS_OP_DIAL_10TH_NUMBER, 
	
	BUTTONS_OP_POWER_OFF = 0xFE, 
}BUTTONS_OP_CODE; 

typedef enum{
	BUTTONS_EVT_CLICK = 0, 
	BUTTONS_EVT_DOUBLE_CLICK, 
	BUTTONS_EVT_SHORT_HOLD, 
	BUTTONS_EVT_LONG_HOLD, 
	BUTTONS_EVT_END, 
}BUTTONS_OP_EVTS; 

typedef enum{
	BUTTONS_IDX_NONE = 0, 
	BUTTONS_IDX_PRIM, 
	BUTTONS_IDX_SIDEL,
	BUTTONS_IDX_PRIM_SIDEL, 
	BUTTONS_IDX_SIDER, 
	BUTTONS_IDX_PRIM_SIDER, 
	BUTTONS_IDX_SIDELR, 
	BUTTONS_IDX_ALL, 
}BUTTONS_OP_IDX; 

#if defined(__CC_ARM)
    #pragma push
    #pragma anon_unions
#elif defined(__ICCARM__)
    #pragma language=extended
#elif defined(__GNUC__)
    // anonymous unions are enabled by default
#endif

//=================================================
// 

typedef struct{
	char imei[ DEV_IMEI_DATA_SIZE ];
	char iccid[ DEV_ICCID_DATA_SIZE ];
}dev_property_t; 



//=================================================
// Button 
typedef struct{ //button_sense_t
	uint16_t 	click; 
	uint16_t 	double_click; 
	uint16_t 	short_hold; 
	uint16_t 	long_hold; 
}button_sense_t; 
typedef struct{ //button_action_t
	uint8_t     task[8][4];
}button_action_t;

typedef struct{//button_settings_t
	button_sense_t sense; 
	button_action_t opcode; 
}button_settings_t; 

//=================================================
// Settings 
typedef union{//sys_mode_t
	struct{
		uint32_t	time:	24; 
		uint32_t 	mode:	8; 		
	};
	uint32_t value; 
}sys_mode_t; 
typedef union{
	struct{
		uint8_t start_hour ; 
		uint8_t start_minute;
		uint8_t end_hour;
		uint8_t end_minute; 
	};
	struct{
		uint16_t start_hm; 
		uint16_t stop_hm; 
	};
	uint32_t value; 
}sys_work_time_t;
typedef union{
	struct{
		uint8_t start_hour; 
		uint8_t start_minute;
		uint8_t end_hour;
		uint8_t end_minute; 
	};
	struct{
		uint16_t start_hm; 
		uint16_t stop_hm; 
	};
	uint32_t value; 
}system_no_disturb_t; 
typedef union{//sys_time_zone_t
	int8_t 	offset;
	uint32_t value; 
}sys_time_zone_t;
typedef union{//sys_password_t
	struct{		
		uint32_t 	number:	31; 	
		uint32_t 	enable:	1; 
	};
	uint32_t value; 
}sys_password_t; 
typedef union{
	struct{//sys_switchs_t
		uint32_t 	leds:	1; 
		uint32_t 	buzzer:	1; 
		uint32_t 	motor:	1; 
		uint32_t 	AGPS:	1; 
		
		uint32_t 	reserved: 28; 		
	};
	uint32_t value; 
}sys_switchs_t; 
typedef union{
	struct{//sys_volume_t
	uint8_t 	ringtone;
	uint8_t 	mic;
	uint8_t 	speaker;
	};
	uint32_t value; 
}sys_volume_t; 
typedef struct{//sys_settings_t
	sys_mode_t	mode; 
	sys_work_time_t work_time; 
	system_no_disturb_t no_disturb; 
	sys_time_zone_t zone; 
	sys_password_t password; 
	sys_switchs_t switches; 
	sys_volume_t volume; 
//    alarm_clock_t alarms[4]; 
}sys_settings_t; 

//=================================================
typedef struct{//phone_contact_t
	union{
		struct{
			uint8_t idx:	4; 	
			uint8_t no_card:1; 
			uint8_t sms:	1; 
			uint8_t	phone:	1; 
			uint8_t enable:	1; 	
		};
		uint8_t 	value; 
	}bit; 
	
	char 	number[ 19 ]; 
}phone_contact_t; 
typedef struct{//phone_sms_prefix_t
	uint8_t 	size; 
	char 	text[19];
}phone_sms_prefix_t; 
typedef union{//phone_sos_t
	struct{	
		uint32_t holding_time: 16; 
		uint32_t rings_time: 8;
		uint32_t loops: 8;
	};
	uint32_t value; 
}phone_sos_t; 
typedef union{
	struct{//phone_swithes_t
	uint32_t 	auto_answer_rings_delay: 7; 
	uint32_t 	auto_answer_enable: 1; 
		
	uint32_t 	intercept_except_auth:	1; //only authorized number 
	uint32_t 	hangup_manual_not_allow: 1; // 
	uint32_t 	monitor_environment_enable: 1; 
	uint32_t 	sms_set_only_by_auth_number: 1; 
	
	uint32_t 	reserved: 20; 	
	};
	uint32_t value; 
}phone_swithes_t; 
typedef struct{//phone_settings_t
	phone_contact_t	contacts[10]; 
	phone_sms_prefix_t sms_prefix; 
	phone_swithes_t	switches; 
	phone_sos_t SOS; 
}phone_settings_t; 
//=================================================
// GPRS 
typedef struct{	//gprs_apn_t
	uint8_t enable; 
	uint8_t apnLen;
	uint8_t userNameLen;
	uint8_t passwordLen; 
	
	char apns[32];
	char username[16];
	char password[16];
}gprs_apn_t; 
typedef struct{//gprs_server_t
    uint16_t flag; 
	uint16_t port;
	char ip[32]; 	
}gprs_server_t; 
typedef struct{//gprs_time_t
	uint32_t heartbeat; 
	uint32_t upload; 
	uint32_t lazy_upload; 	
}gprs_time_t; 
typedef struct{//gprs_settings_t
	gprs_apn_t apn; 
	gprs_server_t server; 
	gprs_time_t interval;
}gprs_settings_t; 
//=================================================
typedef union{//alert_power_t
	struct{
		uint32_t threshold: 5; 
		uint32_t low:1;
		uint32_t on:1; 
		uint32_t off:1; 		
	};
	uint32_t value; 
}alert_power_t; 
typedef struct{//alert_geo_t
	union{
		struct{
			uint32_t idx:4; 
			uint32_t count:4;// count of data long&lat
			
			uint32_t enable:1; 
			uint32_t dir: 1; 
			uint32_t type:1;	// 0 use radius , 1 use fence 
			uint32_t reserved: 5;  			
			
			uint32_t radius:16; 
		};
		uint32_t value; 
	}; 
	union{
		int32_t data[16]; 	// 8 lines at most 
		struct{
			int32_t longitude;
			int32_t latitude; 
		};
	};
}alert_geo_t; 	
typedef union{//alert_motion_t
	struct{		
		uint32_t setup_time: 16; //static time to setup alertion 
		uint32_t action_time: 14; //15 motion time to generate alert 
        uint32_t dial:1;          //
		uint32_t enable:1; 
	};
	uint32_t value; 
}alert_motion_t; 
typedef union{//alert_static_t
	struct{
		uint32_t threshold: 30; //31
        uint32_t dial: 1;       //
		uint32_t enable: 1; 		
	};
	uint32_t value; 
}alert_static_t; 
typedef union{//alert_speed_t
	struct{
		uint32_t threshold:31;
		uint32_t enable: 1;		
	};
	uint32_t value; 
}alert_speed_t; 
typedef union{//alert_tilt_t
	struct{
		uint32_t time: 16;  //8
		uint32_t angle: 8;	//7	
        uint32_t reserve: 6;
        uint32_t dial: 1;
		uint32_t enable: 1; 
	};
	uint32_t value;
}alert_tilt_t; 
typedef union{//alert_falldown_t
	/*struct{
		uint32_t mode: 8; 
		uint32_t level: 8;
	};
	uint32_t value;
    */
    struct{   //chen 2019/07/15
		uint8_t level: 4; 
		uint8_t reserve: 2;
        uint8_t call: 1;
        uint8_t enable: 1;
	};    
	uint8_t value;
    
}alert_falldown_t; 
typedef struct{
	alert_power_t power; 
	alert_geo_t geos[ ALERT_GEO_NUMBERS ]; 
	alert_motion_t motion; 
	alert_static_t actionless; 
	alert_speed_t speed; 
	alert_tilt_t tilt; 
	alert_falldown_t falldown; 
}alert_settings_t; 


typedef struct{

//	conf_file_header_t	head;
	uint32_t 		version; 
	
	alert_settings_t alert; 
	button_settings_t buttons;
	gprs_settings_t gprs; 
	sys_settings_t	system;
	phone_settings_t phone; 
	
}dev_config_t; 


#if defined(__CC_ARM)
    #pragma pop
#elif defined(__ICCARM__)
    // leave anonymous unions enabled
#elif defined(__GNUC__)
    // anonymous unions are enabled by default
#endif

typedef void (*dev_settings_update_handler_t)(uint32_t key ); 



extern dev_property_t dev_property; 
extern dev_config_t dev_settings; 
extern dev_settings_update_handler_t m_dev_cb; 


#endif 
