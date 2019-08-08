

#ifndef DFU_RES_H__
#define DFU_RES_H__


typedef enum{
	DFU_RES_SUCCESS = 0, 
    
  DFU_RES_ERROR_INVALID_VERSION = 0x11, //协议版本不支持
 DFU_RES_ERROR_INVALID_ENCRYPTION, //加密方式不支持
  DFU_RES_ERROR_LENGTH, //协议长度错误
    DFU_RES_ERROR_CRC,  //校验错误
  DFU_RES_ERROR_INVALID_CMD, //命令不支持
  DFU_RES_ERROR_INVALID_KEY,
  DFU_RES_ERROR_INVALID_KEY_LENGTH,
    
	DFU_RES_ERROR_INVALID_FORMAT=0x21,   //chen 2019/07/03
    DFU_RES_ERROR_DATA_SIZE,
    DFU_RES_ERROR_STATE, 
    DFU_RES_ERROR_INVALID_PARAM,
	DFU_RES_ERROR_NO_MEM,
    
  DFU_RES_ERROR_SUB_FUNCTION_NOT_SUPPORTED, //功能不支持
  DFU_RES_ERROR_GPS_IS_NOT_READY, //GPS无定位	 
	DFU_RES_ERROR_ADDR, 
	
		
	DFU_RES_UNKNOW_STATE, 
    DFU_RES_ERROR_BATTERY_POWER_LOW = 0XF0,
    
}dfu_res_ext_code_t;

/*
typedef enum {
  EPB_SUCCESS = 0,

  //协议错误
  EPB_ERR_INVALID_VERSION = 0x11, //协议版本不支持
  EPB_ERR_INVALID_ENCRYPTION, //加密方式不支持
  EPB_ERR_LENGTH, //协议长度错误
  EPB_ERR_CHECK_SUM, //校验错误
  EPB_ERR_INVALID_CMD, //命令不支持
  EPB_ERR_INVALID_KEY,
  EPB_ERR_INVALID_KEY_LENGTH,



  EPB_ERR_INVALID_FORMAT = 0x21, //数据越界
  EPB_ERR_DATA_SIZE_ERROR, //数据字节长度错误。
  EPB_ERR_INVALID_STATE, //状态错误
  EPB_ERR_INVALID_PARAMETER, //参数错误
  EPB_ERR_NO_MEM, //内存不足

  EPB_ERR_SUB_FUNCTION_NOT_SUPPORTED, //功能不支持
  EPB_ERR_GPS_IS_NOT_READY, //GPS无定位
  EPB_ERR_ADDR_RESP, //地址错误

  EPB_BATTERY_POWER_LOW = 0XF0,

}epb_errcode_t;

*/


#endif 
