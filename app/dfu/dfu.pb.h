

#ifndef DFU_PB_H__
#define DFU_PB_H__

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    DFU_FW_TYPE_APPLICATION = 0,
    DFU_FW_TYPE_SOFTDEVICE = 1,
    DFU_FW_TYPE_BOOTLOADER = 2,
    DFU_FW_TYPE_SOFTDEVICE_BOOTLOADER = 3
} dfu_fw_type_t;
#define DFU_FW_TYPE_MIN DFU_FW_TYPE_APPLICATION
#define DFU_FW_TYPE_MAX DFU_FW_TYPE_SOFTDEVICE_BOOTLOADER
#define DFU_FW_TYPE_ARRAYSIZE ((dfu_fw_type_t)(DFU_FW_TYPE_SOFTDEVICE_BOOTLOADER+1))

typedef enum
{
    DFU_HASH_TYPE_NO_HASH = 0,
    DFU_HASH_TYPE_SHA256 = 1,
    DFU_HASH_TYPE_MD5 = 2, 
	DFU_HASH_TYPE_LAST, 
} dfu_hash_type_t;
#define DFU_HASH_TYPE_MIN DFU_HASH_TYPE_NO_HASH
#define DFU_HASH_TYPE_MAX DFU_HASH_TYPE_MD5
#define DFU_HASH_TYPE_ARRAYSIZE ((dfu_hash_type_t)(DFU_HASH_TYPE_LAST))

typedef enum
{
    DFU_SIGNATURE_TYPE_ECDSA_P256_SHA256 = 0,
	DFU_SIGNATURE_TYPE_ED25519 = 1, 
} dfu_signature_type_t;
#define DFU_SIGNATURE_TYPE_MIN DFU_SIGNATURE_TYPE_ECDSA_P256_SHA256
#define DFU_SIGNATURE_TYPE_MAX DFU_SIGNATURE_TYPE_ED25519
#define DFU_SIGNATURE_TYPE_ARRAYSIZE ((dfu_signature_type_t)(DFU_SIGNATURE_TYPE_ED25519+1))


/* Struct definitions */
//typedef PB_BYTES_ARRAY_T(32) dfu_hash_hash_t;
typedef struct{
	uint8_t 	size; 
	uint8_t 	bytes[32]; 
}dfu_hash_hash_t; 

typedef struct {
    dfu_hash_type_t hash_type;
    dfu_hash_hash_t hash;
/* @@protoc_insertion_point(struct:dfu_hash_t) */
} dfu_hash_t;

typedef struct{
	bool has_fw_version;
	uint32_t fw_version; 
	bool has_hw_version; 
	uint32_t hw_version; 
	uint8_t sd_req_count; 
	uint32_t sd_req[4]; 
	bool has_type; 
	dfu_fw_type_t type;
	
	bool has_sd_size; 
	uint32_t sd_size; 
	bool has_bl_size; 
	uint32_t bl_size; 
	bool has_app_size; 
	uint32_t app_size; 
	
	bool has_hash; 
	dfu_hash_t	hash; 	
	bool has_crc; 
	uint32_t 	crc32; 
}pb_init_command_t; 

typedef struct{
	uint8_t 	size; 
	uint8_t 	bytes[64]; 
}dfu_signed_command_signature_t;

typedef struct{	
	dfu_signature_type_t signature_type; 
	dfu_signed_command_signature_t signature;
}pb_signed_command_t; 


typedef struct{
	bool has_init; 
	pb_init_command_t init; 
	
	bool has_signed_command; 
	pb_signed_command_t sign_command; 
}pb_create_command_t; 

typedef enum{
	PB_INIT_SIGN = 1, 

	PB_INIT_HASH, 
	PB_INIT_CRC32, 
	PB_INIT_DFU_TYPE, 
	PB_INIT_APP_SIZE, 
	PB_INIT_SD_SIZE, 
	PB_INIT_BL_SIZE, 
	PB_INIT_FW_VER, 
	PB_INIT_HW_VER, 
	PB_INIT_SD_FWID, 	
}PB_INIT_TYPE; 

#define PB_SIGNED_COMMAND_SIZE		(40)	//32hash + 4type 
#define PB_CREATE_COMMAND_SIZE		(255)	//152  //chen
#define PB_WRITE_COMMAND_MIN_SIZE	(4 + 4 ) 
#define PB_WRITE_COMMAND_SIZE		(256 + 4)//offset + fw 


bool dfu_pb_init_decode(const uint8_t * p_data, uint16_t len, pb_create_command_t * req_cmd); 

#endif 

