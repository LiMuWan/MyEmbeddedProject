#include "Int_Modbus.h"

void Int_Modbus_SendCmd(uint8_t *cmd, uint16_t size)
{
    HAL_UART_Transmit(&huart2, cmd, size, 1000);
    // 每一次发送命令最好稍微有点延迟
    HAL_Delay(100);

    // 测试代码，将主设备发送过去的命令输出打印
    printf(" master send cmd ");
    for (uint16_t i = 0; i < size; i++)
    {
        printf("%02X ", cmd[i]);
    }
    printf("\r\n");
}

/**
 * @brief  读线圈
 */
void Int_Modbus_ReadCoil(uint8_t dev_id, uint16_t start_addr, uint16_t num)
{
    uint8_t cmd[8] = {0}; 
    cmd[0] = dev_id;
    cmd[1] = 0x01;
    cmd[2] = (start_addr >> 0) & 0xFF;
    cmd[3] = (start_addr >> 0) & 0xFF;
    cmd[4] = (num >> 8) & 0xFF;
    cmd[5] = (num >> 0) & 0xFF;
    uint16_t crc = usMBCRC16(cmd, 6);
    // crc低位在前
    cmd[6] = (crc >> 0) & 0xFF;
    cmd[7] = (crc >> 8) & 0xFF;
    Int_Modbus_SendCmd(cmd, 8);
}

/**
 * @brief  读离散输入
 */
void Int_Modbus_ReadInputStatus(uint8_t dev_id, uint16_t start_addr, uint16_t num)
{
    uint8_t cmd[8] = {0};
    cmd[0] = dev_id;
    cmd[1] = 0x02;
    cmd[2] = (start_addr >> 8) & 0xFF;
    cmd[3] = (start_addr >> 0) & 0xFF;
    cmd[4] = (num >> 8) & 0xFF;
    cmd[5] = (num >> 0) & 0xFF;
    uint16_t crc = usMBCRC16(cmd,6);
    //crc低位在前
    cmd[6] = (crc >> 0)&0xFF;
    cmd[7] = (crc >> 8)&0xFF;
    Int_Modbus_SendCmd(cmd,8);
}

/**
 * @brief  读保持寄存器
 */
void Int_Modbus_ReadHoldingReg(uint8_t dev_id, uint16_t start_addr, uint16_t num)
{
    uint8_t cmd[8] = {0};
    cmd[0] = dev_id;
    cmd[1] = 0x03;
    cmd[2] = (start_addr >> 8)&0xFF;
    cmd[3] = (start_addr >> 0)&0xFF;
    cmd[4] = (num >> 8) &0xFF;
    cmd[5] = (num >> 0)&0xFF;
    uint16_t crc = usMBCRC16(cmd,6);
    //crc低位在前
    cmd[6] = (crc >> 0)&0xFF;
    cmd[7] =(crc >> 8)&0xFF;
    Int_Modbus_SendCmd(cmd,8);
}

/**
 * @brief  读输入寄存器
 */
void Int_Modbus_ReadInputReg(uint8_t dev_id, uint16_t start_addr, uint16_t num)
{
    uint8_t cmd[8] = {0};
    cmd[0] = dev_id;
    cmd[1] = 0x04;
    cmd[2] = (start_addr >> 8)&0xFF;
    cmd[3] = (start_addr >> 0)&0xFF;
    cmd[4] = (num >> 8)&0xFF;
    cmd[5] = (num >> 0)&0xFF;
    uint16_t crc = usMBCRC16(cmd,6);
    //crc低位在前
    cmd[6] = (crc >> 0)&0xFF;
    cmd[7] = (crc >> 8)&0xFF;
    Int_Modbus_SendCmd(cmd,8);
}

/**
 * @brief  写单个线圈
 */
void Int_Modbus_WriteCoil(uint8_t dev_id, uint16_t start_addr, uint8_t value)
{
    uint8_t cmd[8] = {0};
    cmd[0] = dev_id;
    cmd[1] = 0x05;
    cmd[2] = (start_addr >> 8)&0xFF;
    cmd[3] = (start_addr >> 0)&0xFF;
    if(value == 1)
    {
        cmd[4] = 0xff;
    }
    else
    {
        cmd[4] = 0x00;
    }
    //此位字节恒为0，用不到
    cmd[5] = 0x00;
    uint16_t crc = usMBCRC16(cmd,6);
    //crc低位在前
    cmd[6] = (crc >> 0)&0xFF;
    cmd[7] = (crc >> 8)&0xFF;
    Int_Modbus_SendCmd(cmd,8);
}

/**
 * @brief  写单个保持寄存器
 */
void Int_Modbus_WriteHoldingReg(uint8_t dev_id, uint16_t start_addr, uint16_t value)
{
    uint8_t cmd[8] = {0};
    cmd[0] = dev_id;
    cmd[1] = 0x06;
    cmd[2] = (start_addr >> 8)&0xFF;
    cmd[3] = (start_addr >> 0)&0xFF;
    cmd[4] = (value >> 8)&0xFF;
    cmd[5] = (value >> 0)&0xFF;
    uint16_t crc = usMBCRC16(cmd,6);
    //crc低位在前
    cmd[6] = (crc >> 0)&0xFF;
    cmd[7] = (crc >> 8)&0xFF;
    Int_Modbus_SendCmd(cmd,8);
}

/**
 * @brief  写多个线圈
 */
void Int_Modbus_WriteCoils(uint8_t dev_id, uint16_t start_addr, uint16_t num, uint8_t *value)
{
    uint8_t bytes_num = num%8 == 0? num/8 : (num/8+1);
    uint8_t total_len = 9 + bytes_num;
    uint8_t *cmd = pvPortMalloc(total_len);
    cmd[0] = dev_id;
    cmd[1] = 0x0f;
    cmd[2] = (start_addr >> 8)&0xFF;
    cmd[3] = (start_addr >> 0)&0xFF;
    cmd[4] = (num >> 8)&0xFF;
    cmd[5] = (num >> 0)&0xFF;
    cmd[6] = bytes_num;
    //数据部分
    for(uint8_t i = 0;i < bytes_num;i++)
    {
        uint8_t data = 0x00;
        for(uint8_t j = 0;j<8;j++)
        {
            data |= value[i*8+j] << j;
        }
        cmd[7+i] = data;//算好了一个字节8位
    }

    uint16_t crc = usMBCRC16(cmd,total_len-2);
    //crc低位在前
    cmd[total_len - 2] = (crc >> 0)&0xff;
    cmd[total_len - 1] = (crc >> 8)&0xff;
    Int_Modbus_SendCmd(cmd,total_len);
    vPortFree(cmd);
}

/**
 * @brief  写多个保持寄存器 按字节数组写入
 */
void Int_Modbus_WriteHoldingRegs(uint8_t dev_id, uint16_t start_addr, uint8_t *data, uint16_t len)
{
    uint8_t total_len = 9 + len;
    uint8_t *cmd = pvPortMalloc(total_len);
    uint16_t reg_num = len/2;
    cmd[0] = dev_id;
    cmd[1] = 0x10;
    cmd[2] = (start_addr >> 8)&0xFF;
    cmd[3] = (start_addr >> 0)&0xFF;
    cmd[4] = (reg_num >> 8) & 0xFF;
    cmd[5] = (reg_num >> 0)&0xFF;
    cmd[6] = len;
    for(int i = 0;i<len;i++)
    {
        cmd[7+i] = data[i];
    }
    uint16_t crc = usMBCRC16(cmd,total_len - 2);
    //crc低位在前
    cmd[total_len - 2] = (crc >> 0)&0xFF;
    cmd[total_len - 1] = (crc >> 8)&0xFF;

    Int_Modbus_SendCmd(cmd,total_len);
    vPortFree(cmd);
}

/**
 * @brief  写多个保持寄存器   按寄存器数组写入
 */
void Int_Modbus_WriteHoldingRegs16(uint8_t dev_id, uint16_t start_addr, uint16_t *reg_data, uint16_t reg_len)
{
    uint8_t total_len = reg_len * 2 + 9;
    uint8_t *cmd = pvPortMalloc(total_len);
    cmd[0] = dev_id;
    cmd[1] = 0x10;
    cmd[2] = (start_addr >> 8)&0xFF;
    cmd[3] = (start_addr >> 0)&0xFF;
    cmd[4] = (reg_len >> 8)&0xFF;
    cmd[5] = (reg_len >> 0)&0xFF;
    cmd[6] = (uint8_t)reg_len * 2;
    for(int i = 0;i<reg_len;i++)
    {
        cmd[7+i*2] = (reg_data[i] >> 8)&0xFF;
        cmd[8+i*2] = (reg_data[i] >> 0)&0xFF;
    }
    uint16_t crc = usMBCRC16(cmd,total_len - 2);
    //crc低位在前
    cmd[total_len - 2] = (crc >> 0)&0xFF;
    cmd[total_len - 1] = (crc >> 8)&0xFF;
    Int_Modbus_SendCmd((uint8_t *)cmd,total_len);
    vPortFree(cmd);
}
