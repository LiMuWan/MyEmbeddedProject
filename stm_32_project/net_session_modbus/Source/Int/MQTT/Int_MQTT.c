#include "Int_MQTT.h"

/***************创建套接字相关参数*********************/
Network network;
MQTTClient client;
#define SN 0 // 使用的套接字编号
#define MQTT_BUFF_SIZE 1024
uint8_t mqtt_sendbuf[MQTT_BUFF_SIZE];
uint8_t mqtt_readbuf[MQTT_BUFF_SIZE];
uint8_t sever_ip[4] = {192, 168, 49, 25}; // MQTT服务器IP
#define SERVER_PORT 1883                  // MQTT服务器端口
#define APP_TO_MOTOR "console_to_gateway"
#define MOTOR_TO_APP "gateway_to_console"
void (*cb)(char *msg); // 回调消息原型
/*********************************************/

// 创建Socket
static void Int_MQTT_CreateSocketAndConnectServer(void);

// 创建MQTTClient和连接MQTT服务器
static void Int_MQTT_CreateMQTTClientAndConnect(void);

static void MessageHandler(MessageData *msg);
void Int_MQTT_Init(void)
{
    // W500初始化
    Int_W5500_Init();
    //  MQTT initialization code goes here
    // 先连接w5500，再通过W5500连接MQTT
    // 2.创建socket套接字(通信的通道0-7),确定这个通道与那个远程服务器链接
    Int_MQTT_CreateSocketAndConnectServer();
    // 3.创建MQTT客户端(网关项目,MQTT客户端)
    Int_MQTT_CreateMQTTClientAndConnect();
     // 3.订阅消息
    // 第二个参数:订阅消息(主题)
    // 第三个参数:QOS0,标识的是客户端通信时候,最多一次!
    // 第四个参数:需要的是一个函数,当订阅的主题有消息传递给我们,次函数就会执行一次！
    MQTTSubscribe(&client, APP_TO_MOTOR, QOS0, MessageHandler);
}

void Int_MQTT_SendMessage(const char *payload)
{
    MQTTMessage message;
    message.qos = QOS0;
    message.payload = (void *)payload;
    message.payloadlen = strlen(payload);
    MQTTPublish(&client, MOTOR_TO_APP, &message);
}

void Int_MQTT_Yield(void)
{
    MQTTYield(&client, 100);
}

static void Int_MQTT_CreateSocketAndConnectServer(void)
{
    // 1.创建Network
    NewNetwork(&network, SN);
    // 2.连接服务器
    int result = ConnectNetwork(&network, sever_ip, SERVER_PORT);
    if (result)
    {
        printf("Connect Server Success/r/n");
    }
    else
    {
        printf("Connect Server Failed/r/n");
    }
}

static void Int_MQTT_CreateMQTTClientAndConnect(void)
{
    // 4.初始化MQTT客户端
    MQTTClientInit(&client, &network, 1000, mqtt_sendbuf, MQTT_BUFF_SIZE, mqtt_readbuf, MQTT_BUFF_SIZE);
    MQTTPacket_connectData default_options = MQTTPacket_connectData_initializer;
    default_options.clientID.cstring = "BJ-8018-LRR1"; // 客户端的ID
    default_options.keepAliveInterval = 60;            // 心跳检测的时候单位s
    int result = MQTTConnect(&client, &default_options);
    // 两个数组2048个字节,次函数在freeRTOS任务当中调用,容易栈溢出,导致卡死！
    if (result == SUCCESS)
    {
        printf("MQTT Connect Success/r/n");
    }
    else
    {
        printf("MQTT Connect Filed/r/n");
    }
}

static void MessageHandler(MessageData *msg)
{
    printf("topic:%s , msg : %s\r\n",msg->topicName->cstring,(char *)msg->message->payload);
    // 将接收到的订阅消息传递给别人
    if (cb != NULL)
    {
        cb((char *)msg->message->payload);
    }
}

// 注册MQTT收到消息的回调
void Int_MQTT_Register_CallBack(void (*mqtt_callback)(char *msg))
{
    cb = mqtt_callback;
}
