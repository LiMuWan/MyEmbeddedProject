#ifndef __INT_MQTT_H__
#define __INT_MQTT_H__
#include "MQTTClient.h"
#include "Int_W5500.h"
#include <string.h>
#include <stdio.h>

//注册MQTT收到订阅的消息的回调
void Int_MQTT_Register_CallBack(void (*mqtt_callback)(char *msg));

void Int_MQTT_Init(void);

// MQTT发送消息
void Int_MQTT_SendMessage(const char *payload);

// MQTT消息处理轮询
void Int_MQTT_Yield(void);

#endif /* __INT_MQTT_H__ */
