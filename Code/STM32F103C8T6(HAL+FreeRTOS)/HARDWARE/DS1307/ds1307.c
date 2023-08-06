/*
 * @Description: ds1307 驱动代码
 * @Author: TOTHTOT
 * @Date: 2023-07-23 14:59:44
 * @LastEditTime: 2023-08-06 21:28:06
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\Learn\stm32\My_Project\Tabletop\Code\STM32F103C8T6(HAL+FreeRTOS)\HARDWARE\DS1307\ds1307.c
 */

#include "ds1307.h"
#include "usart1.h"
#include <stdio.h>
#include "i2c.h"
#include "delay.h"

/* 全局变量 */
ds1307_device_t g_ds1307_dev_st = {0};
/* 用户实现代码开始 */
uint8_t ds1307_set_reg_byte(uint8_t reg_adder, uint8_t value)
{
    uint8_t data[2] = {reg_adder, value};
    return HAL_I2C_Master_Transmit(&hi2c1, DS1307_I2C_ADDR, &data[0], 2, 1000);
}

uint8_t ds1307_get_reg_byte(uint8_t reg_addr)
{
    uint8_t val;

    HAL_I2C_Master_Transmit(&hi2c1, DS1307_I2C_ADDR << 1, &reg_addr, 1, 1000);
    HAL_I2C_Master_Receive(&hi2c1, DS1307_I2C_ADDR << 1, &val, 1, 1000);

    return val;
}
/* 用户实现代码结束 */

uint8_t ds1307_set_time_zone(ds1307_device_t *dev, int8_t hr, uint8_t min)
{
    uint8_t ret = 0;

    ret = dev->set_reg_byte_cb(DS1307_REG_UTC_HR, hr);
    ret = dev->set_reg_byte_cb(DS1307_REG_UTC_HR, min);

    return ret;
}

uint8_t ds1307_set_clock_halt(ds1307_device_t *dev, uint8_t halt)
{
    uint8_t ch = (halt ? 1 << 7 : 0);
    return dev->set_reg_byte_cb(DS1307_REG_SECOND, ch | (dev->get_reg_byte_cb(DS1307_REG_SECOND) & 0x7f));
}

uint8_t ds1307_get_clock_halt(ds1307_device_t *dev)
{
    return (dev->get_reg_byte_cb(DS1307_REG_SECOND) & 0x80) >> 7;
}

uint8_t ds1307_encode_bcd(uint8_t dec)
{
    return (dec % 10 + ((dec / 10) << 4));
}

void ds1307_set_sec(ds1307_device_t *dev, uint8_t second)
{
    uint8_t ch = ds1307_get_clock_halt(dev);
    dev->set_reg_byte_cb(DS1307_REG_SECOND, ds1307_encode_bcd(second | ch));
}

uint8_t ds1307_get_sec(ds1307_device_t *dev)
{
    return ds1307_encode_bcd(dev->get_reg_byte_cb(DS1307_REG_SECOND) & 0x7f);
}

uint16_t ds1307_get_time(ds1307_device_t *dev, PAINT_TIME *time_st)
{
}

/**
 * @name: print_current_time
 * @msg: 输出当前系统时间
 * @param {PAINT_TIME} *time 时间结构体
 * @return {无}
 * @author: TOTHTOT
 * @date: 2023年8月6日
 */
void print_current_time(PAINT_TIME *time)
{
    INFO_PRINT("time[%2d:%2d:%2d]\r\n", time->Hour, time->Min, time->Sec);
}

/**
 * @name: ds1307_time_increase
 * @msg: 系统时间增加
 * @param {PAINT_TIME} *time_st
 * @return {*}
 * @author: TOTHTOT
 * @date: 2023年8月1日
 */
void ds1307_time_increase(PAINT_TIME *time_st)
{
    uint8_t Month_Day_tab[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // 每月的天数

    time_st->Msec++;
    if (time_st->Msec >= 1) // 1s进入中断一次
    {
        time_st->Msec = 0L;
        time_st->Sec++; // 秒进位
        if (time_st->Sec >= 60)
        {
            time_st->Sec = 0;
            time_st->Min++; // 分钟进位
            if (time_st->Min >= 60)
            {
                time_st->Min = 0;
                time_st->Hour++; // 小时进位
                if (time_st->Hour >= 24)
                {
                    time_st->Hour = 0;
                    time_st->Day++; // 天进位
                    if (time_st->Day > (((time_st->Year % 4 == 0) && (time_st->Month == 2)) ? Month_Day_tab[time_st->Month] + 1 : Month_Day_tab[time_st->Month]))
                    {
                        time_st->Day = 1;
                        time_st->Month++; // 月进位
                        if (time_st->Month > 12)
                        {
                            time_st->Month = 1;
                            time_st->Year++; // 年进位
                            if (time_st->Year > 99)
                            {
                                time_st->Year = 0;
                            }
                        }
                    }
                }
            }
        }
    }
}

/**
 * @name: ds1307_init
 * @msg: 每次上电后都要读取一次ds1307的值然后刷新 Paint_Time结构体
 * @param {ds1307_device_t} *dev
 * @param {PAINT_TIME} *time
 * @return {*}
 * @author: TOTHTOT
 * @date:
 */
uint8_t ds1307_init(ds1307_device_t *dev, PAINT_TIME *time)
{
    uint8_t ret = 0;

    /* 函数指针初始化 */
    dev->get_reg_byte_cb = ds1307_get_reg_byte;
    dev->set_reg_byte_cb = ds1307_set_reg_byte;

    ds1307_set_clock_halt(dev, 0);
    ds1307_set_time_zone(dev, 8, 0);
    ds1307_set_sec(dev, 12);
    delay_xms(2000);
    printf("sec = %d\r\n", ds1307_get_sec(dev));
    return ret;
}
