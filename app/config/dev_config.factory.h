

#ifndef DEV_CONFIG_FACTORY_H__
#define DEV_CONFIG_FACTORY_H__

// <<< Use Configuration Wizard in Context Menu >>>\n

// <h> Property 
// <o> Device Module Name 
// <i> define this project module code 
#ifndef PROPERTY_MODULE_NAME 
#define PROPERTY_MODULE_NAME	0x20170907
#endif 

// <o> Firmware Version 
#ifndef PROPERTY_FIRMWARE_VER 
#define PROPERTY_FIRMWARE_VER	0x01000001
#endif 

// <s> Firmware Version String 
#ifndef PROPERTY_FW_VER_STRING
#define PROPERTY_FW_VER_STRING	"v1.0.0.1"
#endif 

// </h> !Property 

// <h> System Configuration 

// <h> Work Mode 
// <o> Mode
// <1=> Mode 1 
// <2=> Mode 2 
// <3=> Mode 3 
// <4=> Mode 4 
// <5=> Mode 5 
#ifndef SYSTEM_MODE_MODE 
#define SYSTEM_MODE_MODE	2 
#endif 

// <o> Time (Unit: second ) 
#ifndef SYSTEM_MODE_TIME 
#define SYSTEM_MODE_TIME	180 
#endif 

// </h> Work Mode 

// <e> GSM & GPS Work Time 
#ifndef SYSTEM_WORK_TIME_ENABLE 
#define SYSTEM_WORK_TIME_ENABLE 0 
#endif 

// <o> start hour 
// <0-23>
#ifndef SYSTEM_WORK_TIME_START_HOUR
#define SYSTEM_WORK_TIME_START_HOUR  0 
#endif 

// <o> start minute 
// <0-59>
#ifndef SYSTEM_WORK_TIME_START_MINUTE
#define SYSTEM_WORK_TIME_START_MINUTE	0 
#endif 

// <o> end hourt 
// <0-23> 
#ifndef SYSTEM_WORK_TIME_END_HOUR
#define SYSTEM_WORK_TIME_END_HOUR	0 
#endif 

// <o> end minute 
// <0-59>
#ifndef SYSTEM_WORK_TIME_END_MINUTE
#define SYSTEM_WORK_TIME_END_MINUTE	0 
#endif 

#ifndef SYSTEM_WORK_TIME_START
#define SYSTEM_WORK_TIME_START	( (SYSTEM_WORK_TIME_START_HOUR << 8 ) | SYSTEM_WORK_TIME_START_MINUTE)
#endif 

#ifndef SYSTEM_WORK_TIME_END
#define SYSTEM_WORK_TIME_END	( (SYSTEM_WORK_TIME_END_HOUR << 8 ) | SYSTEM_WORK_TIME_END_MINUTE )
#endif 

// </e> !GSM & GPS Work Time

// <e> No disturb Settings 
#ifndef SYSTEM_NO_DISTURB_TIME_ENABLE 
#define SYSTEM_NO_DISTURB_TIME_ENABLE 0 
#endif 

// <o> start hour 
// <0-23>
#ifndef SYSTEM_NO_DISTURB_TIME_START_HOUR
#define SYSTEM_NO_DISTURB_TIME_START_HOUR  0 
#endif 

// <o> start minute 
// <0-59>
#ifndef SYSTEM_NO_DISTURB_TIME_START_MINUTE
#define SYSTEM_NO_DISTURB_TIME_START_MINUTE	0 
#endif 

// <o> end hourt 
// <0-23> 
#ifndef SYSTEM_NO_DISTURB_TIME_END_HOUR
#define SYSTEM_NO_DISTURB_TIME_END_HOUR	0 
#endif 

// <o> end minute 
// <0-59>
#ifndef SYSTEM_NO_DISTURB_TIME_END_MINUTE
#define SYSTEM_NO_DISTURB_TIME_END_MINUTE	0 
#endif 

#ifndef SYSTEM_NO_DISTURB_TIME_START
#define SYSTEM_NO_DISTURB_TIME_START	( (SYSTEM_NO_DISTURB_TIME_START_HOUR << 8 ) | SYSTEM_NO_DISTURB_TIME_START_MINUTE)
#endif 

#ifndef SYSTEM_NO_DISTURB_TIME_END
#define SYSTEM_NO_DISTURB_TIME_END	( (SYSTEM_NO_DISTURB_TIME_END_HOUR << 8 ) | SYSTEM_NO_DISTURB_TIME_END_MINUTE )
#endif 

// </e> ! No disturb Settings

// <e> TimeZone Offset 
#ifndef SYSTEM_TIME_ZONE_ENABLE
#define SYSTEM_TIME_ZONE_ENABLE		0 
#endif 

// <q> Offset Sign 
#ifndef SYSTEM_TIME_ZONE_SIGN
#define SYSTEM_TIME_ZONE_SIGN		0 
#endif 


#ifndef SYSTEM_TIME_ZONE_UNIT
#define SYSTEM_TIME_ZONE_UNIT	15
#endif 

// <o> Timezone Offset ( hours )
// <#*4> 
#ifndef SYSTEM_TIME_ZONE_VALUE	
#define SYSTEM_TIME_ZONE_VALUE		32
#endif 

#if SYSTEM_TIME_ZONE_SIGN > 0 
#define SYSTEM_TIME_ZONE_OFFSET		(SYSTEM_TIME_ZONE_VALUE * (-1))
#else 
#define SYSTEM_TIME_ZONE_OFFSET 	SYSTEM_TIME_ZONE_VALUE
#endif 

// </e> !TimeZone Offset 

// <e> Password Protect
#ifndef SYSTEM_PASSWORD_ENABLE
#define SYSTEM_PASSWORD_ENABLE 		0 
#endif 

// <o> Password value 
#ifndef SYSTEM_PASSWORD_VALUE
#define SYSTEM_PASSWORD_VALUE		123456
#endif 

// </e> !Password 

// <h> Switches 

// <q> LEDS switch 
#ifndef SYSTEM_LEDS_ENABLE
#define SYSTEM_LEDS_ENABLE	1 
#endif 

// <q> Motor Switch 
#ifndef SYSTEM_MOTOR_ENABLE 
#define SYSTEM_MOTOR_ENABLE 1 
#endif 

// <q> Buzzer Switch 
#ifndef SYSTEM_BUZZER_ENABLE 
#define SYSTEM_BUZZER_ENABLE 1 
#endif 

// <q> AGPS Switch 
#ifndef SYSTEM_AGPS_ENABLE 
#define SYSTEM_AGPS_ENABLE	0 
#endif 

// </h> !Switches 

// <h> Volume 

// <o> ringtone 
// <0-100>
#ifndef SYSTEM_VOLUME_RINGTONE_LEVEL 
#define SYSTEM_VOLUME_RINGTONE_LEVEL	100 
#endif 

// <o> mic 
// <0-15>
#ifndef SYSTEM_VOLUME_MIC_LEVEL
#define SYSTEM_VOLUME_MIC_LEVEL		15 
#endif 

// <o> Speaker 
// <0-100> 
#ifndef SYSTEM_VOLUME_SPEAKER_LEVEL		
#define SYSTEM_VOLUME_SPEAKER_LEVEL 100 
#endif 

// </h> !Volume 

// <e> Device Name 
#ifndef SYSTEM_DEV_NAME_ENABLE
#define SYSTEM_DEV_NAME_ENABLE	0 
#endif 

// <s.15> name string 
#ifndef SYSTEM_DEV_NAME_STRING 
#define SYSTEM_DEV_NAME_STRING		"eview"
#endif 

// </e> !Device Name 


// </h> !System Configuration 

// <h> Buttons Configuration 

// <e> Sensitivity 

// <o> Click threshold (unit: ms) 
// <5-30>
// <#/10>
// <i> valid : 5-30 (50ms-300ms)
#ifndef BUTTONS_CLICK_THRESHOLD		
#define BUTTONS_CLICK_THRESHOLD		5  
#endif 

// <o> Double Click Threshold (unit: ms ) 
// <#/10>
#ifndef BUTTONS_DOUBLE_CLICK_THRESHOLD	
#define BUTTONS_DOUBLE_CLICK_THRESHOLD	100 
#endif 

// <o> Short-Hold Threshold (unit: ms) 
// <50-200>
// <#/10> 
// <i> Short-Hold event threshold 
#ifndef BUTTONS_SHORT_HOLD_THRESHOLD
#define BUTTONS_SHORT_HOLD_THRESHOLD	100 
#endif 

// <o> Long-Hold threshold (unit: ms )
// <#/10>
#ifndef BUTTONS_LONG_HOLD_THRESHOLD
#define BUTTONS_LONG_HOLD_THRESHOLD		300
#endif 

#if BUTTONS_SHORT_HOLD_THRESHOLD >= BUTTONS_LONG_HOLD_THRESHOLD
#error "buttons short-hold threshold > long-hold threshold"
#endif 
#if BUTTONS_CLICK_THRESHOLD >= BUTTONS_SHORT_HOLD_THRESHOLD
#error "buttons click threshold > long-hold threshold"
#endif 

// </e> !Sensitivity 

// <e> Buttons actions 
#define BUTTONS_ACTIONS		1 
// <i> factory not initialized these action

// <h> SOS Button
// <o> Click Event 
// <0=> Idle 
// <1=> SOS Alert 
// <0x10=>Dial Auth-Contact 0 
// <0x11=>Dial Auth-Contact 1
// <0x12=>Dial Auth-Contact 2
// <0x13=>Dial Auth-Contact 3
// <0x14=>Dial Auth-Contact 4
// <0x15=>Dial Auth-Contact 5
// <0x16=>Dial Auth-Contact 6
// <0x17=>Dial Auth-Contact 7
// <0x18=>Dial Auth-Contact 8
// <0x19=>Dial Auth-Contact 9
// <0xFE=> Power Off 

#ifndef SOS_CLICK_EVENT
#define SOS_CLICK_EVENT		0 
#endif 

// <o> Double-Click Event 
// <0=> Idle 
// <1=> SOS Alert 
// <0x10=>Dial Auth-Contact 0 
// <0x11=>Dial Auth-Contact 1
// <0x12=>Dial Auth-Contact 2
// <0x13=>Dial Auth-Contact 3
// <0x14=>Dial Auth-Contact 4
// <0x15=>Dial Auth-Contact 5
// <0x16=>Dial Auth-Contact 6
// <0x17=>Dial Auth-Contact 7
// <0x18=>Dial Auth-Contact 8
// <0x19=>Dial Auth-Contact 9
// <0xFE=> Power Off 
#ifndef SOS_DOUBLE_CLICK_EVENT
#define SOS_DOUBLE_CLICK_EVENT	1
#endif 

// <o> Short-Hold Event 
// <0=> Idle 
// <1=> SOS Alert 
// <0x10=>Dial Auth-Contact 0 
// <0x11=>Dial Auth-Contact 1
// <0x12=>Dial Auth-Contact 2
// <0x13=>Dial Auth-Contact 3
// <0x14=>Dial Auth-Contact 4
// <0x15=>Dial Auth-Contact 5
// <0x16=>Dial Auth-Contact 6
// <0x17=>Dial Auth-Contact 7
// <0x18=>Dial Auth-Contact 8
// <0x19=>Dial Auth-Contact 9
// <0xFE=> Power Off 
#ifndef SOS_SHORT_HOLD_EVENT	
#define SOS_SHORT_HOLD_EVENT	0 
#endif 

// <o> Long-Hold Event 
// <0=> Idle 
// <1=> SOS Alert 
// <0x10=>Dial Auth-Contact 0 
// <0x11=>Dial Auth-Contact 1
// <0x12=>Dial Auth-Contact 2
// <0x13=>Dial Auth-Contact 3
// <0x14=>Dial Auth-Contact 4
// <0x15=>Dial Auth-Contact 5
// <0x16=>Dial Auth-Contact 6
// <0x17=>Dial Auth-Contact 7
// <0x18=>Dial Auth-Contact 8
// <0x19=>Dial Auth-Contact 9
// <0xFE=> Power Off 
#ifndef SOS_LONG_HOLD_EVENT
#define SOS_LONG_HOLD_EVENT		0 
#endif 

// </h> !SOS Button Event 

// <h> Side-L Button 
// <o> Click Event 
// <0=> Idle 
// <1=> SOS Alert 
// <0x10=>Dial Auth-Contact 0 
// <0x11=>Dial Auth-Contact 1
// <0x12=>Dial Auth-Contact 2
// <0x13=>Dial Auth-Contact 3
// <0x14=>Dial Auth-Contact 4
// <0x15=>Dial Auth-Contact 5
// <0x16=>Dial Auth-Contact 6
// <0x17=>Dial Auth-Contact 7
// <0x18=>Dial Auth-Contact 8
// <0x19=>Dial Auth-Contact 9
// <0xFE=> Power Off 
#ifndef SIDE_L_CLICK_EVENT 
#define SIDE_L_CLICK_EVENT		0 
#endif 

// <o> Double-Click Event 
// <0=> Idle 
// <1=> SOS Alert 
// <0x10=>Dial Auth-Contact 0 
// <0x11=>Dial Auth-Contact 1
// <0x12=>Dial Auth-Contact 2
// <0x13=>Dial Auth-Contact 3
// <0x14=>Dial Auth-Contact 4
// <0x15=>Dial Auth-Contact 5
// <0x16=>Dial Auth-Contact 6
// <0x17=>Dial Auth-Contact 7
// <0x18=>Dial Auth-Contact 8
// <0x19=>Dial Auth-Contact 9
// <0xFE=> Power Off 
#ifndef SIDE_L_DOUBLE_CLICK_EVENT
#define SIDE_L_DOUBLE_CLICK_EVENT 	0 
#endif 

// <o> Short-hold Event 
// <0=> Idle 
// <1=> SOS Alert 
// <0x10=>Dial Auth-Contact 0 
// <0x11=>Dial Auth-Contact 1
// <0x12=>Dial Auth-Contact 2
// <0x13=>Dial Auth-Contact 3
// <0x14=>Dial Auth-Contact 4
// <0x15=>Dial Auth-Contact 5
// <0x16=>Dial Auth-Contact 6
// <0x17=>Dial Auth-Contact 7
// <0x18=>Dial Auth-Contact 8
// <0x19=>Dial Auth-Contact 9
// <0xFE=> Power Off 
#ifndef SIDE_L_SHORT_HOLD_EVENT
#define SIDE_L_SHORT_HOLD_EVENT  0 
#endif 

// <o> Long-hold Event 
// <0=> Idle 
// <1=> SOS Alert 
// <0x10=>Dial Auth-Contact 0 
// <0x11=>Dial Auth-Contact 1
// <0x12=>Dial Auth-Contact 2
// <0x13=>Dial Auth-Contact 3
// <0x14=>Dial Auth-Contact 4
// <0x15=>Dial Auth-Contact 5
// <0x16=>Dial Auth-Contact 6
// <0x17=>Dial Auth-Contact 7
// <0x18=>Dial Auth-Contact 8
// <0x19=>Dial Auth-Contact 9
// <0xFE=> Power Off 
#ifndef SIDE_L_LONG_HOLD_EVENT 
#define SIDE_L_LONG_HOLD_EVENT	0 
#endif 

// </h> !Side-L button Event 

//#if (BUTTONS_ACTIONS  > 0 )
//#error "buttons action need to defined yourself"
//#endif 

// </e> !Buttons Actions 

// </h> !Buttons Configuration 

// <h> GPRS Configuration 

// <e> GPRS Apn Enable 
#ifndef GPRS_APN_ENABLE
#define GPRS_APN_ENABLE		0 
#endif 

// <s.31> APN string 
#ifndef GPRS_APN_NAME_STRING
#define GPRS_APN_NAME_STRING	""
#endif 

// <s.16> APN Username String 
#ifndef GPRS_APN_USER_NAME_STRING
#define GPRS_APN_USER_NAME_STRING	""
#endif 

// <s.16> APN Passowrd String 
#ifndef GPRS_APN_PASSWORD_STRING
#define GPRS_APN_PASSWORD_STRING	""
#endif 

// </e> !GPRS Apn Enable 

// <h> GPRS Server Settings 

// <s.31> IP or domain name 
#ifndef GPRS_SERVER_IP_STRING	
#define GPRS_SERVER_IP_STRING	"119.123.126.148" 
#endif 

// <o> port ( 0-65535 )
// <0-65535> 
#ifndef GPRS_SERVER_PORT
#define GPRS_SERVER_PORT		5050
#endif 

// </h> !GPRS Server Settings 

// <h> GPRS Time Setting 

// <o> Heart beat interval (unit:minutes) 
// <#*60>
#ifndef GPRS_HEARTBEAT_INTERVAL
#define GPRS_HEARTBEAT_INTERVAL		300 
#endif 

// <o> Upload interval (unit: minute )
// <#*60>
#ifndef GPRS_UPLOAD_INTERVAL
#define GPRS_UPLOAD_INTERVAL		1800
#endif 

// <o> Lazy upload interval( unit: minute )
// <#*60> 
#ifndef GPRS_LAZY_UPLOAD_INTERVAL
#define GPRS_LAZY_UPLOAD_INTERVAL	7200
#endif 

// </h> !GPRS Time Setting 


// </h> !GPRS Configuration 

// <h> Phone Relate Configuration 

// <h> Authorized Contact setting 
// <e> 1st authorized contact
#ifndef AUTH_CONTACT_1ST_ENABLE
#define AUTH_CONTACT_1ST_ENABLE 1
#endif 

// <q> accect sms alert
#ifndef AUTH_CONTACT_1ST_ACCEPT_SMS
#define AUTH_CONTACT_1ST_ACCEPT_SMS	0 
#endif 

// <q> accept phone alert 
#ifndef AUTH_CONTACT_1ST_ACCEPT_PHONE
#define AUTH_CONTACT_1ST_ACCEPT_PHONE	1 
#endif 

// <q> support no-simcard phone call 
#ifndef AUTH_CONTACT_1ST_NO_CARD
#define AUTH_CONTACT_1ST_NO_CARD	0 
#endif 

// <s.19> phone number 
#ifndef AUTH_CONTACT_1ST_PHONE_NUMBER
#define AUTH_CONTACT_1ST_PHONE_NUMBER	"13590257197"
#endif 

// </e> !1st authorized contact 

// <e> 2nd authorized contact
#ifndef AUTH_CONTACT_2ND_ENABLE
#define AUTH_CONTACT_2ND_ENABLE 1
#endif 

// <q> accect sms alert
#ifndef AUTH_CONTACT_2ND_ACCEPT_SMS
#define AUTH_CONTACT_2ND_ACCEPT_SMS	0 
#endif 

// <q> accept phone alert 
#ifndef AUTH_CONTACT_2ND_ACCEPT_PHONE
#define AUTH_CONTACT_2ND_ACCEPT_PHONE	1 
#endif 

// <q> support no-simcard phone call 
#ifndef AUTH_CONTACT_2ND_NO_CARD
#define AUTH_CONTACT_2ND_NO_CARD	1 
#endif 

// <s.19> phone number 
#ifndef AUTH_CONTACT_2ND_PHONE_NUMBER
#define AUTH_CONTACT_2ND_PHONE_NUMBER	"114"
#endif 

// </e> !2nd authorized contact 

// </h> !Authorized Contact setting

// <e> Sms Response prefix text 
#ifndef SMS_PREFIX_TEXT_ENABLE
#define SMS_PREFIX_TEXT_ENABLE	0 
#endif 

// <s.19> SMS Prefix Text 
#ifndef SMS_PREFIX_TEXT_STRING
#define SMS_PREFIX_TEXT_STRING	""
#endif 

// </e> !Sms Response prefix text

// <h> SOS Call Time 

// <o> Holding Time Allow (unit:second)
// <120-86400>
#ifndef SOS_HOLDING_TIME_ALLOW	
#define SOS_HOLDING_TIME_ALLOW 	3600 
#endif 

// <o> SOS_RINGTIME_MAX (unit: second) 
// <5-120> 
#ifndef SOS_RING_TIME_ALLOW	
#define SOS_RING_TIME_ALLOW		30
#endif 

// <o> SOS CALL LOOPS (unit: times ) 
// <0-10>
#ifndef SOS_CALL_LOOP_ALLOW	
#define SOS_CALL_LOOP_ALLOW		5 
#endif 

// </h> !SOS Call Time 

// <h> Switches 

// <e> Auto Answer incoming call 
#ifndef AUTO_ANWER_INCOMING_CALL_ENABLE 
#define AUTO_ANWER_INCOMING_CALL_ENABLE	1 
#endif 

// <o> Auto Answer Call After ring's count 
#ifndef AUTO_ANSWER_RING_COUNTS	
#define AUTO_ANSWER_RING_COUNTS		5 
#endif 

// </e> !Auto Answer incoming call 

// <q> Intercept phone call exclude authorized number 
#ifndef INTERCEPT_PHONE_CALL_ENABLE
#define INTERCEPT_PHONE_CALL_ENABLE 	0 
#endif 

// <q> Hangup phone call manually enable 
#ifndef HANGUP_PHONE_CALL_ENABLE 
#define HANGUP_PHONE_CALL_ENABLE		0 
#endif 

// <q> Monitor Environment Enable 
#ifndef MONITOR_ENVIRONMENT_ENABLE 
#define MONITOR_ENVIRONMENT_ENABLE		0 
#endif 

// <q> SMS command only by authorized number 
#ifndef SMS_COMMAND_ONLY_AUTH_CONTACT
#define SMS_COMMAND_ONLY_AUTH_CONTACT 	0 
#endif 

// </h> !Switches 

// </h> !Phone Releate Configuration 

// <h> Alert Configuration 

// <h> Power Alert 

// <e> Power Low Alert Enable 
#ifndef POWER_LOW_ALERT_ENABLE
#define POWER_LOW_ALERT_ENABLE	1 
#endif 

// <o> Power Low Threshold (perent 0-50%) 
// <0-50> 
#ifndef POWER_LOW_ALERT_THRESHOLD 
#define POWER_LOW_ALERT_THRESHOLD	30 
#endif 
// </e> Power Low Alert Enable 

// <q> Power On Alert Enable 
#ifndef POWER_ON_ALERT_ENABLE
#define POWER_ON_ALERT_ENABLE	0 
#endif 

// <q> Power Off Alert Enable 
#ifndef POWER_OFF_ALERT_ENABLE 
#define POWER_OFF_ALERT_ENABLE 	0 
#endif 

// </h> !Power Alert


// <h> GEO Fence Settings 
// <e> GEO Fence No.0 enable 
// <i> only support radius fence here 
#ifndef GEO_FENCE_NO_0_ALERT_ENABLE 
#define GEO_FENCE_NO_0_ALERT_ENABLE	1 
#endif 

// <q> dir select as inside 
#ifndef GEO_FENCE_NO_0_ALERT_DIR	
#define GEO_FENCE_NO_0_ALERT_DIR	1 
#endif 

// <q> geo type as polygon 
// <i> Only support polygon here, because not define list of points array here 
#ifndef GEO_FENCE_NO_0_ALERT_TYPE	
#define GEO_FENCE_NO_0_ALERT_TYPE	0 
#endif 

// <o> radius (unit: meters )
#ifndef GEO_FENCE_NO_0_RADIUS
#define GEO_FENCE_NO_0_RADIUS		1000
#endif 

// <o> geo points count( 0 - 8 )
// <0-8> 
// <i> in circle mode, it needs 1 point( longitude & latitude ) at least 
// <i> in polygon mode, it needs 3 points at least 
#ifndef GEO_FENCE_NO_0_ALERT_POINT_COUNT
#define GEO_FENCE_NO_0_ALERT_POINT_COUNT 1 
#endif 

// <h> Longitude & Latitude
// <i> <22644816, 114022769> = <22.644816, 114.022769> in fact
// <q> West longitude 
#ifndef GEO_FENCE_NO_0_LONG_WEST
#define GEO_FENCE_NO_0_LONG_WEST	0 
#endif 

// <o> Longitude  (-180 ~ +180)
// <i> format = ddd.mmmmmmm
#ifndef GEO_FENCE_NO_0_ALERT_LONGITUDE_0
#define GEO_FENCE_NO_0_ALERT_LONGITUDE_0	1140213250
#endif 

// <q> South Latitude 
#ifndef GEO_FENCE_NO_0_LAT_SOUTH
#define GEO_FENCE_NO_0_LAT_SOUTH	0 
#endif 

// <o> Latitude  (-90 ~ +90)
// <i> format dd.mmmmmmm
#ifndef GEO_FENCE_NO_0_ALERT_LATITUDE_0 
#define GEO_FENCE_NO_0_ALERT_LATITUDE_0		226446776
#endif 

#ifndef GEO_FENCE_NO_0_LONGITUDE_0
	#if GEO_FENCE_NO_0_LONG_WEST > 0 
	#define GEO_FENCE_NO_0_LONGITUDE_0	(GEO_FENCE_NO_0_ALERT_LONGITUDE_0 * (-1))
	#else
	#define GEO_FENCE_NO_0_LONGITUDE_0	GEO_FENCE_NO_0_ALERT_LONGITUDE_0
	#endif 
#endif 

#ifndef GEO_FENCE_NO_0_LATITUDE_0 
	#if GEO_FENCE_NO_0_LAT_SOUTH > 0 
	#define GEO_FENCE_NO_0_LATITUDE_0 (GEO_FENCE_NO_0_ALERT_LATITUDE_0 *(-1)) 
	#else 
	#define GEO_FENCE_NO_0_LATITUDE_0	GEO_FENCE_NO_0_ALERT_LATITUDE_0
	#endif 
#endif 

// </h> !Longitude & Latitude No.0 

// </e> !GEO Fence No.0 enable 

// </h> GEO Fence Settings 

// <e> Motion Alert Enable  
#ifndef MOTION_ALERT_ENABLE
#define MOTION_ALERT_ENABLE	1 
#endif 

// <o> Setup time (unit: minutes ) 
// <#*60> 
#ifndef MOTION_ALERT_SETUP_TIME
#define MOTION_ALERT_SETUP_TIME 	60 
#endif 

// <o> Action Time (unit: minutes) 
// <#*60>
#ifndef MOTION_ALERT_ACTION_TIME
#define MOTION_ALERT_ACTION_TIME	60 
#endif 

// </e> !Motion Alert Enable 

// <e> Actionless Alert Enable  
#ifndef ACTIONLESS_ALERT_ENABLE 
#define ACTIONLESS_ALERT_ENABLE		0 
#endif 

// <o> Threshold (unit: minutes ) 
// <#*60> 
#ifndef ACTIONLESS_ALERT_THRESHOLD	
#define ACTIONLESS_ALERT_THRESHOLD	120 
#endif 

// </e> !Actionless Alert Enable 

// <e> Speed Alert Enable 
#ifndef SPEED_ALERT_ENABLE 
#define SPEED_ALERT_ENABLE	1 
#endif 

// <o> Threshold ( unit: KM/h )
// <#*1000> 
#ifndef SPEED_ALERT_THRESHOLD
#define SPEED_ALERT_THRESHOLD	12000
#endif 

// </e> Speed Alert Enable 

// <e> Tilt Alert Enable 
#ifndef TILT_ALERT_ENABLE 
#define TILT_ALERT_ENABLE 	1 
#endif 

// <o> ANGLE_THRESHOLD (unit: degree ) 
#ifndef TILT_ALERT_ANGLE_THRESHOLD
#define TILT_ALERT_ANGLE_THRESHOLD		40 
#endif 

// <o> Action Time Threshold( unit: minutes ) 
#ifndef TILT_ALERT_TIME_THRESHOLD	
#define TILT_ALERT_TIME_THRESHOLD		20 
#endif 

// </e> !Tilt Alert Enable 

// <e> Falldown alert enable 
#ifndef FALLDOWN_ALERT_ENABLE 
#define FALLDOWN_ALERT_ENABLE 	1 
#endif 

// <o> Mode 
// <0=> Disable 
// <1=> Enable
// <2=> Enable, active call alert 
#ifndef FALLDOWN_ALERT_MODE		
#define FALLDOWN_ALERT_MODE		1//0 
#endif 

// <o> Level ( 1 - 9 ) 
// <1-9> 
#ifndef FALLDOWN_ALERT_LEVEL
#define FALLDOWN_ALERT_LEVEL	9
#endif 

// <o> Time Threshold(unit: seconds )
#ifndef FALLDOWN_ALERT_TIME
#define FALLDOWN_ALERT_TIME		120 
#endif 

// </e> !Falldown Alert enable 


// </h> Alert Configuration 

// <<< end of configuration section >>>

#endif 
