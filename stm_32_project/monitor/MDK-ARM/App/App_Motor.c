#include "App_Motor.h"

//引入电机运行状态
extern uint8_t motor_run_status = 0;
// 存储加速阶段的周期个数
float acc_cycle = 0;
// 0是三角形 1梯形
uint8_t flag = 0;
// 设置ARR寄存器，修改速度
static void Calc_SetARR(void);
// 设置加速阶段的周期个数
static void Calc_AccCycle(void);
// 计算出电机的下一步的速度周期个数
static void NextSpeed(void);

void App_Motor_Start(void)
{
    // 1.设置转动方向
    if (App_Motor_GetTargetAngle() > 0)
    {
        HAL_GPIO_WritePin(MOTOR_DIR_GPIO_Port, MOTOR_DIR_Pin, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(MOTOR_DIR_GPIO_Port, MOTOR_DIR_Pin, GPIO_PIN_SET);
    }
    // 2.计算速度，本质是设置重装载的值
    Calc_SetARR();
    // 3.电机启动判断图形是三角形还是梯形
    Calc_AccCycle();
    // 4.启动电机
    HAL_GPIO_WritePin(MOTOR_EN_GPIO_Port, MOTOR_EN_Pin, GPIO_PIN_SET);
    // 启动定时器
    HAL_TIM_OC_Start_IT(&htim1, TIM_CHANNEL_1);

    // 启用电机解码器
    Int_EnableEncoder();

    Int_ModBus_SetMotorStatus(1);

    motor_run_status = 1;
}

void App_Motor_Stop(void)
{
    // 清除数据
    App_Motor_SetCurrentAngleCycle(0);
    App_Motor_SetCurrentSpeedRPMCycle(0);
    // 关闭电机使能
    HAL_TIM_OC_Stop_IT(&htim1, TIM_CHANNEL_1);
    // 禁用电机解码器
    Int_DisableEncoder();

    Int_ModBus_SetMotorStatus(0);

    motor_run_status = 0;
}

static void Calc_SetARR(void)
{
    // 设置为最小速度
    App_Motor_SetCurrentSpeedRPMCycle(App_Motor_GetMinSpeedRPM());
    // 设置速度本质就是设置TIM的ARR
    __HAL_TIM_SetAutoreload(&htim1, App_Motor_GetUS());
}

// 计算加速阶段的周期个数，判断是T形还是三角形
static void Calc_AccCycle(void)
{
    // V1²-V0² = 2as
    // s = (V1²-V0²)/(2a)
    float result = (App_Motor_GetMaxSpeedRPMCycle() * App_Motor_GetMaxSpeedRPMCycle() -
                    App_Motor_GetCurrentSpeedRPMCycle() * App_Motor_GetCurrentSpeedRPMCycle()) /
                   (App_Motor_GetAccSpeedRPM() * 2);
    if (result >= abs(App_Motor_GetTargetAngleCycle() / 2))
    {
        // 三角形
        acc_cycle = (abs(App_Motor_GetTargetAngleCycle()) / 2);
    }
    else
    {
        // 梯形
        acc_cycle = result;
        flag = 1;
    }
}

static void NextSpeed(void)
{
    // v1² = v0² + 2as
    float v1;
    // 加速阶段
    // 当前角度周期个数小于加速阶段周期个数 加速
    if (App_Motor_getCurrentAngleCycle() < acc_cycle)
    {
        v1 = sqrt(App_Motor_GetCurrentSpeedRPMCycle() * App_Motor_GetCurrentSpeedRPMCycle() +
                  2 * App_Motor_GetAccSpeedRPM() * 1);
    }
    // 匀速阶段
    else if (App_Motor_getCurrentAngleCycle() >= acc_cycle && App_Motor_getCurrentAngleCycle() < App_Motor_GetTargetAngleCycle() - acc_cycle && flag)
    {
        v1 = App_Motor_GetMaxSpeedRPMCycle();
    }
    else
    {
        // 减速阶段
        v1 = sqrt(App_Motor_GetCurrentSpeedRPMCycle() * App_Motor_GetCurrentSpeedRPMCycle() -
                  2 * App_Motor_GetAccSpeedRPM() * 1);
    }
    // 更新ARR的值本质上就是更新速度
    App_Motor_SetCurrentSpeedRPMCycle(v1);
    __HAL_TIM_SetAutoreload(&htim1, App_Motor_GetUS());
}

// 当CNT与CCR1的值相等的时候触发1次
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1)
    {
        // CNT与CCR1相等于一次，当前角度周期+1
        App_Motor_SetCurrentAngleCycle(App_Motor_getCurrentAngleCycle() + 1);
        printf("current Angle = %f\r\n", App_Motor_getCurrentAngleCycle());
        // 当前角度的周期与目标角度相等，停止电机
        // 更新速度
        NextSpeed();
        if (App_Motor_getCurrentAngleCycle() >= abs(App_Motor_GetTargetAngleCycle()))
        {
            // 电机停止
            printf("App_Motor_Stop\r\n");
            App_Motor_Stop();
        }
    }
}