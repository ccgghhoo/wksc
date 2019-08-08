/**
 *
 */
#ifndef __APP_UTC_TIME_H__
#define __APP_UTC_TIME_H__

#include "sdk_errors.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
  uint8_t second;    // 0-59
  uint8_t minute;   // 0-59
  uint8_t hour;        // 0-23
  uint8_t day;          // 0-30
  uint8_t month;     // 0-11
  uint16_t year;       // 2000+
  uint8_t weekday;
} utc_time_t;





#define BUILD_UINT16(loByte, hiByte)	((uint16_t)(((loByte) & 0x00FF) + (((hiByte) & 0x00FF) << 8)))
#define	IsLeapYear(yr)	                (!((yr) % 400) || (((yr) % 100) && !((yr) % 4)))
#define	YearLength(yr)	                (IsLeapYear(yr) ? 366 : 365)
// (MAXCALCTICKS * 5) + (max remainder) must be <= (uint16 max),
// so: (13105 * 5) + 7 <= 65535
#define MAXCALCTICKS    ((uint16)(13105))
#define	BEGYEAR	          2000        // UTC started at 00:00:00 January 1, 2000
#define	DAY                   86400UL  // 24 hours * 60 minutes * 60 seconds


extern  utc_time_t    m_user_time_utc;
extern  uint32_t      m_user_time_senconds;


uint32_t   app_utc_to_seconds( utc_time_t *tm );
void app_seconds_to_utc( utc_time_t *tm,  uint32_t  tm_seconds );
void  UTC_timer_init(void);
uint32_t app_utc_get_current_time_senconds(void);

#ifdef __cplusplus
}
#endif

#endif // EEPROM_SIMULATOR_H__
