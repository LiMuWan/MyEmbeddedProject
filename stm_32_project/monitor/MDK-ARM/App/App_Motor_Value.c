#include "App_Motor_Value.h"
/**
   电机数值管理:
             1__旋转目标角度    (角度与速度都可以是小数)
             2——当前角度
             3__当前电机最大速度
             2——当前电机的速度
 **/
// 电机的角度数值
// static float target_angle = 360;   // 目标角度
// static float current_angle = 0;    // 当前角度
// static float max_SpeedRPM = 60;    // RPM[Revolutions Per Minute:每分钟转多少圈,当前57电机上线200RPM]。当前参数表示1圈/s
// static float current_SpeedRPM = 0; // 电机的当前速度

// 电机旋转的角度、速度,本质需要通过定时器的溢出周期个数决定
// 角度分为正负
static float target_angle_cycle = 3200 * 4; // 1080° = 3圈 = 3200 * 3
static float current_angle_cycle = 0;
// 速度不分正负
// 最大速度
static float max_SpeedRPM_CPS = 3200; // 表示含义:表示的速度位1s转一圈
// 最小速度
static float min_SpeedRPM_CPS = 800;
// 加速度
static float acc_SpeedRPM_CPS = 600;

static float current_SpeedRPM_CPS = 0; // 当前速度:0

/***********周期换算位角度********************/
static float cycle_to_angle(float cycle)
{
    return cycle / CYCLE_TURN * ANGLE;
}

static float angle_to_cycle(float angle)
{
    return angle / ANGLE * CYCLE_TURN;
}

/**********周期换算速度********************************/
static float speedRPM_to_Cycle(float speedRPM)
{
    return speedRPM / SECOND * CYCLE_TURN;
}
static float cycle_to_SpeedRPM(float cycle)
{

    return cycle / CYCLE_TURN * SECOND;
}

/*****************电机角度**************************/
// 获取与设置 电机的目标角度
void App_Motor_SetTargetAngle(float newTargetAngle)
{
    target_angle_cycle = angle_to_cycle(newTargetAngle);
}
float App_Motor_GetTargetAngle(void)
{
    return cycle_to_angle(target_angle_cycle);
}

// 获取与设置 电机的当前角度
void App_Motor_SetCurrentAngle(float newCurrentAngle)
{
    current_angle_cycle = angle_to_cycle(newCurrentAngle);
}
float App_Motor_GetCurrentAngle(void)
{
    return cycle_to_angle(current_angle_cycle);
}

/*********************电机速度*********************/
// 获取与设置电机的最大速度
void App_Motor_SetMaxSpeedRPM(float newMaxSpeedRPM)
{
    max_SpeedRPM_CPS = speedRPM_to_Cycle(newMaxSpeedRPM);
}
float App_Motor_GetMaxSpeedRPM(void)
{
    return cycle_to_SpeedRPM(max_SpeedRPM_CPS);
}

// 获取与设置电机的当前速度
void App_Motor_SetCurrentSpeedRPM(float newCurrentSpeedRPM)
{
    current_SpeedRPM_CPS = speedRPM_to_Cycle(newCurrentSpeedRPM);
}
float App_Motor_GetCurrentSpeedRPM(void)
{
    return cycle_to_SpeedRPM(current_SpeedRPM_CPS);
}

// 获取与设置电机的最小速度
void App_Motor_SetMinSpeedRPM(float newMinSpeedRPM)
{
    min_SpeedRPM_CPS = newMinSpeedRPM;
}
float App_Motor_GetMinSpeedRPM(void)
{
    return min_SpeedRPM_CPS;
}
// 获取与设置电机的加速度
void App_Motor_SetAccSpeedRPM(float newAccSpeedRPM)
{
    acc_SpeedRPM_CPS = newAccSpeedRPM;
}
float App_Motor_GetAccSpeedRPM(void)
{
    return acc_SpeedRPM_CPS;
}

/********************周期操作*********************/
/*****************电机角度周期*******************************/
float App_Motor_GetTargetAngleCycle(void)
{
    return target_angle_cycle;
}
void App_Motor_SetTargetAngleCycle(float targetAngleCycle)
{
    target_angle_cycle = targetAngleCycle;
}
float App_Motor_getCurrentAngleCycle(void)
{
    return current_angle_cycle;
}
void App_Motor_SetCurrentAngleCycle(float currentAngleCycle)
{
    current_angle_cycle = currentAngleCycle;

    // 设置当前角度
    Int_ModBus_SetCurrentAngle(App_Motor_GetCurrentAngle());
}

/****************电机速度周期*********************************************/
float App_Motor_GetMaxSpeedRPMCycle(void)
{
    return max_SpeedRPM_CPS;
}
void App_Motor_SetMaxSpeedRPMCycle(float MaxSpeedRPMCycle)
{
    max_SpeedRPM_CPS = MaxSpeedRPMCycle;
}
float App_Motor_GetCurrentSpeedRPMCycle(void)
{
    return current_SpeedRPM_CPS;
}
void App_Motor_SetCurrentSpeedRPMCycle(float currentSpeedRPMCycle)
{

    current_SpeedRPM_CPS = currentSpeedRPMCycle;
}
/***********************ARR:当前速度周期换算ARR数值,单位us***************************************************/
uint16_t App_Motor_GetUS(void)
{
    return (1 / current_SpeedRPM_CPS) * 1000000;
}
