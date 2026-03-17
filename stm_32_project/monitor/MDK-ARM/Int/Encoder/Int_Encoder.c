#include "Int_Encoder.h"

uint32_t encoder_overflow_count = 0;

// 启用解码器
void Int_EnableEncoder(void)
{
    __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_UPDATE);
    HAL_TIM_IC_Start(&htim4, TIM_CHANNEL_1 | TIM_CHANNEL_2);
    // 启动的时候会自动执行一次更新中断，如果是向下计数的话，会有一次溢出
    // 清除的开始更新中断
    __HAL_TIM_CLEAR_IT(&htim4, TIM_IT_UPDATE);
    encoder_overflow_count = 0;
    // 重置为0
    __HAL_TIM_SetCounter(&htim4, 0);
}

// 禁用解码器
void Int_DisableEncoder(void)
{
    __HAL_TIM_DISABLE_IT(&htim4, TIM_IT_UPDATE);
    HAL_TIM_IC_Stop(&htim4, TIM_CHANNEL_1 | TIM_CHANNEL_2);
}

// 获取解码器的返回的计数总个数
uint32_t Int_Encoder_GetCount(COUNT_DIR dir)
{
    if (dir == UP)
    {
        return encoder_overflow_count * 65536 + __HAL_TIM_GET_COUNTER(&htim4);
    }
    else if(dir == DOWN)
    {
        return encoder_overflow_count - 65536 + __HAL_TIM_GET_COUNTER(&htim4);
    }

    return 0;
}

//TIM3 溢出 更新中断服务程序
__weak void TIM3_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM4)
    {
        encoder_overflow_count++;
    }
    else if (htim->Instance == TIM3)
    {
         TIM3_PeriodElapsedCallback(htim);
    }
}
