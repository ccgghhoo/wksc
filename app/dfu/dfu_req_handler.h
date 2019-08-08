

#ifndef DFU_REQ_HANDLER_H__
#define DFU_REQ_HANDLER_H__

#include <stdint.h>
#include <stdbool.h>

#include "dfu.proto.h"
#include "dfu.pb.h"
#include "dfu.res.h"

typedef struct{
	uint8_t 	cmd; 
	uint8_t* 	p_data; 
	uint16_t 	size; 
}dfu_req_t; 

typedef struct{
	uint8_t*	p_data; 
	uint16_t 	size; 
    uint16_t    rsp_neg_flag;  //chen added 2019/07/03
}dfu_res_t; 

typedef struct{
	uint32_t crc32; 
	uint32_t size; 
	uint32_t write_offset; 
}dfu_req_init_t; 
	

void dfu_init(void); 

void dfu_process(void); 

bool dfu_req_decode( dfu_req_t * p_dfu_req, dfu_res_t * p_dfu_res ); 

dfu_req_init_t* get_firmware_info(void) ;

void dfu_complete_free_alloc_mem(void);

//uint8_t dfu_req_init( const uint8_t * p_data, uint16_t len );

//uint8_t dfu_req_write( const uint8_t * p_data, uint16_t len ); 

//uint8_t dfu_req_execute(void); 

//uint8_t dfu_req_query_state(uint8_t * state, uint8_t *size); 

//uint8_t dfu_req_reset_device(void);



#endif 

