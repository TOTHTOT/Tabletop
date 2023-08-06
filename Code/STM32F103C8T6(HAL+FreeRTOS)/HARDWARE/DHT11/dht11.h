/*
 * @Description: dht11 驱动
 * @Author: TOTHTOT
 * @Date: 2023-08-06 08:40:42
 * @LastEditTime: 2023-08-06 10:33:21
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\Learn\stm32\My_Project\Tabletop\Code\STM32F103C8T6(HAL+FreeRTOS)\HARDWARE\DHT11\dht11.h
 */
#ifndef __DHT11_H__
#define __DHT11_H__

#include <stdint.h>
#include "main.h"

/* 模块定义 */
// 模块状态
typedef enum dht11_state_t dht11_state_t;
enum dht11_state_t
{
    DHT11_STATUS_UNKNOWN, // 未知状态
    DHT11_STATUS_ONLINE,  // 在线状态
    DHT11_STATUS_OFFLINE, // 掉线状态
    DHT11_TOTAL_STATUS    // 默认状态
};
// io控制
typedef enum dht11_io_ctrl_t dht11_io_ctrl_t;
enum dht11_io_ctrl_t
{
    DHT11_IO_NONE,   // 无
    DHT11_IO_INPUT,  // 输入状态
    DHT11_IO_OUTPUT, // 输出状态
    DHT11_IO_TOTAL   // 状态统计
};

typedef struct dht11_dev_t dht11_dev_t;
struct dht11_dev_t
{
#define DHT11_RETRY_TIMES 200
    uint16_t temp;
    uint16_t humi;
    
    dht11_state_t state_em;

    /* 底层IO口函数指针 */
    uint8_t (*io_ctrl_cb)(dht11_io_ctrl_t in_out, uint8_t io_state);
    /* 外部函数指针 */
    uint8_t (*read_data_cb)(dht11_dev_t *dev);
};

/* 全局变量 */
extern dht11_dev_t g_dht11_device_st;

/* 全局函数 */
uint8_t dht11_init(dht11_dev_t *dev);

#endif /* __DHT11_H__ */
