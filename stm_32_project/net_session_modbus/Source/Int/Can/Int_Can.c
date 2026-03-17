#include "Int_Can.h"

//1.开启Can节点
void Int_CAN_Start(uint16_t msg_id)
{
    //1.配置接收的滤波器
    CAN_FilterTypeDef filter_config;

    //1.1CAN接收滤波器是能开关开启
    filter_config.FilterActivation = CAN_FILTER_ENABLE;
    //1.2ID需要多少位的，标准模式11位
    filter_config.FilterScale = CAN_FILTERSCALE_16BIT;
    //1.3过滤器的模式：列表模式
    filter_config.FilterMode = CAN_FILTERMODE_IDLIST;
    //1.4使用哪一个过滤器
    filter_config.FilterBank = 0;
    //1.5使用哪一个缓冲器
    filter_config.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    //1.6标识
    filter_config.FilterIdHigh = (msg_id << 5);
    HAL_CAN_ConfigFilter(&hcan,&filter_config);
    //2.开启CAN片上外设，使CAN进入正常模式
    HAL_CAN_Start(&hcan);

}

//2.接受数据
void Int_CAN_ReceiveData(Can_Receive_T *buffers,uint8_t *sizes)
{
    //2.1获取到收到数据帧的个数
    *sizes = HAL_CAN_GetRxFifoFillLevel(&hcan,CAN_FILTER_FIFO0);

    for(uint16_t i = 0;i<*sizes;i++)
    {
        CAN_RxHeaderTypeDef rxHeader;
        HAL_CAN_GetRxMessage(&hcan,CAN_FILTER_FIFO0,&rxHeader,buffers[i].data);
        buffers[i].id = rxHeader.StdId;
        buffers[i].length = rxHeader.DLC;

        //输出打印数据
        printf("gateway can receive\r\n");
        for(uint16_t j = 0;j<buffers[i].length;j++)
        {
            printf("%02x ",buffers[i]);
        }
        printf("\r\n");
    }
}

//3.发送数据
void Int_CAN_SendData(uint16_t msg_id,uint8_t buffers[],uint16_t length)
{
    //如果邮箱没有空闲，不能发送数据
    while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan) == 0)
    {
        HAL_Delay(10);
    }
    CAN_TxHeaderTypeDef txHeader;
    //发送给节点的ID(接收数据的ID)
    txHeader.StdId = msg_id;
    txHeader.DLC = length;
    txHeader.IDE = CAN_ID_STD;
    txHeader.RTR = CAN_RTR_DATA;
    uint32_t maxBoxID;//使用哪一个邮箱发送的数据
    //发送数据
    HAL_CAN_AddTxMessage(&hcan,&txHeader,buffers,&maxBoxID);

    printf("gateway can send data\r\n ");
    for(uint16_t i = 0;i<length;i++)
    {
        printf("%02x ",buffers[i]);
    }
    printf("\r\n");
}
