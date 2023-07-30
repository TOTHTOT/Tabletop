/*
 * @Description: w25qxx 的驱动代码
 * @Author: TOTHTOT
 * @Date: 2023-07-18 21:32:17
 * @LastEditTime: 2023-07-30 16:35:26
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\Learn\stm32\My_Project\Tabletop\Code\STM32F103C8T6(HAL+FreeRTOS)\HARDWARE\W25QXX\w25qxx.h
 */
#ifndef __W25QXX_H__
#define __W25QXX_H__

#include "main.h"

// 指令表
#define W25X_WriteEnable 0x06
#define W25X_WriteDisable 0x04
#define W25X_ReadStatusReg1 0x05
#define W25X_ReadStatusReg2 0x35
#define W25X_ReadStatusReg3 0x15
#define W25X_WriteStatusReg1 0x01
#define W25X_WriteStatusReg2 0x31
#define W25X_WriteStatusReg3 0x11
#define W25X_ReadData 0x03
#define W25X_FastReadData 0x0B
#define W25X_FastReadDual 0x3B
#define W25X_PageProgram 0x02
#define W25X_BlockErase 0xD8
#define W25X_SectorErase 0x20
#define W25X_ChipErase 0xC7
#define W25X_PowerDown 0xB9
#define W25X_ReleasePowerDown 0xAB
#define W25X_DeviceID 0xAB
#define W25X_ManufactDeviceID 0x90
#define W25X_JedecDeviceID 0x9F
#define W25X_Enable4ByteAddr 0xB7
#define W25X_Exit4ByteAddr 0xE9

/* 结构体定义 */
typedef struct w25qxx_device_t w25qxx_device_t;
#pragma pack(1)
struct w25qxx_device_t
{
#define W25QXX_READ_BUF_SIZE 4096
    uint8_t read_buf[W25QXX_READ_BUF_SIZE]; // 读缓存

// W25X系列Q系列芯片列表
#define W25Q80 0XEF13     // W25Q80  ID
#define W25Q16 0XEF14     // W25Q16  ID
#define W25Q32 0XEF15     // W25Q32  ID
#define W25Q64 0XEF16     // W25Q64  ID
#define W25Q128 0XEF17    // W25Q128 ID
#define W25Q256 0XEF18    // W25Q256 ID
    uint32_t device_type; // 设备类型

    /* 保存在 flash 中的数据有一下:
    1. WiFi的图标;
    2. 天气图标, 很多个;
    3. 历史的温湿度数据一天存100个;
    4. 各个组件的位置信息(未实现);
    5. 连接的WIFI以及密码; */
    struct save_data_t
    {
#define FLASH_WiFi_ICON_ADDER 0x00000000
#define FLASH_WiFi_ICON_SIZE 0x2000
#define FLASH_WEATHER_ICON_ADDER 0x2000
#define FLASH_WEATHER_ICON_SIZE 0x2000
#define FLASH_HISTORY_DATA_ADDER

        char a;

    } save_data_st;

    /* 函数指针底层驱动, 用户提供 */
    uint8_t (*wr_callback)(uint8_t send_data); // 底层读写函数指针
    int (*cs_ctrl_callback)(uint8_t state);    // 片选控制
    void (*set_speed_callback)(uint8_t speed); // 设置速度
    /* 函数指针, 外部使用 */
    void (*erase_sector_cb)(uint32_t Dst_Addr, w25qxx_device_t *dev);                                           // 擦除扇区
    void (*write_data_cb)(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite, w25qxx_device_t *dev); // 写入数据
    void (*read_data_cb)(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead, w25qxx_device_t *dev);    // 读取数据
};
#pragma pack()
/* 全局变量 */
extern w25qxx_device_t g_w25qxx_dev;

/* 全局函数 */
uint32_t w25qxx_init(w25qxx_device_t *dev);

#endif /* __W25QXX_H__ */
