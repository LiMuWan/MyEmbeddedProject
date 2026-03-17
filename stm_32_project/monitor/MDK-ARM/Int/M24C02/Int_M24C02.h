#ifndef __INT_M24C02_H__
#define __INT_M24C02_H__
#include "i2c.h"
#define M24C02_ADD_W 0XA0
#define M24C02_ADD_R 0XA1

//写入
//1.写入数据地址  2.写入数据  3.写入数据个数
void Int_M24C02_WriteByte(uint16_t mem_addr, uint8_t *data, uint16_t sizes);

//读取
void Int_M24C02_ReadByte(uint16_t mem_addr, uint8_t *buffers, uint16_t sizes);
#endif
/**
 * M24C02内存大小为2KB,每一页16个字节,一共128页。
 * 此模块,写入周期为5ms。使用I2C2通信!
 **/
