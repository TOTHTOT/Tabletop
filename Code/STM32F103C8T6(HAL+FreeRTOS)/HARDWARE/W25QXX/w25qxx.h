/*
 * @Description: w25qxx 的驱动代码
 * @Author: TOTHTOT
 * @Date: 2023-07-18 21:32:17
 * @LastEditTime: 2023-08-02 20:59:49
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
typedef struct sensor_data_t sensor_data_t;

#pragma pack(1)
    typedef enum {
        W25QXX_STATE_NONE,
        W25QXX_STATE_ONLINE,
        W25QXX_STATE_OFFLINE,
        W25QXX_TOTAL_STATE
    } w25qxx_state_t;

struct w25qxx_device_t
{
#define W25QXX_READ_BUF_SIZE 4096
    uint8_t read_buf[W25QXX_READ_BUF_SIZE]; // 读缓存

// W25X系列Q系列芯片列表
#define W25Q80 0XEF13        // W25Q80  ID
#define W25Q16 0XEF14        // W25Q16  ID
#define W25Q32 0XEF15        // W25Q32  ID
#define W25Q64 0XEF16        // W25Q64  ID
#define W25Q128 0XEF17       // W25Q128 ID
#define W25Q256 0XEF18       // W25Q256 ID
    uint32_t device_type;    // 设备类型
    w25qxx_state_t state_em; // 设备状态

    // 保存写写入到flash的格式
    struct save_data_t
    {
// 标志参数信息
#define W25QXX_FLAG_ADDER 0x00000000
#define W25QXX_FLAG_DATA "OK"
#define W25QXX_FLAG_LEN 2
// 历史数据信息
#define W25QXX_DATA_ADDER (W25QXX_FLAG_ADDER + W25QXX_FLAG_LEN)
#define W25QXX_DATA_MAX_DAYS 10   // 保存的天数最大值
#define W25QXX_DATA_MAX_NUM 100   // 一天保存数据最大数目
#define W25QXX_SENSOR_HEAD_SIZE 4 // 一天信息中的头部信息大小 字节
        // 系统的配置参数, 在 W25QXX_FLAG_ADDER 后面, 这个结构体总体大小小于500字节!!
        struct save_data_config
        {
            uint8_t wifi_name[20];
            uint8_t wifi_password[25];
            uint16_t current_write_counter; // 当前写入到 flash 中的数量, 每写一次就加1, 每过一天就重置为0 小于W25QXX_DATA_MAX_NUM
            uint32_t current_write_day;     // 当前写入的天数等于0时写入在, 写入地址计算公式如下:
                                            // (W25QXX_DATA_MAX_NUM*sizeof(sensor_data_t)*(current_write_day%W25QXX_DATA_MAX_DAYS)) + W25QXX_DATA_ADDER + (W25QXX_SENSOR_HEAD_SIZE*(current_write_day%W25QXX_DATA_MAX_DAYS)) + current_write_counter*sizeof(sensor_data_t)
                                            // current_write_day 每过一天加1, 永不清0, 上电时与前一条的数据的日期对比, 如果大于就+1
                                            // 等于就继续写, 小于就覆盖上一条数据.
        } system_config_st;

        // 写入传感器数据到flash格式
        // 头四个字节分别表示:年(2 byte), 月(1 byte), 日(1 byte), 后面紧跟着 W25QXX_DATA_MAX_NUM 个传感器数据
        struct one_day_sensor_data_t
        {
            uint16_t year;
            uint8_t mon;
            uint8_t day;
            struct sensor_data_t
            {
                uint8_t temp;
                uint8_t humi;
            } sensor_data_st[W25QXX_DATA_MAX_NUM];
        } one_day_data_t;
    } save_data_st;

    /* 函数指针底层驱动, 用户提供 */
    uint8_t (*wr_callback)(uint8_t send_data); // 底层读写函数指针
    int (*cs_ctrl_callback)(uint8_t state);    // 片选控制
    void (*set_speed_callback)(uint8_t speed); // 设置速度
    /* 函数指针, 外部使用 */
    void (*erase_sector_cb)(uint32_t Dst_Addr, w25qxx_device_t *dev);                                                 // 擦除扇区
    void (*write_data_cb)(const uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite, w25qxx_device_t *dev); // 写入数据
    void (*read_data_cb)(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead, w25qxx_device_t *dev);          // 读取数据
    uint8_t (*check_flag_cb)(w25qxx_device_t *dev, uint32_t flag_adder, const uint8_t *flag, uint8_t flag_len);       // 检查标志位是否有效
    uint32_t (*write_sensor_adder_cb)(uint32_t current_write_counter, uint32_t current_write_day);                    // 传感器写入地址计算
};
#pragma pack()
/* 全局变量 */
extern w25qxx_device_t g_w25qxx_dev_st;

/* 全局函数 */
uint32_t w25qxx_init(w25qxx_device_t *dev);

#endif /* __W25QXX_H__ */
