#ifndef __APP_MODBUS_H__
#define __APP_MODBUS_H__
#include <stdint.h>
#include <string.h>
#include "App_Motor_Value.h"
#include "App_Motor_PID.h"

void Int_ModBus_GetREG_HOLD_BUF(uint16_t buffers[]);

#endif /* __APP_MODBUS_H__ */
