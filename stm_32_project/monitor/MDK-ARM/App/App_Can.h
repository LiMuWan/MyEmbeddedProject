#ifndef __APP_CAN_H__
#define __APP_CAN_H__
#include "Int_Can.h"
#include <stdint.h>
#include <string.h>
#include "App_Motor_Value.h"
#include "App_Motor_PID.h"
//初始化CAN
void App_Can_init(void);
//电机节点需要持续判断是否接受到消息
void App_Can_Poll(void);

#endif /* __APP_CAN_H__ */