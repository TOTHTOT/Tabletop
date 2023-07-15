/*
 * @Description: sht30 驱动文件
 * @Author: TOTHTOT
 * @Date: 2023-07-14 21:46:52
 * @LastEditTime: 2023-07-15 09:20:27
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\Learn\stm32\My_Project\Tabletop\Code\STM32F103C8T6(HAL+FreeRTOS)\HARDWARE\SHT30\sht30.c
 */
#include "sht30.h"
#include <string.h>

/* 全局变量 */
sht30_device_t g_sht_dev = {0};
/* 用户实现的iic Start */

static uint8_t iic_send(uint16_t adder, uint8_t *data, uint16_t data_len)
{
    return HAL_I2C_Master_Transmit(&hi2c1, adder, data, data_len, 0xFFFF);
}

static uint8_t iic_recv(uint16_t adder, uint8_t *data, uint16_t data_len)
{
    return HAL_I2C_Master_Receive(&hi2c1, adder, data, data_len, 0xFFFF);
}
/* 用户实现的iic End */

/**
 * @name: sht30_send_cmd
 * @msg: 发送命令
 * @param {sht30_cmd_t} cmd 命令
 * @param {sht30_device_t} *dev sht30设备
 * @return {*}
 * @author: TOTHTOT
 * @date: 2023年7月14日22:48:46
 */
static uint8_t sht30_send_cmd(sht30_cmd_t cmd, sht30_device_t *dev)
{
    uint8_t cmd_buffer[2];
    cmd_buffer[0] = cmd >> 8;
    cmd_buffer[1] = cmd;
    return dev->iic_send_callback(SHT30_ADDR_WRITE, cmd_buffer, sizeof(cmd_buffer));
}

/**
 * @name: sht30_read_data
 * @msg: 读取 sht30 数据
 * @param {sht30_device_t} *dev
 * @return {*}
 * @author: TOTHTOT
 * @date: 2023年7月15日09:07:32
 */
static uint8_t sht30_read_data(sht30_device_t *dev)
{
    return dev->iic_recv_callback(SHT30_ADDR_READ, dev->sht30_read_buf, sizeof(dev->sht30_read_buf));
}

/**
 * @name: sht30_reset
 * @msg: 复位sht30
 * @param {sht30_device_t} *dev sht30设备
 * @return {*}
 * @author: TOTHTOT
 * @date: 2023年7月15日08:40:22
 */
void sht30_reset(sht30_device_t *dev)
{
    sht30_send_cmd(SOFT_RESET_CMD, dev);
    HAL_Delay(20);
}

#define CRC8_POLYNOMIAL 0x31

/**
 * @name: sht30_check_crc8
 * @msg: 数据校验
 * @param {uint8_t} *message
 * @param {uint8_t} initial_value
 * @return {*}
 * @author: TOTHTOT
 * @date: 2023年7月15日08:42:31
 */
static uint8_t sht30_check_crc8(uint8_t *const message, uint8_t initial_value)
{
    uint8_t remainder;    // 余数
    uint8_t i = 0, j = 0; // 循环变量

    /* 初始化 */
    remainder = initial_value;

    for (j = 0; j < 2; j++)
    {
        remainder ^= message[j];

        /* 从最高位开始依次计算  */
        for (i = 0; i < 8; i++)
        {
            if (remainder & 0x80)
            {
                remainder = (remainder << 1) ^ CRC8_POLYNOMIAL;
            }
            else
            {
                remainder = (remainder << 1);
            }
        }
    }

    /* 返回计算的CRC码 */
    return remainder;
}

/**
 * @name: sht30_get_data
 * @msg: 读取一次 sht30 数据
 * @param {sht30_device_t} *dev 数据保存在结构体中
 * @return {*}
 * @author: TOTHTOT
 * @date: 2023年7月15日09:13:18
 */
uint8_t sht30_get_data(sht30_device_t *dev)
{
    uint8_t ret = 0;

    // 发送读取数据命令
    ret = sht30_send_cmd(READOUT_FOR_PERIODIC_MODE, dev);
    if (ret != HAL_OK)
    {
        return ret;
    }

    // 读取数据
    ret = sht30_read_data(dev);
    if (ret != HAL_OK)
    {
        return ret;
    }

    return ret;
}

/**
 * @name: sht30_data_conversion
 * @msg: 将SHT30接收的6个字节数据进行CRC校验，并转换为温度值和湿度值
 * @param {sht30_device_t} *dev sht30设备
 * @return {校验成功  —— 返回0
 * 			校验失败  —— 返回1，并设置温度值和湿度值为0}
 * @author: TOTHTOT
 * @date: 2023年7月15日09:15:16
 */
uint8_t sht30_data_conversion( sht30_device_t *dev)
{
    uint16_t recv_temperature = 0;
    uint16_t recv_humidity = 0;

    /* 校验温度数据和湿度数据是否接收正确 */
    if (sht30_check_crc8(dev->sht30_read_buf, 0xFF) != dev->sht30_read_buf[2] || sht30_check_crc8(&dev->sht30_read_buf[3], 0xFF) != dev->sht30_read_buf[5])
        return 1;

    /* 转换温度数据 */
    recv_temperature = ((uint16_t)dev->sht30_read_buf[0] << 8) | dev->sht30_read_buf[1];
    dev->temperatue = -45 + 175 * ((float)recv_temperature / 65535);

    /* 转换湿度数据 */
    recv_humidity = ((uint16_t)dev->sht30_read_buf[3] << 8) | dev->sht30_read_buf[4];
    dev->humidity = 100 * ((float)recv_humidity / 65535);

    return 0;
}

/**
 * @name:sht30_init
 * @msg:初始化 sht30
 * @return {*}
 * @author: TOTHTOT
 * @date:2023年7月15日08:57:50
 */
int sht30_init(sht30_device_t *dev)
{
    // 初始化数据
    dev->humidity = 0;
    dev->temperatue = 0;
    memset(dev->sht30_read_buf, 0, sizeof(dev->sht30_read_buf));

    // 初始化函数指针
    dev->iic_recv_callback = iic_recv;
    dev->iic_send_callback = iic_send;

    sht30_reset(dev);

    return sht30_send_cmd(MEDIUM_2_CMD, dev);
}
