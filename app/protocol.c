

#include "app_heap.h"

#include "protocol.h"
#include "proto.pack.h"

#include "dev_conf.h"
#include "dev_conf.pb.h"
#include "dev_config.write.h"
#include "dev_config.read.h"

#include "crc16.h"
//#include "record.h"
#include "fw.h"

//#include "mode.h"
#include "led_pwm_ctrl.h"
#include "LIS3DH_drv.h"
#include "app_utc_time.h"
#include "app_i2c_slave.h"
#include "app_motion_detect.h"
#include "batt_adc_detect.h"


#if 1
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#define PB_LOG				NRF_LOG_DEBUG
#else
#define PB_LOG(...)
#endif 

// payload sequence 
#define COMMAND_OFFSET			0
#define LENGTH_OFFSET			1 
#define KEY_OFFSET				2 
#define VALUE_OFFSET			3  

extern resp_callback_t			m_resp_cb; 
extern msg_packet_t* 			mp_msg_ptr;


__WEAK void server_ack_alert_msg(void)
{
    // do nothing 
}

static void dev_record_data_consume( const uint8_t * p_data, uint16_t len )
{
	
}


static void dev_config_data_consume( const uint8_t * p_data, uint16_t len ) 
{
		
}

__WEAK bool loc_gps_util_enable_movement(uint32_t threshold)
{

}

static void dev_service_data_consume( const uint8_t * p_data, uint16_t len )
{
	
}

static void dev_system_control_data_consume( const uint8_t * p_data, uint16_t len)
{
	
}

static void dev_memory_read_consume( const uint8_t * p_data, uint16_t len )
{
	
}



static void dev_firmware_data_consume( const uint8_t * p_data, uint16_t len )
{
// cmd - key - value ( no length of this command ) 
//	uint8_t field_len = p_data[ LENGTH_OFFSET ]; 
	
	if( len < 2) {
		__negative_resp( EPB_ERR_INVALID_LENGTH ); 
		return ;
	}
//	uint8_t field_type = p_data[ 1 ];  //chen
    
    uint8_t field_len = p_data[ LENGTH_OFFSET ]; 
    uint16_t msg_len = len;
	uint8_t field_type = p_data[ KEY_OFFSET ];
    uint8_t const *key_value;
    if(msg_len>3)
    {
       key_value  = &p_data[ VALUE_OFFSET ];
    }
    else
    {
       key_value  = NULL;
    }
	
	// command id / field len / keyword / value 
    //PB_LOG("proto_pack_dfu_process!!"); 
	//proto_pack_dfu_process( field_type, p_data + 2, len - 2 ); 
    proto_pack_dfu_process( field_type, key_value, msg_len>3? msg_len-3:0 ); 
            
}

static void dev_factory_test_data_consume( const uint8_t * p_data, uint16_t len )
{
	
}


static uint8_t * protocol_read_step_cnt(uint8_t *readbuff)
{
    uint8_t *p_readbuff = readbuff;
    *p_readbuff++ = 5; 
	*p_readbuff++ = 0x02;
    
    uint32_t step_cnt=md_app_get_step_counter();
    *p_readbuff++= step_cnt&0xff;
    *p_readbuff++= (step_cnt>>8)&0xff;
    *p_readbuff++= (step_cnt>>16)&0xff;
    *p_readbuff++= (step_cnt>>24)&0xff;  

    return p_readbuff;
}
static uint8_t * protocol_read_acc_raw_data(uint8_t *readbuff)
{
    uint8_t *p_readbuff = readbuff;    
	*p_readbuff++ = 7; 
	*p_readbuff++= 0x01;     
	LIS3DH_GetAccAxesRaw((AxesRaw_t*)p_readbuff); //34 56 78
    p_readbuff+=6;
    return p_readbuff;
}
static uint8_t * protocol_read_status_data(uint8_t *readbuff)
{
uint8_t *p_readbuff = readbuff;    
	*p_readbuff++ = 5;     //len
	*p_readbuff++ = 0x03;  //key
    
    *p_readbuff = 0;
    if(md.motion_alert_flag)
    {
        *p_readbuff |= 1;
        md.motion_alert_flag = 0;
    }
    if(md.static_alert_flag)
    {
        *p_readbuff|= 1<<1;
        md.static_alert_flag = 0;
    }
    if(md.tilt_flag)
    {      
        *p_readbuff|= 1<<2;
        md.tilt_flag = 0;
    } 
    if(md.fall_down_flag)
    {
        *p_readbuff|= 1<<3;
        md.fall_down_flag = 0;
    } 
//    if(md.motion_ind)
//    {
//        *p_readbuff|= 1<<4;
//    }
    p_readbuff++;
    *p_readbuff++= 0;
    *p_readbuff++= 0;    
    *p_readbuff++= m_batt_level; 
    
    return p_readbuff;
}

static uint8_t * protocol_read_base_info(uint8_t *readbuff)
{
    uint8_t *p_readbuff = readbuff;
    *p_readbuff++ = 41;     //len=key_value_len + key_len
	*p_readbuff++ = 0x08;  //key
    
    memcpy(p_readbuff, (uint8_t *)&appl_info.fw_version, 40/*sizeof(app_info_t)*/);
    
    p_readbuff += 40/*sizeof(app_info_t)*/;
    
    return p_readbuff;
    
}

static uint8_t * protocol_read_int_report(uint8_t *readbuff)
{
   uint8_t *p_readbuff = readbuff;
	*p_readbuff++ = 8;     //len=key_value_len + key_len
	*p_readbuff++ = 0x10;  //key
    
    memcpy(p_readbuff, (uint8_t*)&pin_int_en.int_en.value, 4); 
    p_readbuff+=4;
   
    *p_readbuff++=pin_int_en.step_threshold&0x00ff; //step threshold 1
    *p_readbuff++ =(pin_int_en.step_threshold&0xff00)>>8; //step threshold 2
    *p_readbuff++ = pin_int_en.batt_threshold;
    
    return p_readbuff;
}

static uint8_t * protocol_read_utc_time(uint8_t *readbuff)
{
   uint8_t *p_readbuff = readbuff;
	*p_readbuff++ = 5;     //len=key_value_len + key_len
	*p_readbuff++ = 0x11;  //key
    
    memcpy(p_readbuff, (uint8_t*)&m_user_time_senconds, sizeof(m_user_time_senconds));   
    p_readbuff += sizeof(m_user_time_senconds);
    
    return p_readbuff;
}
static uint8_t * protocol_read_led_mode(uint8_t *readbuff)
{
    uint8_t *p_readbuff = readbuff;
    *p_readbuff++ = 5;     //len=key_value_len + key_len
	*p_readbuff++ = 0x12;  //LED CTRL
    
    *p_readbuff++ = led_inc_mode_red.led_inc_mode;     
	*p_readbuff++ = led_inc_mode_green.led_inc_mode; 
    *p_readbuff++ = led_inc_mode_blue.led_inc_mode;     
	*p_readbuff++ = 0x00;//led_br
    
    return p_readbuff;
}

static uint8_t * protocol_read_motion_alert(uint8_t *readbuff)
{
    uint8_t *p_readbuff = readbuff;
    *p_readbuff++ = 5;     //len=key_value_len + key_len
	*p_readbuff++ = 0x52;  //motion ALERT
    
    uint32_t temp_motion;
    
    temp_motion = (uint32_t)(dev_settings.alert.motion.value);
    memcpy(p_readbuff, (uint8_t *)&temp_motion, 4);
    
    p_readbuff += 4;   

   return p_readbuff;    
}
static uint8_t * protocol_read_nomotion_alert(uint8_t *readbuff)
{
    uint8_t *p_readbuff = readbuff;
    *p_readbuff++ = 5;     //len=key_value_len + key_len
	*p_readbuff++ = 0x53;  //no motion ALERT
    
    uint32_t temp_nomotion=0;
    temp_nomotion = (uint32_t)(dev_settings.alert.actionless.value);;

    memcpy(p_readbuff, (uint8_t *)&temp_nomotion, 4);
    p_readbuff += 4; 
return p_readbuff;    
}
static uint8_t * protocol_read_tilt_alert(uint8_t *readbuff)
{
    uint8_t *p_readbuff = readbuff;
    *p_readbuff++ = 5;     //len=key_value_len + key_len
	*p_readbuff++ = 0x55;  //tilt ALERT
    
    uint32_t temp_tilt = (uint32_t)(dev_settings.alert.tilt.value);
    
    memcpy(p_readbuff, (uint8_t *)&temp_tilt, 4);
    p_readbuff += 4; 
    
    return p_readbuff;
}
static uint8_t * protocol_read_falldown_alert(uint8_t *readbuff)
{
    uint8_t *p_readbuff = readbuff;
    *p_readbuff++ = 2;     //len=key_value_len + key_len
	*p_readbuff++ = 0x56;  //falldown ALERT
   
//    uint32_t temp_falldown = (uint32_t)(dev_settings.alert.falldown.value);    
//    memcpy(p_readbuff, (uint8_t *)&temp_falldown, 4);
//    p_readbuff += 4; 
    *p_readbuff++ = dev_settings.alert.falldown.value;
    
    return p_readbuff;
    
}

static void protocol_read_acc_data_process(void)
{
    uint8_t buff[9];
	
	buff[ 0 ] = COMMAND_ID_SENSOR_HUB; 
	buff[ 1 ] = 7; 
	buff[ 2 ] = 0x01; 
    
	LIS3DH_GetAccAxesRaw((AxesRaw_t*)(&buff[3])); //34 56 78
	
	__data_pack( buff, 9/*sizeof(buff)*/, FLAG_NORMAL_RESP ); 
}
static void protocol_read_step_cnt_process(void)
{
    uint8_t buff[9];    
    buff[ 0 ] = COMMAND_ID_SENSOR_HUB; 
	buff[ 1 ] = 5; 
	buff[ 2 ] = 0x02;
    
    uint32_t step_cnt=md_app_get_step_counter();
    buff[ 3 ]= step_cnt&0xff;
    buff[ 4 ]= (step_cnt>>8)&0xff;
    buff[ 5 ]= (step_cnt>>16)&0xff;
    buff[ 6 ]= (step_cnt>>24)&0xff;
    
    __data_pack( buff, 7, FLAG_NORMAL_RESP ); 
}

static void protocol_read_status_process(void)
{   
    uint8_t buff[9];    
    buff[ 0 ] = COMMAND_ID_SENSOR_HUB; 
    protocol_read_status_data(&buff[1]);
    __data_pack( buff, 7, FLAG_NORMAL_RESP ); 
}

//-----------------------------------------------------------
static uint8_t* protocol_read_comb_msg(const uint8_t * p_data, uint16_t len, uint8_t *rsp_buff)
{
 
    uint8_t *p_buff_tail = rsp_buff;
    uint16_t temp_len=len; 
    
    for(uint32_t i=0; i<temp_len; i--)
    {
        switch(p_data[i])
        {
        case  HUB_KEY_XYZ_READ:
            p_buff_tail = protocol_read_acc_raw_data(p_buff_tail);
            break;
        case  HUB_KEY_STEP_COUNT_READ:
            p_buff_tail = protocol_read_step_cnt(p_buff_tail);
            break;
        case  HUB_KEY_STATE:
            p_buff_tail = protocol_read_status_data(p_buff_tail);
            break;
        case  HUB_KEY_INFO:
            p_buff_tail = protocol_read_base_info(p_buff_tail);
            break;
        case  HUB_KEY_INT_ENABLE:
            p_buff_tail = protocol_read_int_report(p_buff_tail);
            break;
        case  HUB_KEY_UTC_UPDATE:
            p_buff_tail = protocol_read_utc_time(p_buff_tail);
            break; 
        
        case  HUB_KEY_LED_CTRL:
            p_buff_tail = protocol_read_led_mode(p_buff_tail);
            break;
        
        case  HUB_KEY_MOTION_ALERT:
            p_buff_tail = protocol_read_motion_alert(p_buff_tail);
            break;
        case  HUB_KEY_NOMOTION_ALERT:
            p_buff_tail = protocol_read_nomotion_alert(p_buff_tail);
            break;
        case  HUB_KEY_TILT_ALERT:
            p_buff_tail = protocol_read_tilt_alert(p_buff_tail);
            break;
        case  HUB_KEY_FALL_DOWN_ALERT:
            p_buff_tail = protocol_read_falldown_alert(p_buff_tail);
            break;
        default:
            break;
        }
    }
    
    return  p_buff_tail; 
          
}
static uint8_t* protocol_read_all_msg(uint8_t *rsp_buff)
{
   
   uint8_t *p_buff_tail =rsp_buff;
   
   p_buff_tail = protocol_read_step_cnt(p_buff_tail);
   p_buff_tail = protocol_read_acc_raw_data(p_buff_tail);
   p_buff_tail = protocol_read_status_data(p_buff_tail);
   p_buff_tail = protocol_read_base_info(p_buff_tail);
   p_buff_tail = protocol_read_int_report(p_buff_tail);
   p_buff_tail = protocol_read_utc_time(p_buff_tail);
   p_buff_tail = protocol_read_led_mode(p_buff_tail);
   p_buff_tail = protocol_read_motion_alert(p_buff_tail);
   p_buff_tail = protocol_read_nomotion_alert(p_buff_tail);
   p_buff_tail = protocol_read_tilt_alert(p_buff_tail);
   p_buff_tail = protocol_read_falldown_alert(p_buff_tail);
    
   return  p_buff_tail;  
}

static void protocol_read_all_msg_process(void)
{
   
    uint8_t buff[128]={0};    
    buff[ 0 ] = COMMAND_ID_SENSOR_HUB;  
    
   uint8_t *p_buff_tail = &buff[1];
   
   p_buff_tail = protocol_read_step_cnt(p_buff_tail);
   p_buff_tail = protocol_read_acc_raw_data(p_buff_tail);
   p_buff_tail = protocol_read_status_data(p_buff_tail);
   p_buff_tail = protocol_read_base_info(p_buff_tail);
   p_buff_tail = protocol_read_int_report(p_buff_tail);
   p_buff_tail = protocol_read_utc_time(p_buff_tail);
   p_buff_tail = protocol_read_led_mode(p_buff_tail);
   p_buff_tail = protocol_read_motion_alert(p_buff_tail);
   p_buff_tail = protocol_read_nomotion_alert(p_buff_tail);
   p_buff_tail = protocol_read_tilt_alert(p_buff_tail);
   p_buff_tail = protocol_read_falldown_alert(p_buff_tail);
    

   __data_pack( buff, p_buff_tail-buff, FLAG_NORMAL_RESP );
}

static void protocol_read_comb_msg_process(const uint8_t * p_data, uint16_t len)
{
    uint16_t temp_len=len;
    uint8_t buff[128]={0};
    buff[ 0 ] = COMMAND_ID_SENSOR_HUB; 
    uint8_t *p_buff_tail = &buff[1];
    
    for(uint32_t i=0; i<temp_len; i--)
    {
        switch(p_data[i])
        {
        case  HUB_KEY_XYZ_READ:
            p_buff_tail = protocol_read_acc_raw_data(p_buff_tail);
            break;
        case  HUB_KEY_STEP_COUNT_READ:
            p_buff_tail = protocol_read_step_cnt(p_buff_tail);
            break;
        case  HUB_KEY_STATE:
            p_buff_tail = protocol_read_status_data(p_buff_tail);
            break;
        case  HUB_KEY_INFO:
            p_buff_tail = protocol_read_base_info(p_buff_tail);
            break;
        case  HUB_KEY_INT_ENABLE:
            p_buff_tail = protocol_read_int_report(p_buff_tail);
            break;
        case  HUB_KEY_UTC_UPDATE:
            p_buff_tail = protocol_read_utc_time(p_buff_tail);
            break; 
        
        case  HUB_KEY_LED_CTRL:
            p_buff_tail = protocol_read_led_mode(p_buff_tail);
            break;
        
        case  HUB_KEY_MOTION_ALERT:
            p_buff_tail = protocol_read_motion_alert(p_buff_tail);
            break;
        case  HUB_KEY_NOMOTION_ALERT:
            p_buff_tail = protocol_read_nomotion_alert(p_buff_tail);
            break;
        case  HUB_KEY_TILT_ALERT:
            p_buff_tail = protocol_read_tilt_alert(p_buff_tail);
            break;
        case  HUB_KEY_FALL_DOWN_ALERT:
            p_buff_tail = protocol_read_falldown_alert(p_buff_tail);
            break;
        default:
            break;
        }
    }
    
    __data_pack( buff, p_buff_tail-buff, FLAG_NORMAL_RESP );
       
}

static void protocol_read_info_process(void)
{
    uint8_t buff[43]={0};
    
    buff[ 0 ] = COMMAND_ID_SENSOR_HUB; 
	buff[ 1 ] = 41;     //len=key_value_len + key_len
	buff[ 2 ] = 0x08;  //key
    
    memcpy(&buff[3], (uint8_t *)&appl_info, sizeof(app_info_t));
    
     __data_pack( buff, 43, FLAG_NORMAL_RESP ); 
}



//=====================write key==================================

static uint8_t protocol_led_ctrl_cmd( const uint8_t * p_data, uint16_t len)
{
    
    if( len != 4 ){
		return EPB_ERR_INVALID_LENGTH ; 
	}
        
    for(uint32_t i=0; i<4 ; i++)
    {
        if(p_data[i]!=0xff)
        {
            led_mode_set(i ,  p_data[i]);      
        }    
    }
        
    return EPB_SUCCESS;     
}
static uint8_t protocol_update_utc(const uint8_t * p_data, uint16_t len)
{
    if( len != 4 ){ 
		return EPB_ERR_INVALID_LENGTH ; 
	}
    //m_user_time_senconds = *((uint32_t*)p_data); 
    m_user_time_senconds = p_data[0] + (p_data[1]<<8) + (p_data[2]<<16) + (p_data[3]<<24); 
    app_seconds_to_utc(&m_user_time_utc ,  m_user_time_senconds );
    
    return EPB_SUCCESS;
}



static uint8_t protocol_set_int_en(const uint8_t * p_data, uint16_t len)
{
    
    if( len != 7 ){ 
		return EPB_ERR_INVALID_LENGTH ; 
	}       
        
    memcpy((uint8_t *)&pin_int_en.int_en.value, p_data, 4);
    pin_int_en.step_threshold = p_data[4]+(p_data[5]<<8);
    pin_int_en.batt_threshold = p_data[6];
    
    return EPB_SUCCESS;
}

static uint8_t protocol_set_motion_alert_data(const uint8_t * p_data, uint16_t len)
{   
    if( len != 4 ){ 
		return EPB_ERR_INVALID_LENGTH ; 
	}
    dfw_alert_motion_write( p_data, 4);
    
    return EPB_SUCCESS;
}

static uint8_t protocol_set_static_alert_data(const uint8_t * p_data, uint16_t len)
{
    if( len != 4 ){ 
		return EPB_ERR_INVALID_LENGTH ; 
	}
    
    dfw_alert_actionless_write( p_data, 4);
    
    return EPB_SUCCESS;
}

static uint8_t protocol_set_tilt_alert_data(const uint8_t * p_data, uint16_t len)
{
    if( len != 4 ){ 
		return EPB_ERR_INVALID_LENGTH ; 
	}    
    
     dfw_alert_tilt_write( p_data, 4);
    
    return EPB_SUCCESS;
}
static uint8_t protocol_set_fall_down_alert_data(const uint8_t * p_data, uint16_t len)
{
    if( len != 1 ){ 
		return EPB_ERR_INVALID_LENGTH ; 
	}    
    dfw_alert_falldown_write( p_data, 1);
    
    return EPB_SUCCESS;
}



static void sensor_hub_cmd_hanle(const uint8_t * p_data, uint16_t len )
{
    if( len < 3 ){
		__negative_resp( EPB_ERR_INVALID_LENGTH ); 
		return ; 
	}
    PB_LOG(" sensor_hub_cmd_hanle!!"); 
    
    uint16_t msg_len=len-1; //0x08 cmd
    uint16_t msg_offset = 0;
    uint8_t  rsp_error_code = 0;
    uint8_t  rsp_buff[128]={0};
    uint8_t  *p_rsp_buff = rsp_buff + 1;
    rsp_buff[0]= COMMAND_ID_SENSOR_HUB;
       
    while(msg_len)
    {
        
	uint8_t field_len = p_data[msg_offset + LENGTH_OFFSET]; 
	uint8_t field_type = p_data[msg_offset + KEY_OFFSET]; 
    uint8_t const *key_value  = &p_data[msg_offset + VALUE_OFFSET]; 
    if(msg_len>field_len+1)
    {
        msg_len -=(field_len+1); 
        msg_offset+=(field_len+1); 
    }
    else{
        msg_len = 0;
        
    }
    
        
//	uint8_t field_len = p_data[ LENGTH_OFFSET ]; 
//	uint8_t field_type = p_data[ KEY_OFFSET ]; 
//    uint8_t const *key_value  = &p_data[ VALUE_OFFSET ];   
	switch( field_type )
    {
        case  HUB_KEY_XYZ_READ:
            //protocol_read_acc_data_process();
            p_rsp_buff =protocol_read_acc_raw_data(p_rsp_buff);
            break;
        case  HUB_KEY_STEP_COUNT_READ:
            //protocol_read_step_cnt_process();
            p_rsp_buff = protocol_read_step_cnt(p_rsp_buff);
            break;
        case  HUB_KEY_STATE:
            //protocol_read_status_process();
            p_rsp_buff = protocol_read_status_data(p_rsp_buff);
            break;
        case  HUB_KEY_INFO:
           // protocol_read_info_process();
            p_rsp_buff = protocol_read_base_info(p_rsp_buff);
            break;
        
        case  HUB_KEY_MSG_READ:
            if(field_len==1)  //0xF0
            {
               p_rsp_buff = protocol_read_all_msg(p_rsp_buff);
            }
            else if(field_len>1)
            {
               p_rsp_buff = protocol_read_comb_msg(key_value, field_len-1,p_rsp_buff);                
            } 
            
            msg_len = 0; 
            
            break;
        
        case  HUB_KEY_INT_ENABLE:
            rsp_error_code=protocol_set_int_en(key_value, field_len-1);
            break;
        case  HUB_KEY_UTC_UPDATE:
            rsp_error_code=protocol_update_utc(key_value, field_len-1);
            break; 
        
        case  HUB_KEY_LED_CTRL:
            //NRF_LOG_INFO(" protocol_led_ctrl_cmd_process!!");    
            rsp_error_code=protocol_led_ctrl_cmd(key_value, field_len-1);
            break;
        
        case  HUB_KEY_MOTION_ALERT:
            rsp_error_code=protocol_set_motion_alert_data(key_value, field_len-1);
           //p_rsp_buff = protocol_read_motion_alert(p_rsp_buff);
            break;
        case  HUB_KEY_NOMOTION_ALERT:
            rsp_error_code=protocol_set_static_alert_data(key_value, field_len-1);
            //p_rsp_buff =protocol_read_nomotion_alert(p_rsp_buff);
            break;
        case  HUB_KEY_TILT_ALERT:
            rsp_error_code=protocol_set_tilt_alert_data(key_value, field_len-1);
            //p_rsp_buff =protocol_read_tilt_alert(p_rsp_buff);
            break;
        case  HUB_KEY_FALL_DOWN_ALERT:
            rsp_error_code=protocol_set_fall_down_alert_data(key_value, field_len-1);
            //p_rsp_buff =protocol_read_falldown_alert(p_rsp_buff);
            break;
       
     }
    
     if(rsp_error_code!=EPB_SUCCESS)
     {
         __negative_resp(rsp_error_code); 
         return;
     }
   
    
  } 
   
  if(mp_msg_ptr->flag.ack)
  {     
       if(p_rsp_buff > rsp_buff+1+2) //cmd len key
       {           
           __data_pack(rsp_buff, p_rsp_buff - rsp_buff, FLAG_NORMAL_RESP );
       }
       else if(p_rsp_buff == rsp_buff+1)
       {
           __negative_resp(rsp_error_code);  
       }
       else
       {
           __negative_resp(EPB_ERR_DATA_SIZE_ERROR);  
       }
   
  }
    
  
        
          
}
void protocol_data_receive_consume(msg_packet_t * p_msg, resp_callback_t evt_handle)
{
	if( p_msg == NULL || p_msg->payload == NULL /*|| evt_handle == NULL*/)
		return; 
	
	uint16_t crc16 = crc16_compute( p_msg->payload, p_msg->len, 0);
	
	if( crc16 != p_msg->crc )
		return; 
	
	m_resp_sent = false; 
	m_resp_cb =  evt_handle ; 
	mp_msg_ptr = p_msg ;
	
	switch( p_msg->payload[ COMMAND_OFFSET ] )
	{
		default:
			break; 
		
		case COMMAND_ID_DATA:
			dev_record_data_consume( p_msg->payload, p_msg->len ); 
			break; 
		
		case COMMAND_ID_CONFIG:
			dev_config_data_consume( p_msg->payload, p_msg->len ); 
			break; 
		
		case COMMAND_ID_SERVICES:
			dev_service_data_consume( p_msg->payload, p_msg->len ); 
			break; 
		
		case COMMAND_ID_SYS_CONTROL:
			dev_system_control_data_consume( p_msg->payload, p_msg->len ); 
			break; 
		
		case COMMAND_ID_MEMORY_READER:
			dev_memory_read_consume( p_msg->payload, p_msg->len ); 
			break; 
        
        
        case COMMAND_ID_SENSOR_HUB:
            
            sensor_hub_cmd_hanle(p_msg->payload, p_msg->len);      //chen
        
            break;
	
		case COMMAND_ID_FIRMWARE:
            PB_LOG(" COMMAND_ID_FIRMWARE   msg_len: %d !!" ,  p_msg->len);  
			dev_firmware_data_consume( p_msg->payload, p_msg->len );   //chen
			break; 
		
		case COMMAND_ID_FACOTRY_TEST:
			dev_factory_test_data_consume( p_msg->payload, p_msg->len ); 
			break; 
	}
	
	// if need ack, send an ack response when no response has been sent 
//	if( p_msg->flag.ack > 0 && !m_resp_sent ){
//		__ack_resp(); 
//	}
}


