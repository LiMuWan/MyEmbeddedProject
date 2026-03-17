#ifndef __APP_MODBUSID_H__
#define __APP_MODBUSID_H__
#include "Int_M24C02.h"
#include <string.h>

// 1.初始化方法
void App_MosBusID_Init(void);

//2.设置ID
void App_SetModBusID(uint8_t newID);

//3.获取ID
uint8_t App_GetModBusId(void);
#endif /* __APP_MODBUSID_H__ */
