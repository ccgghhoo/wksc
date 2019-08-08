

#ifndef PROTO_PB_H__
#define PROTO_PB_H__


#define MAGIC_NUMBER			0xAB 
#define EPB_VER					0x00 

#define FLAG_ACK				0x10 
#define FLAG_ERROR				0x20
#define FLAG_VERSION			EPB_VER
#define FLAG_CRYPTO_NONE		0x00 

#define FLAG_NORMAL_RESP		(0x00 | FLAG_VERSION ) 
#define FLAG_ERROR_RESP			(0x00 | FLAG_ERROR | FLAG_VERSION ) 
#define FLAG_ACK_RESP			(0x00 | FLAG_ACK | FLAG_VERSION )

#define EPB_PAYLOAD_MAX_LEN		1024 	// 
#define EPB_HEAD_DATA_SIZE		8 
#define EPB_DATA_SIZE_MAX		(1024 + 8 + 8 )

#define EPB_HEART_BEAT_MAGIC_CODE	0x5A 

typedef enum{
	EPB_STEP_MAGIC  = 0, 
	EPB_STEP_FLAG, 
	EPB_STEP_LEN_LSB, 
	EPB_STEP_LEN_MSB, 
	EPB_STEP_CRC_LSB, 
	EPB_STEP_CRC_MSB, 
	EPB_STEP_TID_LSB, 
	EPB_STEP_TID_MSB, 
	EPB_STEP_PAYLOAD, 
	EPB_STEP_COMPLETED, 
}EPB_STEP_ENUM; 

typedef enum{
	EPB_SUCCESS = 0,
	
	EPB_ERR_INVALID_LENGTH	,
	EPB_ERR_INVALID_FORMAT, 
	EPB_ERR_DATA_SIZE_ERROR ,
	EPB_ERR_RESOURCE_INSUFFICIENT, 
	EPB_ERR_SUB_FUNCTION_NOT_SUPPORTED , 
	EPB_ERR_NO_MEM, 
	EPB_ERR_GPS_IS_NOT_READY, 
	EPB_ERR_ADDR_RESP, 
	EPB_ERR_INVALID_KEY, 
	EPB_ERR_DELAY_NOT_EXPIRED_TIME, 
	
	EPB_ERR_BAD_CHECKSUM, 	 
}epb_errcode_t; 


typedef enum{
	COMMAND_ID_DATA 		= 0x01, 
	COMMAND_ID_CONFIG		= 0x02, 
	COMMAND_ID_SERVICES		= 0x03, 
	COMMAND_ID_SYS_CONTROL	= 0x04, 	
	COMMAND_ID_MEMORY_READER = 0x05, 
	
	COMMAND_ID_FIRMWARE 	= 0x7E,
	COMMAND_ID_NEGATIVE 	= 0x7F, 	
	COMMAND_ID_FACOTRY_TEST = 0xFF, 
}epb_command_id_t; 

typedef enum{ //epb_key_of_data_t	
	DATA_KEY_TX_COMPLETED = 0x10,
	DATA_KEY_HISTORICAL_DATA = 0x11, 		
	DATA_KEY_GPS_LOC, 
	DATA_KEY_GSM_CALL_RECENTS, 
	DATA_KEY_GSM_LOC , 
	
	
    DATA_KEY_GPS_LATEST_LOC  = 0x30,      
	DATA_KEY_GENERAL_DATA, 	 	
    DATA_KEY_GENERAL_FLAG, 
    
    DATA_KEY_GPS_ALERT_NOTIFY = 0x40,
    DATA_KEY_GSM_ALERT_NOTIFY, 
}epb_key_of_data_t; 

typedef enum{ //epb_key_of_service_t

	SERV_KEY_KEEP_ALIVE 	= 0x10,
	SERV_KEY_ADDR_PARSE, 
	SERV_KEY_CONN_SERVER, 
    SERV_KEY_DISTANCE_ALERT, 
	
	SERV_KEY_FIND_ME		= 0x20, 
}epb_key_of_service_t; 

typedef enum{ //epb_key_of_sys_control_t
	
	SYS_KEY_RESET_RECORD	= 0x10, 
	SYS_KEY_FACTORY_RECOVERY, 
	SYS_KEY_DEVICE_RESET, 
	SYS_KEY_DEVICE_RESET_DIRECTLY, 
	SYS_KEY_ENTER_DFU, 
	SYS_KEY_POWER_OFF, 
	
}epb_key_of_sys_control_t; 

typedef enum{
	
	MEMO_KEY_INNER_RAM = 0x10,
	MEMO_KEY_EXTERNAL_FLASH, 
	MEMO_KEY_MEMS, 
}epb_key_of_memory_reader_t;

//typedef enum{ //epb_key_of_firmware_t

//	FIRM_KEY_REQ_UPDATE		= 0x10, 
//	FIRM_KEY_XFER_CONTENT, 
//	FIRM_KEY_VALIDATE, 
//	FIRM_KEY_QUERY, 
//	FIRM_KEY_RESET_DEVICE, 
//}epb_key_of_firmware_t; 

typedef enum{ //epb_key_of_factory_test_t
	
	FACTORY_KEY_BUTTON 		= 0x10, 
	FACTORY_KEY_LED, 
	FACTORY_KEY_MOTOR, 
	FACTORY_KEY_BUZZER, 
	FACTORY_KEY_SPEAKER, 
	
	FACTORY_KEY_GPS			= 0x20, 
	FACTORY_KEY_GSM, 
	FACTORY_KEY_ICM20648, 
	FACTORY_KEY_FLASH, 
	
	FACTORY_KEY_START_TEST, 
	FACTORY_KEY_STOP_TEST, 
}epb_key_of_factory_test_t; 




typedef struct{
	uint8_t 		version:	4; 
	uint8_t 		ack:		1; 
	uint8_t 		error:		1; 
	uint8_t 		encrypt:	2; 
}l1_flag_t;

typedef struct{
	uint8_t* 		data; 
	uint16_t 		len; 
}l2_payload_t; 

#if defined(__CC_ARM)
    #pragma push
    #pragma anon_unions
#elif defined(__ICCARM__)
    #pragma language=extended
#elif defined(__GNUC__)
    // anonymous unions are enabled by default
#endif

typedef struct{
	
	uint8_t  magic; 
	union{
		struct{
			uint8_t version:	4; 
			uint8_t ack:		1; 
			uint8_t error:		1; 
			uint8_t encrypt:	2; 
		}flag;
		uint8_t val;
	};
	uint16_t len;
	uint16_t crc; 
	uint16_t id; 
	
	uint8_t* payload; 
}msg_packet_t; 


#if defined(__CC_ARM)
    #pragma pop
#elif defined(__ICCARM__)
    // leave anonymous unions enabled
#elif defined(__GNUC__)
    // anonymous unions are enabled by default
#endif




#endif 
