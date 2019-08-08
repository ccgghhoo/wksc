/**
*
*
*
**/

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "nrf_gpio.h"
#include "app_timer.h"
#include "app_util_platform.h"
#include "bsp.h"
#include "app_utc_time.h"
#include "app_evt.h"


APP_TIMER_DEF(m_timer_second);

utc_time_t    m_user_time_utc =  //2000-1-1-00:00
                            {
                                .second =0,
                                .minute=0,
                                .hour = 0,
                                .day=6,
                                .month=6,
                                .year = 2019,
                                .weekday=4,
                            };

uint32_t  m_user_time_senconds;

                
static void sencond_time_out_handler(void * p_context)
{
		m_user_time_senconds++;
        set_app_evt(APP_EVT_1S);
		//app_seconds_to_utc(&m_user_time_utc ,  m_user_time_senconds );//
		if(m_user_time_utc.second<59)
		{
			m_user_time_utc.second++;
		}
		else 
		{
			m_user_time_utc.second=0;
			if(m_user_time_utc.minute<59)
			{
				m_user_time_utc.minute++;
			}
			else 
			{
				m_user_time_utc.minute=0;
				if(m_user_time_utc.hour<23)
				{
					m_user_time_utc.hour++;
				}
				else 
				{
					m_user_time_utc.hour=0;
					app_seconds_to_utc(&m_user_time_utc ,  m_user_time_senconds );	// once one  day
				}
			}	
							
		}
	
}

void  UTC_timer_init(void)
{
	ret_code_t err_code;
	err_code = app_timer_create(&m_timer_second, APP_TIMER_MODE_REPEATED, sencond_time_out_handler);
    APP_ERROR_CHECK(err_code);
    err_code = app_timer_start(m_timer_second, APP_TIMER_TICKS(1000), NULL); //onesecond
    APP_ERROR_CHECK(err_code);
	
	m_user_time_senconds = app_utc_to_seconds(&m_user_time_utc);
		
}



/*********************************************************************
 * @fn      monthLength
 *
 * @param   lpyr - 1 for leap year, 0 if not
 *
 * @param   mon - 0 - 11 (jan - dec)
 *
 * @return  number of days in specified month
 */
static uint8_t monthLength( uint8_t lpyr, uint8_t mon )
{
  uint8_t days = 31;

  if ( mon == 1 ) // feb
  {
    days = ( 28 + lpyr );
  }
  else
  {
    if ( mon > 6 ) // aug-dec
    {
      mon--;
    }

    if ( mon & 1 )
    {
      days = 30;
    }
  }

  return ( days );
}

/*******************************************************
*UTC time   ---->  seconds
*
*******************************************************/
uint32_t   app_utc_to_seconds( utc_time_t *tm )
{
  uint32_t seconds;

  /* Seconds for the partial day */
  seconds = (((tm->hour * 60UL) + tm->minute) * 60UL) + tm->second;

  /* Account for previous complete days */
  {
    /* Start with complete days in current month */
    uint16_t days = tm->day;

    /* Next, complete months in current year */
    {
      int8_t month = tm->month;
      while ( --month >= 0 )
      {
        days += monthLength( IsLeapYear( tm->year ), month );
      }
    }

    /* Next, complete years before current year */
    {
      uint16_t year = tm->year;
      while ( --year >= BEGYEAR )
      {
        days += YearLength( year );
      }
    }
    /* Add total seconds before partial day */
    seconds += (days * DAY);
  }
  return ( seconds );
}

/*********************************************************
*
*  seconds ----> UTC  time
*
**********************************************************/

void app_seconds_to_utc( utc_time_t *tm,  uint32_t  tm_seconds )
{
  // calculate the time less than a day - hours, minutes, seconds
  {
    uint32_t day = tm_seconds % DAY;
    tm->second = day % 60UL;
    tm->minute = (day % 3600UL) / 60UL;
    tm->hour = day / 3600UL;
  }
  uint32_t weeks=tm_seconds /DAY;
  tm->weekday = (((weeks %7) + 6) % 7);
  // Fill in the calendar - day, month, year
  {
    uint16_t numDays = tm_seconds / DAY;
    tm->year = BEGYEAR;
    while ( numDays >= YearLength( tm->year ) )
    {
      numDays -= YearLength( tm->year );
      tm->year++;
    }

    tm->month = 0;
    while ( numDays >= monthLength( IsLeapYear( tm->year ), tm->month ) )
    {
      numDays -= monthLength( IsLeapYear( tm->year ), tm->month );
      tm->month++;
    }

    tm->day = numDays;
  }
}

uint32_t  app_utc_get_current_time_senconds(void)
{
    return  m_user_time_senconds;
}






















