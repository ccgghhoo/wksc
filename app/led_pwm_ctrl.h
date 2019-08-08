
/******************************************************************************

*******************************************************************************/
#ifndef __PWM_CTRL__H
#define __PWM_CTRL__H

#include <stdbool.h>
#include "sdk_errors.h"
typedef struct 
 {
    uint8_t led_cycle_flag;
    uint8_t led_time_set;
    uint8_t led_time_cnt;
    uint8_t led_db_item_index; 
    uint8_t led_inc_mode;    
    const uint8_t *led_mode_db_set; 
}led_inc_mode_t;
 

extern led_inc_mode_t led_inc_mode_red, led_inc_mode_green, led_inc_mode_blue;



void bsp_pwm_init(void);
void led_pwm_breath_handle(void);
void stop_led_pwm0(void);
void start_led_pwm0(void);

void led_mode_set(uint8_t led_id ,  uint8_t mode);
void led_inc_mode_drv_process(void);
void led_rgb_timer_init(void);
void led_inc_all_off(void);

#endif /* */



