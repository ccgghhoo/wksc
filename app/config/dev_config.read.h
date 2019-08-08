

#ifndef DEV_CONF_READ_H__
#define DEV_CONF_READ_H__

#include <stdint.h>

uint16_t dev_config_read_max_size(void); 

uint16_t dev_config_read( const uint8_t * p_data, uint8_t len, uint8_t * p_read_out, uint16_t buff_max_size ); 

#endif 

