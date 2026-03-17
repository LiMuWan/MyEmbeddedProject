#include "App_Motor_PID.h"

Struct_PID pid =
    {
        .Kp = 0.3f,
        .Ki = 0.01f,
        .Kd = 0.2f,
        .error_cur = 0.0f,
        .error_pre = 0.0f,
        .segma = 0.0f,
        .t = 0.5f,
        .output_value = 0.0f};

// 设置ARR寄存器，修改速度
static void Calc_SetARR(void);
// 计算出电机的下一步的速度周期个数
static void NextSpeed(void);

void PID_Reset()
{
    pid.error_cur = 0.0f;
    pid.error_pre = 0.0f;
    pid.segma = 0.0f;
    pid.output_value = 0.0f;
}

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
    // 2.计算速度：本质就是设置ARR重装载寄存器的值
    Calc_SetARR();

    //每一次启动电机需要重置pid的值
    PID_Reset();
    // 4.启动电机
    HAL_GPIO_WritePin(MOTOR_EN_GPIO_Port, MOTOR_EN_Pin, GPIO_PIN_SET);
    // 启动定时器1
    HAL_TIM_OC_Start_IT(&htim1, TIM_CHANNEL_1);
    //启动定时器3,每间隔50ms获取一次编码器的计数
    HAL_TIM_Base_Start_IT(&htim3);
    // 启用电机解码器
    Int_EnableEncoder();
}

void App_Motor_Stop(void)
{
    // 清除数据
    App_Motor_SetCurrentAngleCycle(0);
    App_Motor_SetCurrentSpeedRPMCycle(0);
    // 关闭电机使能
    HAL_TIM_OC_Stop_IT(&htim1, TIM_CHANNEL_1);
    //停止定时器3
    HAL_TIM_Base_Stop_IT(&htim3);
    // 禁用电机解码器
    Int_DisableEncoder();
}

static void Calc_SetARR(void)
{
    // 设置为最小速度
    App_Motor_SetCurrentSpeedRPMCycle(App_Motor_GetMinSpeedRPM());
    // 设置速度本质就是设置TIM的ARR
    __HAL_TIM_SetAutoreload(&htim1, App_Motor_GetUS());
}


static void cal_PID()
{
     // 计算误差
    pid.error_pre = pid.error_cur;
    pid.error_cur = App_Motor_GetTargetAngleCycle() - App_Motor_getCurrentAngleCycle();
    pid.segma += pid.error_cur;

    // 计算出输出值
    pid.output_value = pid.Kp * pid.error_cur + pid.Ki * pid.segma * pid.t +
                       (pid.error_cur - pid.error_pre) / pid.t;

    if (pid.output_value >= App_Motor_GetMaxSpeedRPMCycle())
    {
        pid.output_value = App_Motor_GetMaxSpeedRPMCycle();
    }
    if (pid.output_value <= App_Motor_GetMinSpeedRPM())
    {
        pid.output_value = App_Motor_GetMinSpeedRPM();
    }

    //更新当前速度周期
    App_Motor_SetCurrentSpeedRPMCycle(pid.output_value);
}

static void NextSpeed(void)
{
   __HAL_TIM_SetAutoreload(&htim1,App_Motor_GetCurrentSpeedRPMCycle());
}

// 当CNT与CCR1的值相等的时候触发1次，每65535个计数触发一次
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

//每50ms执行一次
void TIM3_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
   if(htim->Instance == TIM3)
   {
       //1.获取编码器计数的数值 编码器的4000个计数为一圈 但是TIM1计数为3200个为一个周期，需要转换一下
       App_Motor_SetCurrentAngleCycle(Int_Encoder_GetCount(App_Motor_GetTargetAngleCycle() > 0 ? UP : DOWN) / COUNT_PER_TURN * 3200);
       //2.计算PID
        cal_PID();
   }
}
