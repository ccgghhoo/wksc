

#ifndef PROTOCOL_H__
#define PROTOCOL_H__

#include <stdint.h>
#include <string.h>

#include "proto.pb.h"


typedef void (*resp_callback_t)(uint8_t *data, uint16_t len); 

void protocol_data_receive_consume(msg_packet_t * msg, resp_callback_t evt_handle); 

#endif 


