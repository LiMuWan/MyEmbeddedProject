#include "App_ConsoleToGateWay.h"

uint8_t device_id;//Motor设备id
//引入二值信号量句柄
extern QueueHandle_t motor_start_handler;

//任务1：提供一个注册函数，用于获取MQTT接收到的消息
void App_Motor_ConsoleToGateWay_Handler(char *msg);

void  App_ConsoleToGateWay_Task(void *parameters)
{
    // 0.给MQTT模块提供注册回调函数
    Int_MQTT_Register_CallBack(App_Motor_ConsoleToGateWay_Handler);
    // 1.初始化mqtt(本质w5500入网操作也完成了)
    Int_MQTT_Init();
    while (1)
    {
        // 2.mqtt客户端接受订阅消息,需要持续刷新！不调用此方法客户端接收不到消息的
        Int_MQTT_Yield();
        vTaskDelay(50);
    }
}

void App_Motor_ConsoleToGateWay_Handler(char *msg)
{
    //printf("任务1接收到订阅的消息：%s\r\n",msg);
    //将JSON形式的字符串，转换为CJSON类型结构体指针
    cJSON *json = cJSON_Parse(msg);
    char *connection_type = cJSON_GetObjectItem(json,"connection_type")->valuestring;
    device_id = cJSON_GetObjectItem(json,"device_id")->valueint;
    float target_angle = cJSON_GetObjectItem(json,"target_angle")->valuedouble;
    float max_speed = cJSON_GetObjectItem(json,"max_speed")->valuedouble;
    //释放JSON，不释放容易内存不够导致乱码
    cJSON_Delete(json);

    printf("%s %d %f %f\r\n",connection_type,device_id,target_angle,max_speed);
    //底部这里，网关与电机通过modbus协议传输数据，将来还有可能使用can协议通信
    if(strcmp(connection_type,"rs485") == 0)
    {
        //主设备准备通过modbus将数据发送到电机
        //需要将浮点数：目标角度、最大速度放在一个数组，数组里面元素整形（uint16_t）
        //float浮点数，占4个字节
        uint16_t modbus_send_buffers[4] = {0};
        memcpy(modbus_send_buffers,&target_angle,4);
        memcpy(modbus_send_buffers+2,&max_speed,4);
        //主设备将订阅收集到的信息写入从设备保持就寄存器数组中
        Int_Modbus_WriteHoldingRegs16(device_id,0,modbus_send_buffers,4);

        //稍微延迟一段时间，能保证电机启动了，释放信号量，通知任务，读取输入寄存器内部实时数据，上报
        vTaskDelay(2000);

        //释放信号量
        xSemaphoreGive(motor_start_handler);
    }
    else if(strcmp(connection_type,"can") == 0)
    {
        uint8_t sendData[8] = {0};
        memcpy(sendData,&target_angle,4);
        memcpy(sendData + 4,&max_speed,4);
        
        //通过CAN，将目标角度与最大速度发送过去
        Int_CAN_SendData(CONSOLE_TO_GATEWAY,sendData,8);

        //稍微延迟一段时间，能保证电机启动了
        vTaskDelay(2000);

        //释放信号量
        xSemaphoreGive(motor_start_handler);
    }
}
