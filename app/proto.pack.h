

#ifndef DEV_DATA_PACK_H__
#define DEV_DATA_PACK_H__

#include <stdint.h>
#include <stdbool.h>

#include "app_util.h"

extern bool m_resp_sent; 


void __data_pack(uint8_t *data, uint16_t len, uint8_t flag); 
uint8_t * __send_pack_msg(uint8_t *data, uint16_t len, uint8_t flag); //chen

void __ack_resp(void); 
void __negative_resp(uint8_t code); 

bool proto_pack_gps_latest_data( void );
bool proto_pack_historical_data(void);

void proto_pack_memory_reader_process( uint8_t field_type, const uint8_t * p_data, uint16_t len ); 
void proto_pack_dfu_process(uint8_t field_type, const uint8_t * p_data, uint16_t len );

// GSM active data 
void proto_pack_get_heart_beat_msg( uint8_array_t * targ );
void proto_pack_get_gps_curr_data_msg( uint8_array_t * targ ); 
void proto_pack_get_alert_msg( uint8_array_t * targ );
void proto_pack_req_server_parse_lat_long(uint8_array_t * targ);
void proto_pack_get_historical_data_msg( uint8_array_t * targ );

void long_lan_parsed_from_server(const uint8_t* addr, uint16_t len);
void connect_to_server(void); 


#endif 
