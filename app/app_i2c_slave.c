/**
*
*
*
**/

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
//#include "config.h"
#include "app_i2c_slave.h"
#include "nrf_drv_twis.h"
#include "nrf_gpio.h"
#include "app_util_platform.h"
#include "bsp.h"
#include "protocol.h"
#include "app_evt.h"
#include "app_heap.h"
#include "nrf_log.h"

#if 1
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#define I2C_SLAVE_LOG				        NRF_LOG_DEBUG
#else
#define I2C_SLAVE_LOG(...)
#endif


#define  APP_I2C_SLAVE_ADDR         0x50 
#define  APP_I2C_SLAVE_INST         1
//#define  APP_I2C_SLAVE_SCL_PIN    23
//#define  APP_I2C_SLAVE_SDA_PIN    24 
#define  APP_I2CS_PAKAGE_MAX_SIZE   252
#define  APP_I2CS_RX_BUFF_SIZE      252
#define  APP_I2CS_TX_BUFF_SIZE      (1+1+2+2+2+1024)//header properties length sequence id checksum Message body


    uint8_t m_i2c_slave_rxbuff[APP_I2CS_RX_BUFF_SIZE];
    uint8_t m_i2c_slave_txbuff[APP_I2CS_TX_BUFF_SIZE];
    
    static bool m_error_flag;
    bool 		m_resp_sent=0;
    bool        m_need_ack=0;  //2019/06/21    
//    uint8_t     read_status=0;
    bool     protocl_package_wait_handle_flag =0;
    bool     write_data_flag=0;
    bool     read_data_flag=0;
    bool     read_data_len_flag=0;
    uint16_t     m_resp_len ;
    uint16_t     m_i2c_sended_len ;
    
static msg_packet_t		i2c_slave_recv_msg ; 
static uint32_t 		m_i2c_slave_msg_state = 0; 
static uint16_t 		m_bytes_received = 0; 

    //TWIS instance.
    static const nrf_drv_twis_t m_twis = NRF_DRV_TWIS_INSTANCE(APP_I2C_SLAVE_INST);
    static app_twis_config_t m_config;


      
    
    /**
     * @brief Start after the WRITE command.
     *
     * Function sets pointers for TWIS to receive data.
     * WRITE command does not write directly to memory array.
     * Temporary receive buffer is used.
     * @sa m_rxbuff
     */
    static void app_i2c_slave_write_begin(void)
    {
        (void)nrf_drv_twis_rx_prepare(&m_twis, m_i2c_slave_rxbuff, sizeof(m_i2c_slave_rxbuff));

        //I2C_SLAVE_LOG("i2c write start !!");
    }

    /**
     * @brief Finalize the WRITE command.
     *
     * Call this function when the write command is finished.
     * It sets a memory pointer and writes all received data to the memory array.
     */
    static void app_i2c_slave_write_end(size_t cnt)
    {       
        
        //I2C_SLAVE_LOG("i2c write ended !!");
        //I2C_SLAVE_LOG("i2c write bytes: %d " , cnt);
//        for(uint32_t i=0; i<cnt; i++)
//        {
//            NRF_LOG_INFO(" %x " , m_i2c_slave_rxbuff[i]);
//        }
//        
        if(write_data_flag)
        {
           write_data_flag = 0; 
           app_tws_recdata_handle(m_i2c_slave_rxbuff, cnt); 
            
        }
        else
        {
            if(cnt==1)
            {
                if(m_i2c_slave_rxbuff[0]==0x20)
                {
                    write_data_flag = 1;
                    //NRF_LOG_INFO("write_data_flag !!");
                }
                else if(m_i2c_slave_rxbuff[0]==0x10)
                {
                    read_data_flag = 1;
                    //NRF_LOG_INFO("read_data_flag !!");
                }
                else if(m_i2c_slave_rxbuff[0]==0x02)
                {
                    read_data_len_flag = 1;
                    //NRF_LOG_INFO("read_data_len_flag !!");
                }            
            }
        }                           
        
    }

    /**
     * @brief Start after the READ command.
     *
     * Function sets pointers for TWIS to transmit data from the current address to the end of memory.
     */
    static void app_i2c_slave_read_begin(void)
    {
        //I2C_SLAVE_LOG("i2c read start !!");
        
        if(m_resp_sent)
        {            
            //I2C_SLAVE_LOG("i2c  read resp data :%x !!", m_resp_len);
            if(read_data_len_flag)
            {
               uint8_t buff[2];            
                buff[0]=m_resp_len&0xff; //length
                buff[1]=(m_resp_len&0xff00)>>8;
                m_i2c_sended_len = 0;
                nrf_drv_twis_tx_prepare(&m_twis, buff, 2);  
            }
            else if(read_data_flag)
            {
                //m_resp_sent=false;
                if(m_resp_len>APP_I2CS_PAKAGE_MAX_SIZE)
                {                    
                    nrf_drv_twis_tx_prepare(&m_twis, &m_i2c_slave_txbuff[m_i2c_sended_len], APP_I2CS_PAKAGE_MAX_SIZE);
                    m_i2c_sended_len+=APP_I2CS_PAKAGE_MAX_SIZE;
                    m_resp_len-=APP_I2CS_PAKAGE_MAX_SIZE;
                }
                else
                {
//                    for(uint32_t i=0; i<m_resp_len; i++)
//                    {
//                        NRF_LOG_INFO(" %x", m_i2c_slave_txbuff[i]);
//                    }
                    nrf_drv_twis_tx_prepare(&m_twis, &m_i2c_slave_txbuff[m_i2c_sended_len], m_resp_len);
                    m_resp_len = 0;
                    m_i2c_sended_len = 0;
                    m_resp_sent=false;                    
                }                    
                
            }
        }
        else  // no data to send
        {
            if(read_data_len_flag)
            {
               uint8_t buff[2]; 
                m_resp_len = 0;    
                buff[0]=0; //length
                buff[1]=0;
                m_i2c_sended_len = 0;
                nrf_drv_twis_tx_prepare(&m_twis, buff, 2);  
            }
            else if(read_data_flag)
            {                

            }
      
        }
                       
    }

    /**
     * @brief Finalize the READ command.
     *
     * Call this function when the read command is finished.
     * It adjusts the current m_addr pointer.
     * @param cnt Number of bytes read.
     */
    static void app_i2c_slave_read_end(size_t cnt)
    {
//        I2C_SLAVE_LOG("i2c read ended !!");
//        I2C_SLAVE_LOG("i2c read bytes: %d " , cnt);
        if(read_data_flag)
        {
            read_data_flag = 0;
        }
        
        if(read_data_len_flag)
        {
            read_data_len_flag = 0;
        }
        
        
    }


    /**
     * @brief Event processing.
     *
     *
     */
    static void app_twis_event_handler(nrf_drv_twis_evt_t const * const p_event)
    {
        switch (p_event->type)
        {
        case TWIS_EVT_READ_REQ:
            if (p_event->data.buf_req)
            {
                app_i2c_slave_read_begin();
            }
            break;
        case TWIS_EVT_READ_DONE:
            app_i2c_slave_read_end(p_event->data.tx_amount);
            break;
        case TWIS_EVT_WRITE_REQ:
            if (p_event->data.buf_req)
            {
                app_i2c_slave_write_begin();
            }
            break;
        case TWIS_EVT_WRITE_DONE:
            app_i2c_slave_write_end(p_event->data.rx_amount);
            break;

        case TWIS_EVT_READ_ERROR:
            I2C_SLAVE_LOG("i2c read error !!");
            
            break;
        case TWIS_EVT_WRITE_ERROR:
            I2C_SLAVE_LOG("i2c write error !!");
            break;
        case TWIS_EVT_GENERAL_ERROR:
            I2C_SLAVE_LOG("i2c general error !!");
            break;
            //m_error_flag = true;
            
            break;
        default:
            break;
        }
    }

/** @} */
void app_i2c_slave_uninit(void)  //chen 07/31
{
    nrfx_twis_uninit(&m_twis);
}    


ret_code_t app_i2c_slave_init(void)
{
    ret_code_t ret;

    m_config.slave_addr                = APP_I2C_SLAVE_ADDR;
    m_config.scl_s                     = APP_I2C_SLAVE_SCL_PIN;
    m_config.sda_s                     = APP_I2C_SLAVE_SDA_PIN;

    const nrf_drv_twis_config_t config =
    {
        .addr               = {m_config.slave_addr, 0},
        .scl                = m_config.scl_s,
        .scl_pull           = NRF_GPIO_PIN_NOPULL,
        .sda                = m_config.sda_s,
        .sda_pull           = NRF_GPIO_PIN_NOPULL,
        .interrupt_priority = APP_IRQ_PRIORITY_HIGH
    };

    /* Init TWIS */
    do
    {
        ret = nrf_drv_twis_init(&m_twis, &config, app_twis_event_handler);
        if (NRF_SUCCESS != ret)
        {
            break;
        }

        nrf_drv_twis_enable(&m_twis);
    }while (0);
    return ret;
}

bool app_i2c_slave_error_check(void)
{
    return m_error_flag;
}

uint32_t app_i2c_slave_error_get_and_clear(void)
{
    uint32_t ret = nrf_drv_twis_error_get_and_clear(&m_twis);
    m_error_flag = false;
    return ret;
}
//================================================================

//uint8_t get_protocol_ack_flag(void)
//{
//    return i2c_slave_recv_msg.flag.ack;
//}


void app_tws_recdata_handle(uint8_t * p_data, uint16_t len)
{
	uint32_t 	index = 0; 
    //I2C_SLAVE_LOG(" app_tws_recdata_handle !");
         
	if(protocl_package_wait_handle_flag) 
        return;
//    I2C_SLAVE_LOG(" app_tws_recdata_handle %d !" , len);
//        for(uint32_t i=0; i<10; i++)
//        {
//            NRF_LOG_INFO(" %x " , p_data[i]);
//            
//        }
        
            
	while( index < len ){		
		switch( m_i2c_slave_msg_state )
		{
			case EPB_STEP_MAGIC:{	// magic 
				i2c_slave_recv_msg.magic = p_data[ index ++]; 
				if( i2c_slave_recv_msg.magic == MAGIC_NUMBER ){
					m_i2c_slave_msg_state = EPB_STEP_FLAG; 							
				}
				break; }
			case EPB_STEP_FLAG:{	// flag 
				i2c_slave_recv_msg.val = p_data[ index ++] ;

//				if( i2c_slave_recv_msg.flag.version != EPB_VER){
//					m_i2c_slave_msg_state = EPB_STEP_MAGIC;
//				}
//				else{
//					m_i2c_slave_msg_state = EPB_STEP_LEN_LSB; 
//				}	
                m_i2c_slave_msg_state = EPB_STEP_LEN_LSB; 
				break;}
			case EPB_STEP_LEN_LSB:{	// Len lsb 
				i2c_slave_recv_msg.len = p_data[ index ++];                 
				m_i2c_slave_msg_state = EPB_STEP_LEN_MSB; 
				break;}
			case EPB_STEP_LEN_MSB:{	// Len msb 
				i2c_slave_recv_msg.len |= (uint16_t)( p_data[ index ++ ] << 8);                
				if( i2c_slave_recv_msg.len > EPB_PAYLOAD_MAX_LEN || i2c_slave_recv_msg.len == 0){
					m_i2c_slave_msg_state = EPB_STEP_MAGIC ; 
				}
				else {
					m_i2c_slave_msg_state = EPB_STEP_CRC_LSB; 
				}
				break; }
			case EPB_STEP_CRC_LSB:{	// Crc lsb 
				i2c_slave_recv_msg.crc = p_data[ index ++ ]; 
				m_i2c_slave_msg_state = EPB_STEP_CRC_MSB; 
				break; }
			case EPB_STEP_CRC_MSB:{	// Crc msb 
				i2c_slave_recv_msg.crc |= (uint16_t)( p_data[ index ++ ] << 8) ; 
				m_i2c_slave_msg_state = EPB_STEP_TID_LSB; 
				break; }
			case EPB_STEP_TID_LSB:{	// Tid lsb 
				i2c_slave_recv_msg.id	= p_data[ index ++ ]; 
				m_i2c_slave_msg_state = EPB_STEP_TID_MSB; 
				break; }
			case EPB_STEP_TID_MSB:{	// Tid msb 
				i2c_slave_recv_msg.id	|= (uint16_t)( p_data[ index ++ ] << 8); 
				m_i2c_slave_msg_state = EPB_STEP_PAYLOAD; 
				m_bytes_received = 0; 
				
				i2c_slave_recv_msg.payload = (uint8_t*)APP_MALLOC( TCPIP_RECV_REQUEST_PAYLOAD, i2c_slave_recv_msg.len ); 
				if( i2c_slave_recv_msg.payload == NULL ){
					m_i2c_slave_msg_state = EPB_STEP_MAGIC; 
					break; 
				}
			}
				//fall through to copy payload 
			case EPB_STEP_PAYLOAD: {
				uint16_t remains = len - index; 	//remain bytes 
				if( ( remains + m_bytes_received ) < i2c_slave_recv_msg.len ){ // no enough bytes for an packet 
					memcpy( i2c_slave_recv_msg.payload + m_bytes_received, p_data + index, remains ); 
					index += remains; 
					m_bytes_received += remains; 
				}
				else{
					remains = i2c_slave_recv_msg.len - m_bytes_received; 
					memcpy( i2c_slave_recv_msg.payload + m_bytes_received, p_data + index, remains );
					index += remains; 
                    
                    protocl_package_wait_handle_flag=1;
                    set_app_evt(APP_EVT_PROTOCOL_DATA_HANDLE); //2019/06/26
					// one packet
//                    NRF_LOG_INFO(" APP_EVT_PROTOCOL_DATA_HANDLE !!");
//                    NRF_LOG_INFO(" app_i2c_protocol_data_process: %x !!", i2c_slave_recv_msg.len);    
//					protocol_data_receive_consume( &i2c_slave_recv_msg, NULL); 
//					
//					m_i2c_slave_msg_state = EPB_STEP_MAGIC; 
//					app_safe_free( i2c_slave_recv_msg.payload ); 
//					i2c_slave_recv_msg.payload = NULL; 

				}				
				break;}
			
			default:
				m_i2c_slave_msg_state = EPB_STEP_MAGIC; 
				break; 
		}
	}	
}


void app_i2c_protocol_data_process(void)
{
    if(protocl_package_wait_handle_flag)
    {
        protocl_package_wait_handle_flag = false;
        protocol_data_receive_consume( &i2c_slave_recv_msg, NULL);
        I2C_SLAVE_LOG(" recieved a package len: %x !!", i2c_slave_recv_msg.len);      
        m_i2c_slave_msg_state = EPB_STEP_MAGIC; 
        app_safe_free( i2c_slave_recv_msg.payload ); 
        i2c_slave_recv_msg.payload = NULL; 
    }
    
}













