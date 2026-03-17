#include "Int_W5500.h"

#define ETHERNET_BUF_MAX_SIZE (1024 * 2)
uint8_t ethernet_buf[ETHERNET_BUF_MAX_SIZE] = {0};
/* network information */
wiz_NetInfo default_net_info = {
    .mac = {0x12, 0x14, 0x88, 0x77, 0x76, 0x81},
    .ip = {192, 168, 49, 119},
    .gw = {192, 168, 49, 1},
    .sn = {255, 255, 255, 0}};

static void wizchip_cris_enter(void)
{
    taskENTER_CRITICAL();
}

static void wizchip_cris_exit(void)
{
    taskEXIT_CRITICAL();
}

static void wizchip_cs_select(void)
{
    // 默认cs是高电平，
    HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_RESET);
}

static void wizchip_cs_deselect(void)
{
    HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_SET);
}

static uint8_t wizchip_spi_readbyte(void)
{
    uint8_t r_byte = 0;
    HAL_SPI_Receive(&hspi2, &r_byte, 1, 1000);
    return r_byte;
}

static void wizchip_spi_writebyte(uint8_t wb)
{
    HAL_SPI_Transmit(&hspi2, &wb, 1, 1000);
}

// 注册W5500回调
void wizchip_spi_cb_reg()
{
    reg_wizchip_cs_cbfunc(wizchip_cs_select, wizchip_cs_deselect);
    reg_wizchip_spi_cbfunc(wizchip_spi_readbyte, wizchip_spi_writebyte);
    reg_wizchip_cris_cbfunc(wizchip_cris_enter, wizchip_cris_exit);
}

// 片选复位
void wizchip_reset()
{
    HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(10);
}

static void print_network_info(void)
{
    uint8_t curip[4] = {0};
    uint8_t curgw[4] = {0};
    uint8_t curmask[4] = {0};
    uint8_t curmac[6] = {0};
    getGAR(curgw);
    getSUBR(curmask);
    getSHAR(curmac);
    getSIPR(curip);
    printf("w5500 ip:%d.%d.%d.%d\n", curip[0], curip[1], curip[2], curip[3]);
    printf("w5500 gw:%d.%d.%d.%d\n", curgw[0], curgw[1], curgw[2], curgw[3]);
    printf("w5500 mask:%d.%d.%d.%d\n", curmask[0], curmask[1], curmask[2], curmask[3]);
    // 打印当前mac
    printf("w5500 mac:%02x:%02x:%02x:%02x:%02x:%02x\n", curmac[0], curmac[1], curmac[2], curmac[3], curmac[4], curmac[5]);
}

void Int_W5500_Init(void)
{
    uint8_t tx_mem_conf[8] = {2, 2, 2, 2, 2, 2, 2, 2}; // 每个Socket 2KB TX 内存
    uint8_t rx_mem_conf[8] = {2, 2, 2, 2, 2, 2, 2, 2}; // 每个Socket 2KB RX 内存
    // 片选复位
    wizchip_reset();
    // 注册回调
    wizchip_spi_cb_reg();
    // ↓↓↓↓ 确认这部分代码是否已经添加？ ↓↓↓↓
    if (wizchip_init(tx_mem_conf, rx_mem_conf) != 0)
    {
        printf("W5500 Socket Memory Allocation Failed!\r\n");
        while (1); // 初始化失败，停机
    }
    // ↑↑↑↑ 确认这部分代码是否已经添加？ ↑↑↑↑
    wizchip_setnetinfo(&default_net_info);
    //测试入网是否成功
    HAL_Delay(8000);
    print_network_info();
}
