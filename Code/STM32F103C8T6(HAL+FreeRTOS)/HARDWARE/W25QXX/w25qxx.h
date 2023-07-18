/*
 * @Description: w25qxx 的驱动代码
 * @Author: TOTHTOT
 * @Date: 2023-07-18 21:32:17
 * @LastEditTime: 2023-07-18 22:31:55
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

/* 宏定义 */
// #define W25QXX_CS_SET HAL_GPIO_WritePin()

typedef struct
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

    /* 函数指针 */
    uint8_t (*wr_callback)(uint8_t send_data);      // 底层读写函数指针
    int (*cs_ctrl_callback)(uint8_t state); // 片选控制

} w25qxx_device_t;

/* 全局变量 */
extern w25qxx_device_t g_w25qxx_dev;

/* 全局函数 */
void w25qxx_wait_busy(w25qxx_device_t *dev);
void w25qxx_erase_sector(uint32_t Dst_Addr, w25qxx_device_t *dev);

#endif /* __W25QXX_H__ */
