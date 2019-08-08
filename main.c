/**
 * Copyright (c) 2015 - 2018, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include "nrf_drv_spi.h"
#include "app_util_platform.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "boards.h"
#include "app_error.h"
#include <string.h>
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "LIS3DH_drv.h"
#include "app_timer.h"
#include "nrf_drv_clock.h"
#include "app_i2c_slave.h"
#include "bsp.h"
#include "led_pwm_ctrl.h"
#include "batt_adc_detect.h"
#include "app_utc_time.h"
#include "app_motion_detect.h"
#include "batt_adc_detect.h"
#include "dfu_req_handler.h"
#include "app_evt.h"
#include "app_wdt.h"
#include "app_power_off.h"


static void bsp_event_handler(bsp_event_t event)
{    
    switch (event)
    {
    case BSP_EVENT_KEY_0: // INT_ICM1 --WATER MARK
    {
 
    }
        break;

    case BSP_EVENT_KEY_1: // INT_ICM2 -- WAKE UP
    {                                
        NRF_LOG_INFO("INT_ICM2! gsensor is active");    
        md_set_wakeup();    

        break;        
    }

    default:
       NRF_LOG_INFO("UNKOWN BUTTON!!!"); 
        break;
    }
}

static void buttons_leds_init(void)
{
    uint32_t err_code = bsp_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS, bsp_event_handler);
    APP_ERROR_CHECK(err_code);
}



static void main_loop(void)
{        
//-----------------------------------------------------------    
    if(check_app_evt(APP_EVT_PROTOCOL_DATA_HANDLE))
    {
        clear_app_evt(APP_EVT_PROTOCOL_DATA_HANDLE);
        app_i2c_protocol_data_process();
    }
    
//   
    if(check_app_evt(APP_EVT_1S))
    {
        //NRF_LOG_INFO("0 ");
        clear_app_evt(APP_EVT_1S);
        if(m_user_time_senconds%60==0)
        {
            battery_level_cal();
            batt_voltage_get();
            //md_motion_int_en(EINT2_MOTION_ALERT_PIN);    //for test 07/24
        }   
//         AxesRaw_t raw_data;     //2019/08/02 for test
//         LIS3DH_GetAccAxesRaw(&raw_data);      
//         NRF_LOG_INFO("x=%d y=%d z=%d", raw_data.X/16, raw_data.Y/16, raw_data.Z/16);
//        
        
        md_motion_or_static_alert_judge();
        //power_off_pin_check_when_reset(); 
        
        if(check_app_evt(APP_EVT_DFU_RESET))
        {
            static uint32_t  dfu_reset_cnt=0;
            if(dfu_reset_cnt++>5)
            {
                dfu_reset_cnt=0;
                clear_app_evt(APP_EVT_DFU_RESET);
                dfu_complete_free_alloc_mem();
                NRF_LOG_INFO("DFU system reset !!");
                NVIC_SystemReset();
            }
        }        
                   
    }
 
//---------------------------------------------    
        
    md_process();
    
    dfu_process();

//---------------------------------------------- 
    app_wdt_reload(); 
    
   if(check_app_evt(0xffffffff)==0 || !md_get_wakeup())
   {     
     __WFE();
   }
    
      
}




static void lfclk_config(void)
{
    ret_code_t err_code;    
    err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);
    nrf_drv_clock_lfclk_request(NULL);   
}

 

int main(void)
{
    
    lfclk_config();    
//       
    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_DEFAULT_BACKENDS_INIT();
    
    APP_ERROR_CHECK(app_timer_init());
     
    app_wdt_reload();   
    led_rgb_timer_init();
    buttons_leds_init();
//	
	UTC_timer_init();
    
//    nrf_gpio_cfg_input(25, NRF_GPIO_PIN_NOPULL); //for test I2C 
//    nrf_gpio_cfg_output(6); //for test I2C 
//    nrf_gpio_pin_clear(6);       
//    nrf_gpio_pin_set(8);

//    app_dfu_flash_init();
//    dfu_setting_flash_init();
    
    LIS3DH_SpiInit();
    md_init();
    APP_ERROR_CHECK(app_i2c_slave_init());
    bsp_pwm_init();
    batt_adc_init();
    power_off_pin_config();
    
    led_mode_set(0 ,  0);  
    led_mode_set(1 ,  0); 
    led_mode_set(2 ,  0);
    led_mode_set(3 ,  0xcc);
    
    power_off_pin_check_when_reset(); 

       
    while (1)
    {
                    
        NRF_LOG_FLUSH();    
//        nrf_delay_ms(10000);        
        main_loop();
//        __WFE();

    }
}
