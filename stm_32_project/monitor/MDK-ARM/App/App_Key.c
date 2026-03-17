#include "App_Key.h"

// 处理按键功能方法
static void App_Key_Handler(uint16_t key_num);

// 外部中断,用于判断按键
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == Key1_Pin)
    {
        // 按键防抖
        HAL_Delay(10);
        // 确定按下按键1
        if (HAL_GPIO_ReadPin(Key1_GPIO_Port, Key1_Pin) == GPIO_PIN_RESET)
        {
            printf("key1 press down\r\n");
            App_Key_Handler(1);
        }
    }
    if (GPIO_Pin == Key2_Pin)
    {
        // 按键防抖
        HAL_Delay(10);
        // 确定按下按键1
        if (HAL_GPIO_ReadPin(Key2_GPIO_Port, Key2_Pin) == GPIO_PIN_RESET)
        {
            App_Key_Handler(2);
        }
    }
    if (GPIO_Pin == Key3_Pin)
    {
        // 按键防抖
        HAL_Delay(10);
        // 确定按下按键1
        if (HAL_GPIO_ReadPin(Key3_GPIO_Port, Key3_Pin) == GPIO_PIN_RESET)
        {
            App_Key_Handler(3);
        }
    }
    if (GPIO_Pin == Key4_Pin)
    {
        // 按键防抖
        HAL_Delay(10);
        // 确定按下按键1
        if (HAL_GPIO_ReadPin(Key4_GPIO_Port, Key4_Pin) == GPIO_PIN_RESET)
        {
            App_Key_Handler(4);
        }
    }
}
static void App_Key_Handler(uint16_t key_num)
{
    // 根据页面判断当前按键处理的业务
    uint8_t pageNo = App_DisPlay_GetPageNo();
    printf("pageNo = %d\r\n",pageNo);
    // oled页面1
    if (pageNo == 1)
    {
        if (key_num == 1)
        {
            // 角度设置
            App_Motor_SetTargetAngle(App_Motor_GetTargetAngle() + 90);
            printf("App_Motor_SetTargetAngle\r\n");
        }
        else if (key_num == 2)
        {
            App_Motor_SetTargetAngle(App_Motor_GetTargetAngle() - 90);
            printf("App_Motor_SetTargetAngle\r\n");
        }
        else if (key_num == 3)
        {
            // 1.电机启动
            App_Motor_Start();
            printf("App_Motor_Start\r\n");
        }
        else if (key_num == 4)
        {
            App_Display_UpdatePageNo();
        }
    }

    // oled页面2
    if (pageNo == 2)
    {
        if (key_num == 1)
        {
            // 最大速度设置
            App_Motor_SetMaxSpeedRPM(App_Motor_GetMaxSpeedRPM() + 20);
        }
        else if (key_num == 2)
        {
            App_Motor_SetMaxSpeedRPM(App_Motor_GetMaxSpeedRPM() - 20);
        }
        else if (key_num == 3)
        {
            // 1.电机启动
            App_Motor_Start();
            printf("App_Motor_Start\r\n");
        }
        else if (key_num == 4)
        {
            App_Display_UpdatePageNo();
        }
    }

    // oled页面3
    if (pageNo == 3)
    {
        if (key_num == 1)
        {
            // modbus的唯一标识操作
            App_SetModBusID(App_GetModBusId() + 1);
            printf("App_SetModBusID\r\n");
        }
        else if (key_num == 2)
        {
            App_SetModBusID(App_GetModBusId() - 1);
            printf("App_SetModBusID\r\n");
        }
        else if (key_num == 3)
        {
            printf("key3_down\r\n");
        }
        else if (key_num == 4)
        {
            App_Display_UpdatePageNo();
        }
    }
}
