

#ifndef HEAP_MANAGER_H__
#define HEAP_MANAGER_H__


#include <stdlib.h>
#include "nrf.h"

//#if 1 
//#include "nrf_log.h"
//#include "nrf_log_ctrl.h"
//#define HM_LOG				NRF_LOG_INFO
//#define HM_ERROR			NRF_LOG_ERROR

//#else
//#define HM_LOG(...)
//#define HM_ERROR(...)

//#endif 


//__STATIC_INLINE uint8_t * app_malloc(uint32_t size)
//{
//	uint8_t * point = (uint8_t*)malloc( size ); 
//#if 1
//	HM_LOG("[HEAP]: req malloc %d bytes\r\n", size);
//	if ( point != NULL){
//		HM_LOG("[HEAP]: location = 0x%X, end = 0x%X\r\n", (uint32_t)point, (uint32_t)(point + size)); 
//	}else{
//		HM_ERROR("[HEAP]: Fail!!!\r\n"); 
//	}
//#endif 	
//	
//	return point; 
//}

//__STATIC_INLINE void app_free(void * ptr)
//{	
//	if( ptr != NULL){
//		HM_LOG("[HEAP]: Free heap at location = 0x%X\r\n", (uint32_t)ptr); 
//		free( ptr ); 
//	}else{
//		HM_ERROR("[HEAP]: entry pointer is nulllllll!!\r\n"); 
//	}
//}


#endif // HEAP_MANAGER_H
