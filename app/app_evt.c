#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "nordic_common.h"
#include "nrf.h"
#include "app_evt.h"



uint32_t  m_app_evt=0;

void clear_app_evt(uint32_t evt)
{
    m_app_evt&=~evt;
}

void set_app_evt(uint32_t evt)
{
    m_app_evt|=evt;
}

bool check_app_evt(uint32_t evt)
{
    return (m_app_evt&evt)? 1:0;
}

//end of  file