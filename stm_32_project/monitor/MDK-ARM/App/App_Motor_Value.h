#ifndef __APP_MOTOR_VALUE_H__
#define __APP_MOTOR_VALUE_H__
#include <stdint.h>
#define ANGLE 360
#define SECOND 60
#define CYCLE_TURN 3200
#include "port.h"

/*****************电机角度**************************/
// 获取与设置 电机的目标角度
void App_Motor_SetTargetAngle(float newTargetAngle);
float App_Motor_GetTargetAngle(void);

// 获取与设置 电机的当前角度
void App_Motor_SetCurrentAngle(float newCurrentAngle);
float App_Motor_GetCurrentAngle(void);

/*********************电机速度*********************/
// 获取与设置电机的最大速度
void App_Motor_SetMaxSpeedRPM(float newMaxSpeedRPM);
float App_Motor_GetMaxSpeedRPM(void);
// 获取与设置电机的当前速度
void App_Motor_SetCurrentSpeedRPM(float newCurrentSpeedRPM);
float App_Motor_GetCurrentSpeedRPM(void);

// 获取与设置电机的最小速度
void App_Motor_SetMinSpeedRPM(float newMinSpeedRPM);
float App_Motor_GetMinSpeedRPM(void);
// 获取与设置电机的加速度
void App_Motor_SetAccSpeedRPM(float newAccSpeedRPM);
float App_Motor_GetAccSpeedRPM(void);

/*****************电机角度周期*******************************/
float App_Motor_GetTargetAngleCycle(void);
void App_Motor_SetTargetAngleCycle(float targetAngleCycle);
float App_Motor_getCurrentAngleCycle(void);
void App_Motor_SetCurrentAngleCycle(float currentAngleCycle);

/****************电机速度周期*********************************************/
float App_Motor_GetMaxSpeedRPMCycle(void);
void App_Motor_SetMaxSpeedRPMCycle(float MaxSpeedRPMCycle);
float App_Motor_GetCurrentSpeedRPMCycle(void);
void App_Motor_SetCurrentSpeedRPMCycle(float currentSpeedRPMCycle);
/********************ARR自动重装载需要的是us******************************************/
uint16_t App_Motor_GetUS(void);
#endif /* __APP_MOTOR_VALUE_H__ */

// 电机数值管理模块:管理角度、速度
