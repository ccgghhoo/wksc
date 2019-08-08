

#include <stdlib.h>
#include <stdbool.h>


#include "app_util_platform.h"
#include "app_heap.h"

#if 0 
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#define HM_LOG				NRF_LOG_INFO
#define HM_ERROR			NRF_LOG_ERROR

#else
#define HM_LOG(...)
#define HM_ERROR(...)

#endif 

static uint32_t alloc_count = 0; 

uint32_t app_alloc_in_used_num(void)
{
	return alloc_count; 
}


uint8_t * app_malloc(uint16_t size)
{
	uint8_t * tmp; 
	
	CRITICAL_REGION_ENTER();
	tmp = (uint8_t*)malloc( size ); 
	CRITICAL_REGION_EXIT();
	
	if( tmp != NULL ){
		alloc_count ++; 
		HM_LOG("[HEAP]: alloc start = 0x%X, count = %d\r\n", (uint32_t)tmp, alloc_count ); 		
	}
	else{
		HM_LOG("[HEAP]: fail !!!!\r\n"); 
	}
		
	return tmp; 
}

uint8_t * app_malloc_with_label(uint32_t label, uint16_t size )
{
	uint8_t * tmp; 
	
	CRITICAL_REGION_ENTER();
	tmp = (uint8_t*)malloc( size ); 
	CRITICAL_REGION_EXIT();
	
	if( tmp != NULL ){
		alloc_count ++; 
		HM_LOG("[HEAP]: label = %d, start = 0x%X, count = %d\r\n", label, (uint32_t)tmp, alloc_count ); 		
	}
	else{
		HM_LOG("[HEAP]: label = %d, fail !!!!\r\n", label); 
	}
		
	return tmp; 
}


void app_safe_free( void * targ ) // safe free 
{
	if( targ != NULL) {
		alloc_count --; 
		HM_LOG("[HEAP]: free at 0x%X, remain = %d \r\n", (uint32_t)targ, alloc_count); 	
		CRITICAL_REGION_ENTER();
		free( targ ); 			
		CRITICAL_REGION_EXIT();
//		targ = (void*)NULL; 				
	}
	else{
		HM_LOG("[HEAP]: targ is NULL!!!!\r\n"); 
	}
}




