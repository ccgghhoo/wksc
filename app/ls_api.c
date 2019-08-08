

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

/**@brief 更新 GSM/GPS工作模式
 * @param mode 模式
 * @param time 相关时间参数
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
/**@brief 按键事件更新，检查是否需要消耗
 * @param null
 * @param null
 * @return
 * @return
*/
__INLINE bool ls_api_button_click_evt(uint8_t index_of_button)
{
  return locate_on_button_click_evt(index_of_button);
}

/**@brief 按键事件更新，检查是否需要消耗
 * @param null
 * @param null
 * @return
 * @return
*/
__INLINE bool ls_api_on_calling()
{
  return locate_on_calling();
}


/**@brief 触发 GSM/GPS 事件
 * @param evt 定位服务接收的事件类型
 * @param null
 * @return
 * @return
*/
__INLINE void ls_api_event_input(ls_evt_accept_t evt)
{
  locate_on_evt_handler(evt);
}
/**@brief 播放GSM模块中的铃声
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
/**@brief 关闭GSM模块播放的铃声
 * @param null
 * @param null
 * @return
 * @return
*/
__INLINE void ls_api_event_alarm_deactivate(void)
{
 // (void)gsm_task_setup_alarmclock(false);
}

/**@brief 检查GPS是否存在有效的位置信息用于上传
 * @param null
 * @param null
* @return true 最后时间段里有数据可用于上传
* @return false 没有最近数据用于上传
*/
__INLINE bool ls_api_get_has_valid_gps_for_upload(void)
{
  return gps_data_has_location_in_final_time();
}

/**@brief 检查GPS是否存在有效的位置信息用于上传
 * @param null
 * @param null
* @return true 最后时间段里有数据可用于上传
* @return false 没有最近数据用于上传
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
/**@brief 定位服务初始化
 * @param null
 * @param null
 * @return
 * @return
*/
__INLINE void ls_api_init(void)
{
  locate_services_init();
}
/**@brief 启动定位服务
 * @param null
 * @param null
 * @return
 * @return
*/
__INLINE void ls_api_start(void)
{
  locate_services_start();
}

/**@brief 停止
 * @param null
 * @param null
 * @return
 * @return
*/
__INLINE void ls_api_stop(void)
{
  locate_services_stop();
}

/**@brief 定位服务运行
 * @param null
 * @param null
 * @return
 * @return
*/
__INLINE void ls_api_exec(void)
{
  locate_services_execution();
}
/**@brief 查询GSM是否处于空闲模式
 * @param null
 * @param null
 * @return
 * @return
*/
__INLINE bool ls_api_gsm_idle(void)
{
  return gsm_is_idle_state();
}


/**@brief 查询 定位服务是否 未在工作
 * @param null
 * @param null
 * @return true 空闲状态
 * @return false GSM/GPS至少有一个在工作
*/
__INLINE bool ls_api_idle(void)
{
  return loc_sm_idle();
}

/**@brief 获取GSM的状态
 * @param null
 * @param null
 * @return
 * @return
*/
__INLINE uint32_t ls_api_get_gsm_state(void)
{
  return gsm_state();
}

/**@brief 获取GSM task的状态
 * @param null
 * @param null
 * @return
 * @return
*/
__INLINE uint32_t ls_api_get_gsm_task(void)
{
  return  gsm_task_get();
}


/**@brief 获取GSM task的状态
 * @param null
 * @param null
 * @return
 * @return
*/
__INLINE void ls_api_gsm_reconnect(void)
{
    gsm_tcp_reconnect();
}


/**@brief 获取GSM 是否注册
 * @param null
 * @param null
 * @return
 * @return
*/
__INLINE bool ls_api_gsm_is_registered(void)
{
   return gsm_is_registered();
}




/**@brief 获取GSM 插卡
 * @param null
 * @param null
 * @return
 * @return
*/
__INLINE bool ls_api_gsm_is_simcard(void)
{
   return gsm_is_simcard();
}


/**@brief 设置ble loc
 * @param null
 * @param null
 * @return
 * @return
*/
__INLINE void ls_api_set_ble_loc(uint8_t mac[6],int32_t lat,int32_t lon,char *address)
{
   loc_ble_set(mac,lat,lon,address);
}


