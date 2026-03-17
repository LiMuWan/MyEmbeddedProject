#include "App_ModBusID.h"
#include <stdint.h>
// 模数验证
uint8_t magic_buffers[2] = {0xA8, 0xC7};
uint8_t modbus_id = 0;
void App_MosBusID_Init(void)
{
    uint8_t tmp_buffers[2] = {0};
    Int_M24C02_ReadByte(0x00, tmp_buffers, 2);
    // 先判断验证ID是否存在,如果存在读取
    if (memcmp((char *)magic_buffers, (char *)tmp_buffers, 2) == 0)
    {
        Int_M24C02_ReadByte(0x02, &modbus_id, 1);
    }
    else
    {
        // 不存在初始化存储1
        modbus_id = 5;
        Int_M24C02_WriteByte(0x00, magic_buffers, 2);
        Int_M24C02_WriteByte(0x02, &modbus_id, 1);
    }
}
// 2.设置ID
void App_SetModBusID(uint8_t newID)
{
    // RAM保存一份
    modbus_id = newID;
    // ROM保存一份
    Int_M24C02_WriteByte(0x00, magic_buffers, 2);
    Int_M24C02_WriteByte(0x02, &modbus_id, 1);
}

// 3.获取ID
uint8_t App_GetModBusId(void)
{
    return modbus_id;
}
