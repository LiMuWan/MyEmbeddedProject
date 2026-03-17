#include "App_GateWayToConsole.h"

extern QueueHandle_t motor_start_handler;
extern QueueHandle_t receive_msg_handler;//接收消息的句柄
// 引入设备的modbusID
extern uint8_t device_id;
// 准备一个容器，接收从设备响应的数据
uint8_t receive_motor_buffers[50];
uint16_t receive_data_length = 0;

float current_angle;
float motor_status;

//整理电机发送过来的数据
void App_Motor_GateWayConsole_GetData(void);
//上报数据
void App_Motor_GateWayConsole_UploadData(void);

void App_GateWayToConsole_Task(void *params)
{
    // 开启一次接收变长数据中断，因为进入中断服务程序【两种情况：写入保持寄存器启动电机、读取输入寄存器实时数据】
    HAL_UARTEx_ReceiveToIdle_IT(&huart2, receive_motor_buffers, 50);
    while (1)
    {
        // 获取二值信号量，判断电机是否启动
        BaseType_t res = xSemaphoreTake(motor_start_handler, UINT32_MAX);

        // 判断电机已经启动
        if (res == pdTRUE)
        {
            // 电机启动以后主设备需要持续，不断的向从设备电机发送读取输入寄存器命令（指导电机停止）
            while (1)
            {
                // 1.主设备需要持续发送读取输入寄存器命令
                Int_Modbus_ReadInputReg(device_id, 0x00, 3);
                //2.获取信号量，能知道对应电机上报数据真的获取到了
                res = xSemaphoreTake(receive_msg_handler,2000);
                if(res == pdTRUE)
                {
                    //1.整理电机发送过来的数据
                    App_Motor_GateWayConsole_GetData();
                    //2.上报数据
                    App_Motor_GateWayConsole_UploadData();
                }

                if(!motor_status)
                {
                    break;
                }
            }
        }
    }
}

void App_Motor_GateWayConsole_GetData(void)
{
    //获取到电机响应的数据
    printf("geteway data:");
    for(uint16_t i = 0;i<receive_data_length;i++)
    {
        printf("%02X ",receive_motor_buffers[i]);
    }
    printf("\r\n");

    //获取电机当前运行角度：3  4 5 6 四个字节，保存的是当前电机角度
    uint8_t tmp = 0;
    tmp = receive_motor_buffers[3];
    receive_motor_buffers[3] = receive_motor_buffers[4];
    receive_motor_buffers[4] = tmp;

    tmp = 0;
    tmp = receive_motor_buffers[5];
    receive_motor_buffers[5] = receive_motor_buffers[6];
    receive_motor_buffers[6] = tmp;

    memcpy(&current_angle,receive_motor_buffers + 3,4);
    //获取到电机当前运行状态
    motor_status = receive_motor_buffers[8];
    printf("%f %d\r\n",current_angle,motor_status);
}

void App_Motor_GateWayConsole_UploadData(void)
{
    //1.通过 cJson_CreateObject方法创建了一个结构体指针
    cJSON *root = cJSON_CreateObject();
    //JSON(结构体)：添加成员 cur_angle
    cJSON_AddNumberToObject(root,"device_id",device_id);
    
    //JSON（结构体）：添加成员 cur_angle
    char cur_angle_str[10];
    sprintf(cur_angle_str,"%.1f",current_angle);
    cJSON_AddStringToObject(root,"cur_angle",cur_angle_str);

    //将JSON结构体转换为JSON形式字符串
    char *json_str = cJSON_PrintUnformatted(root);

    //通过mqtt将JSON形式的字符串上报
    Int_MQTT_SendMessage(json_str);
    cJSON_Delete(root);
    cJSON_free(root);
}

// USART中断收消息的回调
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART2)
    {
        HAL_UARTEx_ReceiveToIdle_IT(&huart2, receive_motor_buffers, 50);

        // 判断一下当前命令是否是读取输入寄存器 //0 位置是设备地址 1位置是功能码
        if (receive_motor_buffers[1] == 0x04)
        {
            receive_data_length = Size;
            BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
            xSemaphoreGiveFromISR(receive_msg_handler,&pxHigherPriorityTaskWoken);
            if(pxHigherPriorityTaskWoken == pdTRUE)
            {
                //任务解除阻塞状态，并且切换回任务的上下文
                portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
            }
        }
    }
}
