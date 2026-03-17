#ifndef __APP_DISPLAY_H__
#define __APP_DISPLAY_H__
#include "Int_OLED.h"
#include "App_Motor_Value.h"
#include "App_ModBusID.h"
#include <stdio.h>
// 1.应用层控制OLED模块
void App_Display_Init(void);
// 2.切换OLED页面的方法
void App_Display_ShowPage(void);
//3.更新页面标识
void App_Display_UpdatePageNo(void);

//4.获取当前oled页面编号
uint8_t App_DisPlay_GetPageNo(void);

#endif /* __APP_DISPLAY_H__ */
