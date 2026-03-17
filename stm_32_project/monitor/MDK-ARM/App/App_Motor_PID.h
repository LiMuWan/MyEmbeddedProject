#ifndef __APP_MOTOR_PID_H__
#define __APP_MOTOR_PID_H__
#include "App_Motor_Value.h"
#include "stdlib.h"
#include "stm32f1xx_hal.h"
#include "main.h"
#include "tim.h"
#include "math.h"
#include "Int_Encoder.h"

typedef struct  
{
   float Kp;
   float Ki;
   float Kd;
   float error_cur;
   float error_pre;
   float segma;
   float t;
   float output_value;
} Struct_PID;


void App_Motor_Start(void);

void App_Motor_Stop(void);

#endif /* __APP_MOTOR_H__ */

