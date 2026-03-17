#ifndef __INT_ENCODER_H__
#define __INT_ENCODER_H__
#include "tim.h"
//每一圈返回4000个方波
#define COUNT_PER_TURN 4000

typedef enum 
{
   UP,
   DOWN
} COUNT_DIR;

//启用解码器
void Int_EnableEncoder(void);


//禁用解码器
void Int_DisableEncoder(void);

//获取解码器的返回的计数总个数
uint32_t Int_Encoder_GetCount(COUNT_DIR dir);

#endif /* __APP_ENCODER_H__ */
