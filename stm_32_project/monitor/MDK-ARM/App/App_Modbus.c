#include "App_Modbus.h"

float target_angle = 0;
float max_speed = 0;

//此函数，当中modbus从设备接收到主设备发过来的目标角度、速度（且写入到保持寄存器中）就会执行一次
void Int_ModBus_GetREG_HOLD_BUF(uint16_t buffers[])
{
     //更新电机的旋转目标角度、最大速度
     memcpy(&target_angle,buffers,4);
     memcpy(&max_speed,buffers+2,4);
     printf("target_angle = %f , max_speed = %f\n",target_angle,max_speed);
     //更新参数
     App_Motor_SetTargetAngle(target_angle);
     App_Motor_SetMaxSpeedRPM(max_speed);

     //启动电机
     App_Motor_Start();
}
