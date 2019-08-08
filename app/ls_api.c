

#include "ls_api.h"


#include "gsflag.h"
#include "GSM/loc_gsm.h"
#include "GPS/loc_gps.h"
#include "BLE/loc_ble.h"

#include "loc.mode.h"
#include "StateManagement/loc.sm.h"
#include "location.h"

#include "GSM/cell_tower_locator.h"
#include "dev_config.get.h"

/**@brief ���� GSM/GPS����ģʽ
 * @param mode ģʽ
 * @param time ���ʱ�����
 * @return
 * @return
*/
__INLINE void ls_api_conf_mode_update(uint8_t mode, uint32_t time)
{
  loc_mode_update(mode, time);
}


//__INLINE bool ls_api_event_consumed_buttons(uint32_t evt)
//{
//    return false;
//}
/**@brief �����¼����£�����Ƿ���Ҫ����
 * @param null
 * @param null
 * @return
 * @return
*/
__INLINE bool ls_api_button_click_evt(uint8_t index_of_button)
{
  return locate_on_button_click_evt(index_of_button);
}

/**@brief �����¼����£�����Ƿ���Ҫ����
 * @param null
 * @param null
 * @return
 * @return
*/
__INLINE bool ls_api_on_calling()
{
  return locate_on_calling();
}


/**@brief ���� GSM/GPS �¼�
 * @param evt ��λ������յ��¼�����
 * @param null
 * @return
 * @return
*/
__INLINE void ls_api_event_input(ls_evt_accept_t evt)
{
  locate_on_evt_handler(evt);
}
/**@brief ����GSMģ���е�����
 * @param null
 * @param null
 * @return
 * @return
*/
__INLINE void ls_api_event_alarm_activate(void)
{
    
  locate_on_evt_handler(LS_EVT_ALARM_CLOCK_START);
 // (void)gsm_task_setup_alarmclock(true);
}
/**@brief �ر�GSMģ�鲥�ŵ�����
 * @param null
 * @param null
 * @return
 * @return
*/
__INLINE void ls_api_event_alarm_deactivate(void)
{
 // (void)gsm_task_setup_alarmclock(false);
}

/**@brief ���GPS�Ƿ������Ч��λ����Ϣ�����ϴ�
 * @param null
 * @param null
* @return true ���ʱ����������ݿ������ϴ�
* @return false û��������������ϴ�
*/
__INLINE bool ls_api_get_has_valid_gps_for_upload(void)
{
  return gps_data_has_location_in_final_time();
}

/**@brief ���GPS�Ƿ������Ч��λ����Ϣ�����ϴ�
 * @param null
 * @param null
* @return true ���ʱ����������ݿ������ϴ�
* @return false û��������������ϴ�
*/
__INLINE bool ls_api_get_has_valid_gps()
{
  return gps_data_has_valid_gps();
}

/**@brief
 * @param null
 * @param null
 * @return
 * @return
*/
__INLINE bool ls_api_data_has_gps_lastest(void)
{
  return gps_data_has_location();
}

__INLINE uint32_t ls_api_get_final_locate_packet(uint8_t *pdata)
{
  return gps_data_final_point_raw_value_reprintf(pdata);
}
__INLINE bool ls_api_get_final_lat_lng(int32_t *lat,int32_t *lng )
{
  return gps_data_final_point(lat,lng);
}




__INLINE uint32_t ls_api_get_cell_locator_data(uint8_t *p_data)
{
  return ctls_get_raw_data(p_data);
}
__INLINE uint8_t ls_api_get_gsm_csq(void)
{
  return gsm_get_val_csq();
}

__INLINE void ls_api_set_mileage(uint32_t mileage)
{
  gps_mileage_set(mileage);
}

// execute
/**@brief ��λ�����ʼ��
 * @param null
 * @param null
 * @return
 * @return
*/
__INLINE void ls_api_init(void)
{
  locate_services_init();
}
/**@brief ������λ����
 * @param null
 * @param null
 * @return
 * @return
*/
__INLINE void ls_api_start(void)
{
  locate_services_start();
}

/**@brief ֹͣ
 * @param null
 * @param null
 * @return
 * @return
*/
__INLINE void ls_api_stop(void)
{
  locate_services_stop();
}

/**@brief ��λ��������
 * @param null
 * @param null
 * @return
 * @return
*/
__INLINE void ls_api_exec(void)
{
  locate_services_execution();
}
/**@brief ��ѯGSM�Ƿ��ڿ���ģʽ
 * @param null
 * @param null
 * @return
 * @return
*/
__INLINE bool ls_api_gsm_idle(void)
{
  return gsm_is_idle_state();
}


/**@brief ��ѯ ��λ�����Ƿ� δ�ڹ���
 * @param null
 * @param null
 * @return true ����״̬
 * @return false GSM/GPS������һ���ڹ���
*/
__INLINE bool ls_api_idle(void)
{
  return loc_sm_idle();
}

/**@brief ��ȡGSM��״̬
 * @param null
 * @param null
 * @return
 * @return
*/
__INLINE uint32_t ls_api_get_gsm_state(void)
{
  return gsm_state();
}

/**@brief ��ȡGSM task��״̬
 * @param null
 * @param null
 * @return
 * @return
*/
__INLINE uint32_t ls_api_get_gsm_task(void)
{
  return  gsm_task_get();
}


/**@brief ��ȡGSM task��״̬
 * @param null
 * @param null
 * @return
 * @return
*/
__INLINE void ls_api_gsm_reconnect(void)
{
    gsm_tcp_reconnect();
}


/**@brief ��ȡGSM �Ƿ�ע��
 * @param null
 * @param null
 * @return
 * @return
*/
__INLINE bool ls_api_gsm_is_registered(void)
{
   return gsm_is_registered();
}




/**@brief ��ȡGSM �忨
 * @param null
 * @param null
 * @return
 * @return
*/
__INLINE bool ls_api_gsm_is_simcard(void)
{
   return gsm_is_simcard();
}


/**@brief ����ble loc
 * @param null
 * @param null
 * @return
 * @return
*/
__INLINE void ls_api_set_ble_loc(uint8_t mac[6],int32_t lat,int32_t lon,char *address)
{
   loc_ble_set(mac,lat,lon,address);
}


