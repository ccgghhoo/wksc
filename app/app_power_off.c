#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "nordic_common.h"
#include "nrf_gpio.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_soc.h"
#include "nrf_power.h"
#include "nrf_drv_uart.h"
#include "app_power_off.h"
#include "LIS3DH_drv.h"
#include "app_i2c_slave.h"
#include "led_pwm_ctrl.h"
#include "nrf_drv_gpiote.h"
#include "app_button.h"



//LOW LEVEL ---POWER OFF   HIGH LEVEL ---POWER ON

#if 1
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#define POWER_OFF_LOG				        NRF_LOG_INFO
#else
#define POWER_OFF_LOG(...)
#endif

static void power_off_wakeup_pin_config(uint32_t pin ,   uint8_t  en_level)
{
        nrf_gpio_pin_sense_t sense = en_level  ? NRF_GPIO_PIN_SENSE_HIGH : NRF_GPIO_PIN_SENSE_LOW;                        
        nrf_gpio_cfg_sense_set(pin, sense);       
}

 static void power_off_mode_pin_prepare(void)
 {
   //LED OFF,  spi uinit,  i2c uinit,  pwm disable, adc  disable , lis3dh  off       
    app_i2c_slave_uninit(); 
    LIS3DH_spi_uninit();    
 	led_inc_all_off();
    app_button_disable();     
    //CRITICAL_REGION_ENTER();  //
#if NRF_LOG_BACKEND_UART_ENABLED
   nrfx_uarte_t uarte = NRFX_UARTE_INSTANCE(0);         
   nrfx_uarte_uninit(&uarte);         
#endif  
         
     power_off_wakeup_pin_config(PWR_OFF_WAKE_UP_PIN ,  PWR_OFF_WAKE_UP_LEVEL);
 }	


static void power_off_mode_enter(void)
{
    // Prepare wakeup pin , disable all the hardwares 
    power_off_mode_pin_prepare();

    // Go to system-off mode (this function will not return; wakeup will cause a reset).
    nrf_power_system_off();
}


static void power_off_pin_evt_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    if(pin!=PWR_OFF_WAKE_UP_PIN || action!=NRF_GPIOTE_POLARITY_HITOLO) return;
    
    if (nrf_gpio_pin_read(pin) == 0)
    {
        nrf_delay_ms(20);
        if (nrf_gpio_pin_read(pin) == 0)
        {      
            power_off_mode_enter();
        }
    }
}

void power_off_pin_config(void)
{
    
        nrf_drv_gpiote_in_config_t int_config = {
        .sense 			= NRF_GPIOTE_POLARITY_HITOLO,
        .pull			= NRF_GPIO_PIN_PULLDOWN,
        .is_watcher 	= false,
        .hi_accuracy	= false,
    };


    APP_ERROR_CHECK(nrf_drv_gpiote_in_init(PWR_OFF_WAKE_UP_PIN,
                                           &int_config,
                                           power_off_pin_evt_handler)
                   );

    nrf_drv_gpiote_in_event_enable(PWR_OFF_WAKE_UP_PIN, true);
    
    POWER_OFF_LOG("reset system init wakeup pin"); 

    
}


void power_off_pin_check_when_reset(void)
{
    uint32_t reset_reason;
    reset_reason = nrf_power_resetreas_get();
    POWER_OFF_LOG("reset reason = %x ", reset_reason);
    
    if (nrf_gpio_pin_read(PWR_OFF_WAKE_UP_PIN) == 0)
    {
       POWER_OFF_LOG("PWR_OFF_WAKE_UP_PIN=0"); 
        nrf_delay_ms(20);
        if (nrf_gpio_pin_read(PWR_OFF_WAKE_UP_PIN) == 0)
        {            
            power_off_mode_enter();
        }        
    }
    else
    {
        POWER_OFF_LOG("PWR_OFF_WAKE_UP_PIN=1"); 
        nrf_delay_ms(20);
        if (nrf_gpio_pin_read(PWR_OFF_WAKE_UP_PIN) == 0)
        {            
            power_off_mode_enter();
        } 
    }
        
    
}



