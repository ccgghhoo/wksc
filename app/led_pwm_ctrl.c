/*
*
*
*/
#include "bsp.h"
#include "nrf_drv_pwm.h"
#include "nrf_drv_clock.h"
#include "nrfx_pwm.h"
#include "led_pwm_ctrl.h"
#include "app_timer.h"

#include "nrf_log.h"


//#define   RED_LED_PWM0            11
//#define   BLUE_LED_PWM0           12
//#define   GREEN_LED_PWM0          13

#define   LED_PWM_PERIOD          20000 //  *1us 
#define   USED_PWM(idx) (1UL << idx)

APP_TIMER_DEF(m_timer_100ms);

static nrf_drv_pwm_t m_pwm0 = NRF_DRV_PWM_INSTANCE(0);
uint8_t m_used = 0;
uint8_t led_pwm_inc_mode = 0;
static nrf_pwm_values_individual_t m_pwm0_seq_values;
static nrf_pwm_sequence_t const    m_pwm0_seq =
{
    .values.p_individual = &m_pwm0_seq_values,
    .length              = NRF_PWM_VALUES_LENGTH(m_pwm0_seq_values),
    .repeats             = 0,
    .end_delay           = 0
};

void bsp_pwm_init(void)
{
    uint32_t err_code;
    nrf_drv_pwm_config_t const config0 =
    {
        .output_pins =
        {            
            BLUE_LED_PWM0/* | NRF_DRV_PWM_PIN_INVERTED*/,// channel 0
            RED_LED_PWM0 ,
            NRF_DRV_PWM_PIN_NOT_USED,// channel 1
            NRF_DRV_PWM_PIN_NOT_USED,// channel 2
        },
        .irq_priority = APP_IRQ_PRIORITY_LOW,
        .base_clock   = NRF_PWM_CLK_1MHz,
        .count_mode   = NRF_PWM_MODE_UP,
        .top_value    = LED_PWM_PERIOD,
        .load_mode    = NRF_PWM_LOAD_INDIVIDUAL,
        .step_mode    = NRF_PWM_STEP_AUTO
    };
    err_code = nrf_drv_pwm_init(&m_pwm0, &config0, NULL);
    APP_ERROR_CHECK(err_code);   
    m_used |= USED_PWM(0);

    m_pwm0_seq_values.channel_0 = 0;  //
    m_pwm0_seq_values.channel_1 = 0;
    m_pwm0_seq_values.channel_2 = 0;
    m_pwm0_seq_values.channel_3 = 0;

    //(void)nrf_drv_pwm_simple_playback(&m_pwm0, &m_pwm0_seq, 1, NRF_DRV_PWM_FLAG_LOOP);

    stop_led_pwm0();
}


void led_pwm_breath_handle(void)
{
    #define  DUTY_CHANGE_STEP    LED_PWM_PERIOD/20  
    
    switch(led_pwm_inc_mode)
    {
        case 0:
          stop_led_pwm0();          
        break;
        
        case 0x0D :  //blue breath
        case 0xD0 :  //red breath
        case 0xDD :  //blue-red-b-r  breath       
        {
            static  bool  down;
            static  uint8_t b_r_exchange_cnt=0;
            static  bool br_select = 0;
            
            uint16_t * p_channels = (uint16_t *)&m_pwm0_seq_values;
            uint16_t value ;
            if(led_pwm_inc_mode==0x0d)
            {
                value = p_channels[0];//blue
            }
            else if(led_pwm_inc_mode==0xd0)
            {
                value = p_channels[1];//read
            }
            else
            {
                if(b_r_exchange_cnt>=3)
                {
                    br_select = !br_select;
                    b_r_exchange_cnt = 0;
                    down = 1;
                }
                
                if(!br_select)
                {
                    value = p_channels[0];//blue
                }
                else
                {
                    value = p_channels[1];//red
                }
            }
            
            if (down)
            {
                if(value>=DUTY_CHANGE_STEP)
                   value -= DUTY_CHANGE_STEP;
                else            
                {
                    value =0;                     
                    b_r_exchange_cnt++;
                    down = false;
                }
            }
            else
            {
                value += DUTY_CHANGE_STEP;
                if (value >= LED_PWM_PERIOD)
                {
                    value = LED_PWM_PERIOD;
                    b_r_exchange_cnt++;                    
                    down = true;
                }
            }
            if(led_pwm_inc_mode==0x0d)
            {
                p_channels[0] = value;   //blue
                p_channels[1] = LED_PWM_PERIOD;   //LED_PWM_PERIOD=0 duty  0=full duty
            }
            else if(led_pwm_inc_mode==0xd0)
            {
                p_channels[1] = value;   //blue
                p_channels[0] = LED_PWM_PERIOD;   //LED_PWM_PERIOD=0 duty  0=full duty                
            }
            else
            {
                if(!br_select)
                {
                    p_channels[0] = value;
                    p_channels[1] = LED_PWM_PERIOD;
                }
                else
                {
                    p_channels[1] = value;
                    p_channels[0] = LED_PWM_PERIOD;
                }
            }
            

        }
        break;
        
        case  0x01:
            if(m_pwm0_seq_values.channel_0 != 0 || m_pwm0_seq_values.channel_1 != LED_PWM_PERIOD)
            {
                m_pwm0_seq_values.channel_0 = 0;
                m_pwm0_seq_values.channel_1 = LED_PWM_PERIOD;
            }
            break;
        case  0x10:
            if(m_pwm0_seq_values.channel_1 != 0 || m_pwm0_seq_values.channel_0 != LED_PWM_PERIOD)
            {
                m_pwm0_seq_values.channel_1 = 0;
                m_pwm0_seq_values.channel_0 = LED_PWM_PERIOD;
            }
            break; 
        case  0x11:
            if(m_pwm0_seq_values.channel_1 != 0 || m_pwm0_seq_values.channel_0 != 0)
            {
                m_pwm0_seq_values.channel_1 = 0;
                m_pwm0_seq_values.channel_0 = 0;
            }
            break;   

        case  0x0C:
        {
            static uint8_t  on_cnt=0 , off_cnt=0, cycle_cnt=0;
            if(cycle_cnt>=4)
            {
                stop_led_pwm0();
                cycle_cnt =0;
                return;
            }
            
            if(on_cnt<1)
            {
                m_pwm0_seq_values.channel_0 = 0;
                m_pwm0_seq_values.channel_1 = LED_PWM_PERIOD;
                on_cnt++;
                off_cnt = 0;
            }
            else
            {
                m_pwm0_seq_values.channel_0 = LED_PWM_PERIOD;
                m_pwm0_seq_values.channel_1 = LED_PWM_PERIOD;
                if(off_cnt++>=3)
                {
                    off_cnt = 0;
                    on_cnt = 0;  
                    cycle_cnt++;                   
                }                
            }
            
        }
        break;
        
        case  0xC0:
         {
            static uint8_t  on_cnt=0 , off_cnt=0 ,cycle_cnt=0 ;
            if(cycle_cnt>=4)
            {
                stop_led_pwm0();
                cycle_cnt = 0;
                return;
            }
             
            if(on_cnt<1)
            {
                m_pwm0_seq_values.channel_1 = 0;
                m_pwm0_seq_values.channel_0 = LED_PWM_PERIOD;
                on_cnt++;
                off_cnt = 0;
            }
            else
            {
                m_pwm0_seq_values.channel_0 = LED_PWM_PERIOD;
                m_pwm0_seq_values.channel_1 = LED_PWM_PERIOD;
                if(off_cnt++>=3)
                {
                    off_cnt = 0;  
                    on_cnt = 0;  
                    cycle_cnt++;
                }                
            }           
         }
         break; 
         
        case  0xCC:        
         {
            static uint8_t  on_cnt=0 , off_cnt=0, cycle_cnt=0;
             
            if(cycle_cnt>=4)
            {
                stop_led_pwm0();
                cycle_cnt = 0;
                return;
            }  
            if(on_cnt<1)
            {
                m_pwm0_seq_values.channel_1 = 0;
                m_pwm0_seq_values.channel_0 = 0;
                on_cnt++;
                off_cnt = 0;
            }
            else
            {
                m_pwm0_seq_values.channel_0 = LED_PWM_PERIOD;
                m_pwm0_seq_values.channel_1 = LED_PWM_PERIOD;
                if(off_cnt++>=3)
                {
                    off_cnt = 0;  
                    on_cnt = 0;  
                    cycle_cnt++;    
                }                
            }           
         }
         break; 

         
        
        
        
        
    }
}

void stop_led_pwm0(void)
{
    m_pwm0_seq_values.channel_0 = LED_PWM_PERIOD;
    m_pwm0_seq_values.channel_1 = LED_PWM_PERIOD; 
    
    nrfx_pwm_stop(&m_pwm0,1);    
    nrf_gpio_pin_clear(RED_LED_PWM0);//
    nrf_gpio_pin_clear(BLUE_LED_PWM0);// 
    led_pwm_inc_mode = 0;
 
}

void start_led_pwm0(void)
{  
   m_pwm0_seq_values.channel_0 = LED_PWM_PERIOD;
   m_pwm0_seq_values.channel_1 = LED_PWM_PERIOD;     
   nrf_drv_pwm_simple_playback(&m_pwm0, &m_pwm0_seq, 1, NRF_DRV_PWM_FLAG_LOOP);    
}



/**************************************************************************************
 **********    gpio control led R G B 
**************************************************************************************/

#define  LED_MODE_ID_RED     0
#define  LED_MODE_ID_GREEN   1
#define  LED_MODE_ID_BLUE    2
#define  LED_MODE_ID_PWM_BR  3

#define  LED_ON              1
#define  LED_OFF             (!LED_ON)


const uint8_t  led_ind_mode_0_db[]={0,0};         //0 =off
const uint8_t  led_ind_mode_1_db[]={1,0};         //1 =on always
const uint8_t  led_ind_mode_2_db[]={2, 5, 25,0};  //2 =cycle
const uint8_t  led_ind_mode_3_db[]={2,1,29,0};
const uint8_t  led_ind_mode_4_db[]={2,1,3,1,25,0};
const uint8_t  led_ind_mode_5_db[]={2,3,27,0};
const uint8_t  led_ind_mode_6_db[]={2,1,29,0};
const uint8_t  led_ind_mode_7_db[]={2,10,20,0};

const uint8_t  led_ind_mode_8_db[]={3,1,0};       //3 =one time
const uint8_t  led_ind_mode_9_db[]={3,1,3,1,0};
const uint8_t  led_ind_mode_10_db[]={3,1,3,1,3,1,0};
const uint8_t  led_ind_mode_11_db[]={3,1,3,1,3,1,3,1,0};
const uint8_t  led_ind_mode_12_db[]={3,1,3,1,3,1,3,1,3,1,3,0};

const uint8_t * p_led_db[] =
{
    led_ind_mode_0_db,
    led_ind_mode_1_db,
    led_ind_mode_2_db,
    led_ind_mode_3_db,
    led_ind_mode_4_db,
    led_ind_mode_5_db,
    led_ind_mode_6_db,
    led_ind_mode_7_db,
    led_ind_mode_8_db,
    led_ind_mode_9_db,
    led_ind_mode_10_db,
    led_ind_mode_11_db,
    led_ind_mode_12_db,
};


led_inc_mode_t led_inc_mode_red, led_inc_mode_green, led_inc_mode_blue;


static void timer_100ms_handler(void * p_context)
{
  
    led_pwm_breath_handle();
           
    led_inc_mode_drv_process();
    
    if(led_inc_mode_red.led_cycle_flag>1 || led_inc_mode_green.led_cycle_flag>1 || 
        led_inc_mode_blue.led_cycle_flag>1 || led_pwm_inc_mode!=0)
    {       
        uint32_t err_code;
        err_code = app_timer_start(m_timer_100ms, APP_TIMER_TICKS(100), NULL); 
        APP_ERROR_CHECK(err_code);
//        NRF_LOG_INFO("100ms time out!!! %d %d %d %d", led_inc_mode_red.led_cycle_flag,led_inc_mode_red.led_cycle_flag,
//                  led_inc_mode_blue.led_cycle_flag, led_pwm_inc_mode);
    }
    
//    nrf_gpio_pin_write(8,nrf_gpio_pin_out_read(8)?0:1);
//    nrf_gpio_pin_write(6,nrf_gpio_pin_out_read(6)?0:1);
    
}

void led_rgb_timer_init(void)
{
    uint32_t err_code;
    err_code = app_timer_create(&m_timer_100ms, APP_TIMER_MODE_SINGLE_SHOT, timer_100ms_handler);
    APP_ERROR_CHECK(err_code);
}

void led_mode_set(uint8_t led_id ,  uint8_t mode)
{
    uint32_t err_code;
    if(led_id==LED_MODE_ID_RED) 
    {
        led_inc_mode_red.led_db_item_index=0;
        led_inc_mode_red.led_time_cnt = 0;
        led_inc_mode_red.led_inc_mode = mode;
        led_inc_mode_red.led_mode_db_set=p_led_db[mode];
        led_inc_mode_red.led_cycle_flag=led_inc_mode_red.led_mode_db_set[0];
        led_inc_mode_red.led_time_set = led_inc_mode_red.led_mode_db_set[1];
    }
    else if(led_id==LED_MODE_ID_GREEN) 
    {
        led_inc_mode_green.led_db_item_index=0;
        led_inc_mode_green.led_inc_mode = mode;
        led_inc_mode_green.led_mode_db_set=p_led_db[mode];
        led_inc_mode_green.led_cycle_flag=led_inc_mode_green.led_mode_db_set[0];
        led_inc_mode_green.led_time_set = led_inc_mode_green.led_mode_db_set[1];
        led_inc_mode_green.led_time_cnt = 0;

    }
    else if(led_id==LED_MODE_ID_BLUE) 
    {
        led_inc_mode_blue.led_db_item_index=0;
        led_inc_mode_blue.led_inc_mode = mode;
        led_inc_mode_blue.led_mode_db_set=p_led_db[mode];
        led_inc_mode_blue.led_cycle_flag=led_inc_mode_blue.led_mode_db_set[0];
        led_inc_mode_blue.led_time_set = led_inc_mode_blue.led_mode_db_set[1];
        led_inc_mode_blue.led_time_cnt = 0;
    }
    else if(led_id==LED_MODE_ID_PWM_BR)
    {
        led_pwm_inc_mode = mode;
        if(mode)
        {
            start_led_pwm0();            
        }
        else
        {
            stop_led_pwm0();
        }
    }
    
    err_code = app_timer_start(m_timer_100ms, APP_TIMER_TICKS(100), NULL); 
    APP_ERROR_CHECK(err_code);
    
//    NRF_LOG_INFO("100ms time out!!! %d %d %d %d", led_inc_mode_red.led_inc_mode,led_inc_mode_green.led_inc_mode,
//                  led_inc_mode_blue.led_inc_mode, led_pwm_inc_mode);
    
    led_inc_mode_drv_process();
    
        
}

void led_inc_mode_drv_process(void)
{
        if(led_inc_mode_red.led_cycle_flag==0)  //OFF ALWAYS
        { 
            nrf_gpio_pin_write(BSP_RED_LED, LED_OFF);
        }
        else if(led_inc_mode_red.led_cycle_flag==1)//on always
        {  
            nrf_gpio_pin_write(BSP_RED_LED,LED_ON);            
        }
        else  
        {  
            if(led_inc_mode_red.led_db_item_index == 0 && led_inc_mode_red.led_time_cnt==0)
            {
                nrf_gpio_pin_write(BSP_RED_LED,LED_ON);
                led_inc_mode_red.led_db_item_index=1;
                led_inc_mode_red.led_time_set = led_inc_mode_red.led_mode_db_set[1];
                
            }
            else
            {
                led_inc_mode_red.led_time_cnt++;
                if(led_inc_mode_red.led_time_cnt==led_inc_mode_red.led_time_set)
                {
                    led_inc_mode_red.led_db_item_index++;
                    
                    nrf_gpio_pin_write(BSP_RED_LED,nrf_gpio_pin_out_read(BSP_RED_LED)?0:1);

                    led_inc_mode_red.led_time_set = led_inc_mode_red.led_mode_db_set[led_inc_mode_red.led_db_item_index];
                    //NRF_LOG_INFO("red timeset:%d reddb index: %d" ,led_inc_mode_red.led_time_set , led_inc_mode_red.led_db_item_index);
                    led_inc_mode_red.led_time_cnt=0;
                    if(led_inc_mode_red.led_time_set ==0)
                    {
                        if(led_inc_mode_red.led_cycle_flag==2)
                        {
                            led_inc_mode_red.led_db_item_index=0;
                            led_inc_mode_red.led_time_cnt = 0;
                            nrf_gpio_pin_write(BSP_RED_LED,LED_OFF);
                        }
                        else if(led_inc_mode_red.led_cycle_flag==3)
                        {
                            led_inc_mode_red.led_db_item_index=0;
                            led_inc_mode_red.led_time_cnt = 0;
                            nrf_gpio_pin_write(BSP_RED_LED,LED_OFF);
                            led_inc_mode_red.led_cycle_flag=0;
                        }
                    }                        
                    
                }                    
            }                
        } 
        
//---------------------------------------------green-------------------------------------
        if(led_inc_mode_green.led_cycle_flag==0)
        { 
            nrf_gpio_pin_write(BSP_GREEN_LED, LED_OFF);
        }
        else if(led_inc_mode_green.led_cycle_flag==1)
        {  
            nrf_gpio_pin_write(BSP_GREEN_LED,LED_ON);
            
        }
        else
        {  
            if(led_inc_mode_green.led_db_item_index == 0 && led_inc_mode_green.led_time_cnt==0)
            {
                nrf_gpio_pin_write(BSP_GREEN_LED,LED_ON);
                led_inc_mode_green.led_db_item_index=1;
                led_inc_mode_green.led_time_set = led_inc_mode_green.led_mode_db_set[1];
                
            }
            else
            {
                led_inc_mode_green.led_time_cnt++;
                if(led_inc_mode_green.led_time_cnt==led_inc_mode_green.led_time_set)
                {
                    led_inc_mode_green.led_db_item_index++;
                    nrf_gpio_pin_write(BSP_GREEN_LED,nrf_gpio_pin_out_read(BSP_GREEN_LED)?0:1);

                    led_inc_mode_green.led_time_set = led_inc_mode_green.led_mode_db_set[led_inc_mode_green.led_db_item_index];
                    led_inc_mode_green.led_time_cnt=0;
                    if(led_inc_mode_green.led_time_set ==0)
                    {                        
                        if(led_inc_mode_green.led_cycle_flag==2)
                        {
                            led_inc_mode_green.led_db_item_index=0;
                            led_inc_mode_green.led_time_cnt = 0;
                            nrf_gpio_pin_write(BSP_GREEN_LED,LED_OFF);
                        }
                        else if(led_inc_mode_green.led_cycle_flag==3)
                        {
                            led_inc_mode_green.led_db_item_index=0;
                            led_inc_mode_green.led_time_cnt = 0;
                            nrf_gpio_pin_write(BSP_GREEN_LED,LED_OFF);
                            led_inc_mode_green.led_cycle_flag=0;
                        }
                    }                        
                    
                }                    
            }                
        }

//----------------------blue-----------------------------------------------------
        if(led_inc_mode_blue.led_cycle_flag==0)
        { 
            nrf_gpio_pin_write(BSP_BLUE_LED, LED_OFF);//
        }
        else if(led_inc_mode_blue.led_cycle_flag==1)
        {  
            nrf_gpio_pin_write(BSP_BLUE_LED,LED_ON);
            
        }
        else
        {  
            if(led_inc_mode_blue.led_db_item_index == 0 && led_inc_mode_blue.led_time_cnt==0)
            {
                nrf_gpio_pin_write(BSP_BLUE_LED,LED_ON); //
                led_inc_mode_blue.led_db_item_index=1;
                led_inc_mode_blue.led_time_set = led_inc_mode_blue.led_mode_db_set[1];
                
            }
            else
            {
                led_inc_mode_blue.led_time_cnt++;
                if(led_inc_mode_blue.led_time_cnt==led_inc_mode_blue.led_time_set)
                {
                    led_inc_mode_blue.led_db_item_index++;
                    nrf_gpio_pin_write(BSP_BLUE_LED,nrf_gpio_pin_out_read(BSP_BLUE_LED)?0:1);//
                    led_inc_mode_blue.led_time_set = led_inc_mode_blue.led_mode_db_set[led_inc_mode_blue.led_db_item_index];
                    led_inc_mode_blue.led_time_cnt=0;
                    if(led_inc_mode_blue.led_time_set ==0)
                    {
                        if(led_inc_mode_blue.led_cycle_flag==2)
                        {
                            led_inc_mode_blue.led_db_item_index=0;
                            led_inc_mode_blue.led_time_cnt = 0;
                            nrf_gpio_pin_write(BSP_BLUE_LED,LED_OFF);  //
                        }
                        else if(led_inc_mode_blue.led_cycle_flag==3)
                        {
                            nrf_gpio_pin_write(BSP_BLUE_LED,LED_OFF);
                            led_inc_mode_blue.led_cycle_flag=0;
                        }
                    }                        
                    
                }                    
            }                
        }            


}  

void led_inc_all_off(void)
{
    nrf_gpio_pin_write(BSP_RED_LED,LED_OFF);
    nrf_gpio_pin_write(BSP_GREEN_LED,LED_OFF);
    nrf_gpio_pin_write(BSP_BLUE_LED,LED_OFF);
    stop_led_pwm0();
}










