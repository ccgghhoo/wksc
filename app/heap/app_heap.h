

#ifndef APP_HEAP_H__
#define APP_HEAP_H__


#include <stdint.h>
#include "malloc_label.h"

uint8_t * app_malloc_with_label(uint32_t label, uint16_t size ); 

uint32_t app_alloc_in_used_num(void); 

uint8_t * app_malloc(uint16_t size); 

void app_safe_free( void * targ );


#if 0
#define APP_MALLOC( label, size)		app_malloc( size ) 
#else
#define APP_MALLOC( label, size)		app_malloc_with_label( label, size )
#endif 


#define APP_FREE					app_safe_free 

#endif 

