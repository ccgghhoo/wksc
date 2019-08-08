#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nrf_gpio.h"
#include "key.h"
#include "nrf_drv_gpiote.h"
#include "br_evt.h"
#include "br_app_timer.h"
#include "nrf_delay.h"
#include "ble_nus.h"
#include "ble_advertising.h"
#include "br_hr.h"
#include  "i2c_lcd.h"
#include "br_disp_page.h"
#include "SEGGER_RTT.h" 
//#include "ZIKU_64X48.h"
#include "boards.h"
#include "kx022.h"
#include "alarm_proc.h"
#include "m_rtc.h"
enum{
	KEY_PUSH_DOWN=1,
	KEY_PUSH_UP =2
};
unsigned short kx022_wake_num=0;
volatile unsigned char  Key_Push_status=0xFF;//按键状态
volatile unsigned char  Old_Key_status=0xFF;//上一次按键状态
volatile unsigned char key_long_push_flag=0;//长按按键是否被按下
static uint32_t old_system_second=0xFFFFFFFF;

void in_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
	
	uint32_t time_second=0;
	time_second=osal_getClock();
	if(pin==KEY_PIN)
	{
		if(action==NRF_GPIO_PIN_SENSE_LOW)//按键按下
		{
			Old_Key_status=Key_Push_status;
			Key_Push_status=KEY_PUSH_DOWN;
			old_system_second=time_second;
		}
		else 
		{
			Old_Key_status=Key_Push_status;
			Key_Push_status=KEY_PUSH_UP;
			if((time_second>=old_system_second)&&(time_second-old_system_second>=2)&&(old_system_second!=0xFFFFFFFF))
			{
                ;//do nothing
			}
			else
			{
				key_long_push_flag++;
				set_evt(M_EVT_KEY_SHORT);        
        #ifdef TEST_RTT  
            SEGGER_RTT_printf(0,"KEY_SHORT-");
        #endif
			}
			old_system_second=0xFFFFFFFF;
		}
	}
	if(pin==KX022_INT2_PIN)
	{
		if(nrf_gpio_pin_read(KX022_INT2_PIN))
		{
			kx022_wake_num++;
			set_evt(M_EVT_KX022_INT);
		}
	}
	
	if(pin==CHRG_5V_DETECT_PIN)
	{
		set_evt(M_EVT_CHARG);
	}
}

void key_init(void)
{
	nrf_drv_gpiote_init();	
	nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(false);
	in_config.pull = NRF_GPIO_PIN_PULLUP;
	nrf_drv_gpiote_in_init(KEY_PIN, &in_config, in_pin_handler);  	
	nrf_drv_gpiote_in_event_enable(KEY_PIN, true);

	nrf_drv_gpiote_in_config_t in_config2 = GPIOTE_CONFIG_IN_SENSE_TOGGLE(false);
	in_config.pull = NRF_GPIO_PIN_NOPULL;
	nrf_drv_gpiote_in_init(KX022_INT2_PIN, &in_config2, in_pin_handler);   
	//nrf_drv_gpiote_in_event_disable(KX022_INT2_PIN);
	nrf_drv_gpiote_in_event_enable(KX022_INT2_PIN, true);
	
	nrf_drv_gpiote_in_config_t in_charg3 = GPIOTE_CONFIG_IN_SENSE_TOGGLE(false);
	in_config.pull = GPIO_PIN_CNF_PULL_Disabled;
	nrf_drv_gpiote_in_init(CHRG_5V_DETECT_PIN, &in_charg3, in_pin_handler);   //detect
	nrf_drv_gpiote_in_event_enable(CHRG_5V_DETECT_PIN, true);
}

//扫描按键按下了多久 区分出长按
void key_second_scan(void)
{
	uint32_t time_second=0;
	time_second=osal_getClock();
	//SEGGER_RTT_printf(0,"KEY_LONG333 \r\n");
    if(Key_Push_status==KEY_PUSH_DOWN)
    {
			//SEGGER_RTT_printf(0,"KEY_LONG222 \r\n");
        if((time_second>=old_system_second)&&(time_second-old_system_second>=2)&&(old_system_second!=0xFFFFFFFF))
        {
					//SEGGER_RTT_printf(0,"KEY_LONG111 \r\n");
            if(Old_Key_status!=KEY_PUSH_DOWN)//防止多次判断为长按
            {
                set_evt(M_EVT_KEY_LONG);
								//SEGGER_RTT_printf(0,"KEY_LONG! \r\n");
            }
            Old_Key_status=KEY_PUSH_DOWN;
        }
    }
}



