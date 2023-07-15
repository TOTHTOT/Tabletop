/*
 * @Description: sht30 驱动文件
 * @Author: TOTHTOT
 * @Date: 2023-07-14 21:47:00
 * @LastEditTime: 2023-07-15 09:32:04
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\Learn\stm32\My_Project\Tabletop\Code\STM32F103C8T6(HAL+FreeRTOS)\HARDWARE\SHT30\sht30.h
 */
#ifndef __SHT30_H__

#include "main.h"
#include "i2c.h"

typedef enum
{
    /* 软件复位命令 */
    SOFT_RESET_CMD = 0x30A2,
    /*
    单次测量模式
    命名格式：Repeatability_CS_CMD
    CS： Clock stretching
    */
    HIGH_ENABLED_CMD = 0x2C06,
    MEDIUM_ENABLED_CMD = 0x2C0D,
    LOW_ENABLED_CMD = 0x2C10,
    HIGH_DISABLED_CMD = 0x2400,
    MEDIUM_DISABLED_CMD = 0x240B,
    LOW_DISABLED_CMD = 0x2416,

    /*
    周期测量模式
    命名格式：Repeatability_MPS_CMD
    MPS：measurement per second
    */
    HIGH_0_5_CMD = 0x2032,
    MEDIUM_0_5_CMD = 0x2024,
    LOW_0_5_CMD = 0x202F,
    HIGH_1_CMD = 0x2130,
    MEDIUM_1_CMD = 0x2126,
    LOW_1_CMD = 0x212D,
    HIGH_2_CMD = 0x2236,
    MEDIUM_2_CMD = 0x2220,
    LOW_2_CMD = 0x222B,
    HIGH_4_CMD = 0x2334,
    MEDIUM_4_CMD = 0x2322,
    LOW_4_CMD = 0x2329,
    HIGH_10_CMD = 0x2737,
    MEDIUM_10_CMD = 0x2721,
    LOW_10_CMD = 0x272A,
    /* 周期测量模式读取数据命令 */
    READOUT_FOR_PERIODIC_MODE = 0xE000,
} sht30_cmd_t;

typedef struct
{
/* ADDR Pin Conect to VSS */
#define SHT30_ADDR_WRITE 0x44 << 1      // 10001000
#define SHT30_ADDR_READ (0x44 << 1) + 1 // 10001011

    float temperatue;          // 采集到的温度
    float humidity;            // 采集到的湿度
    uint8_t sht30_read_buf[6]; // 从 sht30 中读取到的原始数据缓存
    // 需要实现的发送函数
    uint8_t (*iic_send_callback)(uint16_t adder, uint8_t *data, uint16_t data_len);
    // 需要实现的接收函数
    uint8_t (*iic_recv_callback)(uint16_t adder, uint8_t *data, uint16_t data_len);

} sht30_device_t;

/* 外部函数 */
int sht30_init(sht30_device_t *dev);
uint8_t sht30_data_conversion(sht30_device_t *dev);
uint8_t sht30_get_data(sht30_device_t *dev);
void sht30_reset(sht30_device_t *dev);

/* 全局变量 */
extern sht30_device_t g_sht_dev;
#endif /* __SHT30_H__ */
