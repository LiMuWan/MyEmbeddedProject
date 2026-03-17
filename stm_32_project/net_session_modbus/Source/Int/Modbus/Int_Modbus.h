#ifndef __INT_MODBUS_H__
#define __INT_MODBUS_H__

#include "usart.h"
#include "mbcrc.h"
#include "FreeRTOS.h"
#include <stdio.h>

//1.封装一个函数，底层用USART2将命令发送给从设备
void Int_Modbus_SendCmd(uint8_t *cmd,uint16_t size);

/**
 * @brief  读线圈 
 */
void Int_Modbus_ReadCoil(uint8_t dev_id,  uint16_t start_addr,uint16_t num);

/**
 * @brief  读离散输入
 */
void Int_Modbus_ReadInputStatus(uint8_t dev_id,  uint16_t start_addr,uint16_t num);

/**
 * @brief  读保持寄存器
 */
void Int_Modbus_ReadHoldingReg(uint8_t dev_id,  uint16_t start_addr,uint16_t num);

/**
 * @brief  读输入寄存器
 */
void Int_Modbus_ReadInputReg(uint8_t dev_id,  uint16_t start_addr,uint16_t num);

/**
 * @brief  写单个线圈 
 */
void Int_Modbus_WriteCoil(uint8_t dev_id,  uint16_t start_addr,uint8_t value);

/**
 * @brief  写单个保持寄存器
 */
void Int_Modbus_WriteHoldingReg(uint8_t dev_id,  uint16_t start_addr,uint16_t value);

/**
 * @brief  写多个线圈
 */
void Int_Modbus_WriteCoils(uint8_t dev_id,  uint16_t start_addr,uint16_t num,uint8_t *value);

/**
 * @brief  写多个保持寄存器 按字节数组写入
 */
void Int_Modbus_WriteHoldingRegs(uint8_t dev_id,  uint16_t start_addr, uint8_t *data ,uint16_t len);

/**
 * @brief  写多个保持寄存器   按寄存器数组写入
 */
void Int_Modbus_WriteHoldingRegs16(uint8_t dev_id,  uint16_t start_addr, uint16_t *reg_data ,uint16_t reg_len);


#endif /* __INT_MODBUS_H__ */
