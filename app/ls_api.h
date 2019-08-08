

#ifndef LOC_MAPPING_INTERFACE_H__
#define LOC_MAPPING_INTERFACE_H__

#include <stdint.h>
#include <stdbool.h>

#include "ls.code.h"
#include "gsm.code.h"

/*
conf 
*/
void        ls_api_conf_mode_update(uint8_t mode, uint32_t time);
//void        ls_api_conf_volume_mic_update(void);
//void        ls_api_conf_volume_speaker_update(void);
//void        ls_api_conf_volume_ringtone_update(void);

/*
event 
*/
bool        ls_api_button_click_evt(uint8_t index_of_button);
bool        ls_api_on_calling();
//bool        ls_api_event_consumed_buttons(uint32_t evt);
void        ls_api_event_input(ls_evt_accept_t evt);

void        ls_api_event_alarm_activate(void);
void        ls_api_event_alarm_deactivate(void);
/*
state or data 
*/
bool        ls_api_get_has_valid_gps_for_upload(void);
bool        ls_api_get_has_valid_gps(); 
uint32_t    ls_api_get_final_locate_packet(uint8_t *pdata);
bool        ls_api_get_final_lat_lng(int32_t *lat,int32_t *lng );

uint32_t    ls_api_get_cell_locator_data(uint8_t *p_data);
uint8_t     ls_api_get_gsm_csq(void);
void        ls_api_set_mileage(uint32_t mileage);

/*
exec
*/
void        ls_api_init(void);
void        ls_api_start(void);
void        ls_api_stop(void);
void        ls_api_exec(void);
bool        ls_api_gsm_idle(void);
bool        ls_api_idle(void);


//
uint32_t ls_api_get_gsm_state(void);
uint32_t ls_api_get_gsm_task(void);


void ls_api_gsm_reconnect(void);
bool ls_api_gsm_is_registered(void);
bool ls_api_gsm_is_simcard(void);


//ble loc

void ls_api_set_ble_loc(uint8_t mac[6], int32_t lat, int32_t lon,char *address);
#endif
