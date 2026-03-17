#ifndef __INT_CAN_H__
#define __INT_CAN_H__
#include "can.h"
#include <stdio.h>
#include <stdint.h>

#define CONSOLE_TO_GATEWAY 0x0001
#define GATEWAY_TO_CONSOLE 0x0002

typedef struct 
{
    uint32_t id;
    uint32_t length;
    uint8_t data[8];
    /* data */
} Can_Receive_T;

//1.开启Can节点
void Int_CAN_Start(uint16_t msg_id);

//2.接受数据
void Int_CAN_ReceiveData(Can_Receive_T *buffers,uint8_t *sizes);

//3.发送数据
void Int_CAN_SendData(uint16_t msg_id,uint8_t buffers[],uint16_t length);

#endif /* __INT_CAN_H__ */
