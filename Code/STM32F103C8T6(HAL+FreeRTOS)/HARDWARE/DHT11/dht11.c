/*
 * @Description: dht11 qudong
 * @Author: TOTHTOT
 * @Date: 2023-08-06 08:40:29
 * @LastEditTime: 2023-08-06 17:59:29
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\Learn\stm32\My_Project\Tabletop\Code\STM32F103C8T6(HAL+FreeRTOS)\HARDWARE\DHT11\dht11.c
 */
#include "dht11.h"
#include "usart1.h"
#include "delay.h"
#include "gpio.h"
#include <stdio.h>

/* 内部调用函数 */
void dht11_io_in(dht11_dev_t *dev);
void dht11_io_out(dht11_dev_t *dev);
void dht11_reset(dht11_dev_t *dev);
uint8_t dht11_read_bit(dht11_dev_t *dev);
uint8_t dht11_read_byte(dht11_dev_t *dev);
uint8_t dht11_check(dht11_dev_t *dev);

/* 全局变量定义 */
dht11_dev_t g_dht11_device_st = {0};

/* 用户实现代码开始 */

/**
 * @name: dht11_io_ctrl
 * @msg: IO口方向控制, 如果是读的话返回的是io电平
 * @param {dht11_io_ctrl_t} in_out io方向控制
 * @param {uint8_t} io_state io输出电平
 * @return {0/1} 读取的电平
 * @author: TOTHTOT
 * @date: 2023年8月6日
 */
uint8_t dht11_io_ctrl(dht11_io_ctrl_t in_out, uint8_t io_state)
{
    uint8_t ret = 0;
    GPIO_InitTypeDef GPIO_InitStructure;

    // INFO_PRINT("io = %d\r\n", in_out);

    switch (in_out)
    {
    case DHT11_IO_INPUT:
        GPIO_InitStructure.Pin = DHT11_DAT_Pin;
        GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
        HAL_GPIO_Init(DHT11_DAT_GPIO_Port, &GPIO_InitStructure);
        ret = HAL_GPIO_ReadPin(DHT11_DAT_GPIO_Port, DHT11_DAT_Pin);
        // INFO_PRINT("ret = %d\r\n", ret);
        break;
    case DHT11_IO_OUTPUT:
        GPIO_InitStructure.Pin = DHT11_DAT_Pin;
        GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
        HAL_GPIO_Init(DHT11_DAT_GPIO_Port, &GPIO_InitStructure);
        if (io_state == 1)
            HAL_GPIO_WritePin(DHT11_DAT_GPIO_Port, DHT11_DAT_Pin, GPIO_PIN_SET);
        else
            HAL_GPIO_WritePin(DHT11_DAT_GPIO_Port, DHT11_DAT_Pin, GPIO_PIN_RESET);
        break;
    default:
        ret = 0xff;
        break;
    }

    return ret;
}

/* 用户实现代码结束 */

/**
 * @name: dht11_reset
 * @msg: 复位 DHT11
 * @param {dht11_dev_t} *dev dht11 设备
 * @return {*} 无
 * @author: TOTHTOT
 * @date: 2023年3月12日19:54:08
 */
void dht11_reset(dht11_dev_t *dev)
{
    dev->io_ctrl_cb(DHT11_IO_OUTPUT, 0); // 拉低
    delay_ms(20);                       // 至少18ms
    dev->io_ctrl_cb(DHT11_IO_OUTPUT, 1); // 拉高
    delay_us(30);                        // 至少20~40us
}

/**
 * @name: dht11_check
 * @msg: 检测dht11是否在线
 * @param {dht11_dev_t} *dev dht11 设备
 * @return {*} == 0 在线; == 1 不在线;
 * @author: TOTHTOT
 * @date:
 */
uint8_t dht11_check(dht11_dev_t *dev)
{
    uint8_t retry = 0;

    while ((dev->io_ctrl_cb(DHT11_IO_INPUT, 0) == 0) && retry < DHT11_RETRY_TIMES)
    {
        // 拉低40~80us
        retry++;
        delay_us(1);
    };

    if (retry >= DHT11_RETRY_TIMES)
    {
        return 1;
    }

    retry = 0;
    while ((dev->io_ctrl_cb(DHT11_IO_INPUT, 0) == 1) && retry < DHT11_RETRY_TIMES)
    {
        // 拉高40~80us
        retry++;
        delay_us(1);
    };
    if (retry >= DHT11_RETRY_TIMES)
    {
        return 2;
    }
    return 0; // 检测到DHT11返回0
}

/**
 * @name: dht11_read_bit
 * @msg: dht11 读取一位
 * @param {dht11_dev_t} *dev dht11 设备
 * @return {*}  读取到的数据
 * @author: TOTHTOT
 * @date: 2023年3月12日20:01:20
 */
uint8_t dht11_read_bit(dht11_dev_t *dev)
{
    uint8_t retry = 0;
    while (dev->io_ctrl_cb(DHT11_IO_INPUT, 0) && retry < 100)
    { // 等待变为低电平
        retry++;
        delay_us(1);
    }
    retry = 0;
    while (!dev->io_ctrl_cb(DHT11_IO_INPUT, 0) && retry < 100)
    { // 等待变为高电平
        retry++;
        delay_us(1);
    }
    delay_us(40); // 等待40us
    if (dev->io_ctrl_cb(DHT11_IO_INPUT, 0))
        return 1;
    else
        return 0;
}

/**
 * @name: dht11_read_byte
 * @msg: dht11 读取一个字节
 * @param {dht11_dev_t} *dev dht11 设备
 * @return {*} 读取到的字节
 * @author: TOTHTOT
 * @date: 2023年3月12日20:00:51
 */
uint8_t dht11_read_byte(dht11_dev_t *dev)
{
    uint8_t i, dat;
    dat = 0;
    for (i = 0; i < 8; i++)
    {
        dat <<= 1;
        dat |= dht11_read_bit(dev);
    }
    return dat;
}

/**
 * @name: dht11_read_data
 * @msg: dht11 读取数据
 * @param {dht11_dev_t} *dev dht11 设备
 * @return {*} == 1 失败; == 2 失败; == 0 成功;
 * @author: TOTHTOT
 * @date: 2023年3月12日20:04:37
 */
uint8_t dht11_read_data(dht11_dev_t *dev)
{
    uint8_t buf[5];
    uint8_t i;

    if (dev->state_em != DHT11_STATUS_ONLINE)
        return 2;
    
    INFO_PRINT("read data\r\n");    // 2023年8月6日17:51:45 测试版本,不知道为什么不加这个打印就直接return 1;退出了
    dht11_reset(dev);
    if (dht11_check(dev) == 0)
    {
        for (i = 0; i < 5; i++)
        {
            buf[i] = dht11_read_byte(dev);
        }
        if ((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])
        {
            dev->humi = buf[0] ;
            dev->temp = buf[2];
        }
    }
    else
        return 1;
    return 0;
}

/**
 * @name: dht11_init
 * @msg: dht11 的初始化
 * @param {dht11_dev_t} *dev dht11 设备
 * @return {*} == 1, 失败; == 0 成功
 * @author: TOTHTOT
 * @date: 2023年3月12日20:05:53
 */
uint8_t dht11_init(dht11_dev_t *dev)
{

    dev->io_ctrl_cb = dht11_io_ctrl;
    dev->read_data_cb = dht11_read_data;

    dht11_reset(dev);

    return dht11_check(dev);
}
