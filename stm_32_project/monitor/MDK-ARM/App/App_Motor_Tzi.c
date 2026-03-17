// #include "App_Motor_Tzi.h"

// /**
//  * @brief 使用TIM3用来统计电机转动的圈数
//  * 每50ms统计一次
//  * Pre = 71
//  * ARR = 50000 => 50ms
//  *
//  */
// static turns = 0;

// float App_Motor_GetTurns()
// {
//     return turns;
// }

// void TIM3_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
// {
//     turns = (float)Int_Encoder_GetCount(App_Motor_GetTargetAngleCycle() > 0 ? UP : DOWN) / COUNT_PER_TURN;
// }