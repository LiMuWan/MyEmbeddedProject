#include "App_Display.h"
// 汉字:16 * 16  英文:8 * 16
//  页面的标志位
uint8_t pageNo = 1;

// 展示标题方法
static void App_Display_ShowTitle(void)
{
    // 尚硅谷电机项目
    for (uint8_t i = 0; i < 7; i++)
    {
        Inf_OLED_ShowChinese(8 + i * 16, 8, i, 16, 1);
    }
}

// 展示角度
static void App_Display_ShowAngle(void)
{
    // 英文最多:一行16个
    // 目标角度
    uint8_t target_buffers[16] = {0};
    sprintf((char *)target_buffers, "tar ang:%6.1f", App_Motor_GetTargetAngle());
    Inf_OLED_ShowString(8, 8 + 16, target_buffers, 16, 1);

    // 目标角度
    uint8_t current_buffers[16] = {0};
    sprintf((char *)current_buffers, "cur ang:%6.1f", App_Motor_GetCurrentAngle());
    Inf_OLED_ShowString(8, 8 + 16 + 16, current_buffers, 16, 1);
}
// 显示速度
static void App_Display_ShowSpeedRPM(void)
{
    // 英文最多:一行16个
    // 目标角度
    uint8_t maxSpeed_buffers[16] = {0};
    sprintf((char *)maxSpeed_buffers, "max spd:%6.1f", App_Motor_GetMaxSpeedRPM());
    Inf_OLED_ShowString(8, 8 + 16, maxSpeed_buffers, 16, 1);

    // 目标角度
    uint8_t currentSpeed_buffers[16] = {0};
    sprintf((char *)currentSpeed_buffers, "cur spd:%6.1f", App_Motor_GetCurrentSpeedRPM());
    Inf_OLED_ShowString(8, 8 + 16 + 16, currentSpeed_buffers, 16, 1);
}

// 显示ID
static void App_Display_ShowModBusID(void)
{
    uint8_t modbusID_buffers[16] = {0};
    sprintf((char *)modbusID_buffers, "mod id:%d", App_GetModBusId());
    Inf_OLED_ShowString(8, 8 + 16, modbusID_buffers, 16, 1);
}

// 1.初始化方法
void App_Display_Init(void)
{

    // 1.初始化OLED
    Inf_OLED_Init();

    // 2.页面切换
    App_Display_ShowPage();
}

// 2.页面的切换
void App_Display_ShowPage(void)
{
    // 1.每一个页面都有标题
    App_Display_ShowTitle();

    // 2.切换页面
    if (pageNo == 1)
    {
        // 显示角度
        App_Display_ShowAngle();
    }
    else if (pageNo == 2)
    {
        // 显示速度
        App_Display_ShowSpeedRPM();
    }
    else
    {
        // 显示ID
        App_Display_ShowModBusID();
    }

    // 3刷新数据
    Inf_OLED_Refresh();
}

// 3.更新页码
void App_Display_UpdatePageNo(void)
{
    // 清屏
    Inf_OLED_Clear();
    pageNo++;
    if (pageNo > 3)
    {
        pageNo = 1;
    }
}

// 4.获取当前页码
uint8_t App_DisPlay_GetPageNo(void)
{
    return pageNo;
}
