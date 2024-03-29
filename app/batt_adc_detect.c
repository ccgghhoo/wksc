/*
*
*
*/
#include "bsp.h"
#include "nrf_saadc.h"
#include "nrfx_saadc.h"
#include "nrf_drv_saadc.h"
#include "batt_adc_detect.h"
#include "nrf_log.h"
//#include "nrf_log_ctrl.h"
//#include "nrf_log_default_backends.h"


#define  BATT_SAMPLE_NUM        8
#define  BATT_AN_PIN            NRF_SAADC_INPUT_AIN0
#define  ADVALUE_TO_MV_RATIO    600*11/4096

uint8_t                      m_batt_level;
static nrf_saadc_value_t     m_batt_adc_buffer[BATT_SAMPLE_NUM];
static uint32_t              m_batt_mv_av;
static bool                  batt_av_saadc_done_flag;


void saadc_callback(nrf_drv_saadc_evt_t const * p_event)
{
    //NRF_LOG_INFO("p_event->type: %d n\r", p_event->type);
    
    if (p_event->type == NRF_DRV_SAADC_EVT_DONE)
    {
        ret_code_t err_code;

        err_code = nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, BATT_SAMPLE_NUM);// init easyDMA
        APP_ERROR_CHECK(err_code);

        int i, batt_sum=0;

        for (i = 0; i < BATT_SAMPLE_NUM; i++)
        {
            batt_sum+=p_event->data.done.p_buffer[i];
            //NRF_LOG_INFO("%d", p_event->data.done.p_buffer[i]);
        }
        batt_sum /= BATT_SAMPLE_NUM;
        m_batt_mv_av = batt_sum*ADVALUE_TO_MV_RATIO;     //Vref=600mv  resolution=12bit  PCB resistor ratio=1/11
        batt_av_saadc_done_flag = true;
        NRF_LOG_INFO("batt adc value: %d \n\r", batt_sum);
        NRF_LOG_INFO("batt voltage : %d mv \n\r", m_batt_mv_av);
    }
}



void batt_adc_init(void)
{
     ret_code_t err_code;
    nrf_saadc_channel_config_t channel_config =
        NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(BATT_AN_PIN);
    
    channel_config.gain = NRF_SAADC_GAIN1;  //gain=1
//    nrfx_saadc_config_t  saadc_config = NRFX_SAADC_DEFAULT_CONFIG;
//    saadc_config.low_power_mode=1;
    err_code = nrf_drv_saadc_init(NULL, saadc_callback);
    APP_ERROR_CHECK(err_code);

    err_code = nrfx_saadc_channel_init(0, &channel_config);
    APP_ERROR_CHECK(err_code);

//    err_code = nrfx_saadc_buffer_convert(m_batt_adc_buffer, BATT_SAMPLE_NUM);  
//    APP_ERROR_CHECK(err_code);
    
    nrfx_saadc_sample();

}

void batt_adc_uninit(void)
{    
   nrfx_saadc_uninit();
}

void batt_start_measure(void)
{    
    nrfx_saadc_sample(); // only start adc sample once
}

int16_t batt_adc_value_get(void)
{
    int16_t bat_value;
    ret_code_t err_code;
    err_code = nrfx_saadc_sample_convert(BATT_AN_PIN, &bat_value); //cpu is blocked until ad convert ended
//    NRF_LOG_INFO("adc sample error code : %x \n\r", err_code);   
    if(err_code!=NRF_SUCCESS)
        return 0;
    return bat_value;
}

int16_t batt_adc_value_av_get(void)
{
    uint32_t batt_value_sum =0;
    for(uint32_t i =0; i<BATT_SAMPLE_NUM; i++)
    {
        m_batt_adc_buffer[i] = batt_adc_value_get();
        batt_value_sum += m_batt_adc_buffer[i];
    }
    batt_value_sum /= BATT_SAMPLE_NUM;
    
    return batt_value_sum;
}

uint32_t batt_voltage_av_get(void)
{    
    return m_batt_mv_av;
}

int16_t batt_voltage_get(void)
{
    int16_t bat_value;
    uint32_t batt_mv;
    bat_value=batt_adc_value_av_get();
    batt_mv = bat_value*ADVALUE_TO_MV_RATIO;
    NRF_LOG_INFO("batt mv: %d  ", batt_mv); 
    return batt_mv;
}



uint8_t battery_level_cal(void)
{
    #define MIN_BATTER_ADC  			2048  	//3.3v
    #define MAX_BATTER_ADC  			2544  	//4.1v

    
    uint32_t  batt_level;
    
    int16_t batt_adc_value = batt_adc_value_av_get();
          
    if(batt_adc_value>=MAX_BATTER_ADC) 
    {
        batt_level = 100;
    }
    else if(batt_adc_value<=MIN_BATTER_ADC)
    {
        batt_level = 0;
    }
    else
    {
        batt_level = (batt_adc_value-MIN_BATTER_ADC)*100/(MAX_BATTER_ADC-MIN_BATTER_ADC);
    }

    m_batt_level = batt_level;
    
    NRF_LOG_INFO("batt level: %d  ", batt_level); 
    	
    return m_batt_level;
}

uint8_t batt_level_get(void)
{       
    return  m_batt_level;
}




