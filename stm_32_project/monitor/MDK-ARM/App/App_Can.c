#include "App_Can.h"

// 存储目标角度、最大速度
float target_angle_can = 0;
float max_speed_can = 0;
// 获取电机运行状态
uint8_t motor_run_status = 0;

// 保存电机状态(上一次状态)
uint8_t motor_run_pre = 0;

// 初始化CAN
void App_Can_init(void)
{
    // 初始化CAN一次，目的启动CAN,设置过滤器
    Int_CAN_Start(CONSOLE_TO_GATEWAY);
}
// 电机节点需要持续判断是否接受到消息
void App_Can_Poll(void)
{
    // 电机停止状态，接收到数据，更新目标角度与速度
    // 如果当前电机正在运行结算接收到，也不更新！
    if (motor_run_status == 0)
    {
        Can_Receive_T motor_rev_buffer[3] = {0};
        uint8_t sizes = 0;
        Int_CAN_ReceiveData(motor_rev_buffer, &sizes);

        // 2.修改电机参数
        for (uint8_t i = 0; i < sizes; i++)
        {
            memcpy(&target_angle_can, motor_rev_buffer[i].data, 4);
            memcpy(&max_speed_can, motor_rev_buffer[i].data + 4, 4);
            // 设置
            App_Motor_SetTargetAngle(target_angle_can);
            App_Motor_SetMaxSpeedRPM(max_speed_can);

            // 电机启动
            App_Motor_Start();
        }
    }

    // 电机CAN节点只要电机转动，CAN节点需要将当前的转动的角度与电机状态上报网关
    if (motor_run_status || motor_run_pre)
    {
        uint8_t uploadData[5] = {0};
        float current_angle = App_Motor_GetCurrentAngle();
        // 当前角度
        memcpy(uploadData, &current_angle, 4);
        // 电机状态
        uploadData[4] = motor_run_status;
        Int_CAN_SendData(GATEWAY_TO_CONSOLE,uploadData,5);
    }

    //保存电机上一次运行状态
    motor_run_pre = motor_run_status;
}