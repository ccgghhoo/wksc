#ifndef _APP_POWER_OFF_H_
#define _APP_POWER_OFF_H_


#define   PWR_OFF_WAKE_UP_LEVEL   1
#define   PWR_OFF_LEVEL           (!PWR_OFF_WAKE_UP_LEVEL)
#define   PWR_OFF_WAKE_UP_PIN     10



void power_off_pin_config(void);
void power_off_pin_check_when_reset(void);

#endif


