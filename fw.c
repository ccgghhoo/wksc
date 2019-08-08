////*********************************************

/////////////////////////////////////////////////////

#include "fw.h"

const app_info_t  appl_info            __attribute__((at((uint32_t)APP_FW_INFO_START_ADDRESS))) 
   = 
{
	.magic = APP_FW_MAGIC_CODE, 
	.fw_version = FW_REVISION, 
	.fw_size = APP_FW_SIZE,  
	.hw_version = APP_FW_HW_VER,
	.sd_version = APP_FW_SD_VERSION, 
	.sd_fw_id = APP_FW_SD_FW_ID,
	.date = APP_FW_DATE,
	.time = APP_FW_TIME, 
}; 





