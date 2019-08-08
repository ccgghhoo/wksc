#ifndef _APP_EVT_H_
#define _APP_EVT_H_

#include <stdint.h>
#include <stdbool.h>

#define APP_EVT_1S                              0x0001
#define APP_EVT_PROTOCOL_DATA_HANDLE            0x0002
#define APP_EVT_DFU_RESET                       0x0004 



void  clear_app_evt(uint32_t evt);
void  set_app_evt(uint32_t evt);    
bool check_app_evt(uint32_t evt);



#endif

