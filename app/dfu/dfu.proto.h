

#ifndef DFU_PROTO_H__
#define DFU_PROTO_H__

#include <stdint.h>

typedef enum{ //epb_key_of_firmware_t

	FIRM_KEY_REQ_UPDATE		= 0x10, 
	FIRM_KEY_XFER_CONTENT, 
	FIRM_KEY_VALIDATE, 
	FIRM_KEY_QUERY, 
	FIRM_KEY_RESET_DEVICE, 
}epb_key_of_firmware_t; 


enum{
	DFU_INIT_COMMAND = 0, 
	DFU_WRITE_FIRMWARE, 
	DFU_EXECUTE_VERIFY, 
	DFU_COMPLETED, 
};

#endif 

