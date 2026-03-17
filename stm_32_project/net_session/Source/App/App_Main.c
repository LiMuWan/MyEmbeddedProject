#include "App_Main.h"

#define App_ConsoleToGateWay_Task_Task_Name "App_ConsoleToGateWay_Task"
#define App_ConsoleToGateWay_Task_StackSize 1024
#define App_ConsoleToGateWay_Task_Priority 4
TaskHandle_t App_ConsoleToGateWay_Task_handle;

#define App_GateWayToConsole_Task_Task_Name "App_GateWayToConsole_Task"
#define App_GateWayToConsole_Task_StackSize 1024
#define App_GateWayToConsole_Task_Priority 4
TaskHandle_t App_GateWayToConsole_Task_handle;

QueueHandle_t motor_start_handler;//电机开始句柄
QueueHandle_t receive_msg_handler;//
void vApplicationStackOverflowHook(TaskHandle_t xTask,
                                   char *pcTaskName)
{
    // 堆栈溢出处理逻辑
    printf("Stack overflow in task: %s\r\n", pcTaskName);
}

void App_Main(void)
{
    //创建二值信号量
    motor_start_handler = xSemaphoreCreateBinary();
    //创建接收消息的信号量
    receive_msg_handler = xSemaphoreCreateBinary();
    // Application main logic goes here
    //1.创建FreeRTOS任务
    xTaskCreate(App_ConsoleToGateWay_Task, App_ConsoleToGateWay_Task_Task_Name, App_ConsoleToGateWay_Task_StackSize, NULL, App_ConsoleToGateWay_Task_Priority, &App_ConsoleToGateWay_Task_handle);
    xTaskCreate(App_GateWayToConsole_Task, App_GateWayToConsole_Task_Task_Name, App_GateWayToConsole_Task_StackSize, NULL, App_ConsoleToGateWay_Task_Priority, &App_GateWayToConsole_Task_handle);

    //2.调度器开始
    vTaskStartScheduler();
}
