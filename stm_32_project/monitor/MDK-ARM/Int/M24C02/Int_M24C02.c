#include "Int_M24C02.h"

// 写入
void Int_M24C02_WriteByte(uint16_t mem_addr, uint8_t *data, uint16_t sizes)
{
    HAL_I2C_Mem_Write(&hi2c2, M24C02_ADD_W, mem_addr, I2C_MEMADD_SIZE_8BIT, data, sizes, 1000);
    // 写入周期为5ms
    HAL_Delay(5);
}

// 读取
void Int_M24C02_ReadByte(uint16_t mem_addr, uint8_t *buffers, uint16_t sizes)
{
    HAL_I2C_Mem_Read(&hi2c2, M24C02_ADD_R, mem_addr, I2C_MEMADD_SIZE_8BIT, buffers, sizes, 1000);
}
