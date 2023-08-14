/*
 * @Description: ds1307 驱动代码
 * @Author: TOTHTOT
 * @Date: 2023-07-23 14:59:44
 * @LastEditTime: 2023-08-14 19:38:49
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
/**
 * @name: ds1307_set_reg_byte
 * @msg: ds1307设置寄存器值
 * @param {uint8_t} reg_adder 寄存器地址
 * @param {uint8_t} value 设置的值
 * @return {0} 成功
 * @return {!= 0} 失败
 * @author: TOTHTOT
 * @date: 2023年8月13日
 */
uint8_t ds1307_set_reg_byte(uint8_t reg_adder, uint8_t value)
{
    uint8_t data[2] = {reg_adder, value};
    return HAL_I2C_Master_Transmit(&hi2c1, DS1307_I2C_ADDR << 1, &data[0], 2, 1000);
}

/**
 * @name: ds1307_get_reg_byte
 * @msg: ds1307获取寄存器值
 * @param {uint8_t} reg_addr 寄存器地址
 * @param {uint8_t} *reg_value 获取值得变量
 * @return {0} 成功
 * @return {!=0} 失败
 * @author: TOTHTOT
 * @date:2023年8月13日
 */
uint8_t ds1307_get_reg_byte(uint8_t reg_addr, uint8_t *reg_value)
{
    uint8_t val;
    HAL_StatusTypeDef hal_ret = HAL_OK;
    uint8_t ret = 0;

    hal_ret = HAL_I2C_Master_Transmit(&hi2c1, DS1307_I2C_ADDR << 1, &reg_addr, 1, 1000);
    if (hal_ret != HAL_OK)
    {
        ret = 1;
    }

    hal_ret = HAL_I2C_Master_Receive(&hi2c1, DS1307_I2C_ADDR << 1, &val, 1, 1000);
    if (hal_ret != HAL_OK)
    {
        ret = 2;
    }

    *reg_value = val;

    return ret;
}
/* 用户实现代码结束 */

uint8_t ds1307_set_time_zone(ds1307_device_t *dev, int8_t hr, uint8_t min)
{
    uint8_t ret = 0;

    ret = dev->set_reg_byte_cb(DS1307_REG_UTC_HR, hr);
    ret = dev->set_reg_byte_cb(DS1307_REG_UTC_HR, min);

    return ret;
}
/**
 * @name: ds1307_set_clock_halt
 * @msg: 设置时钟停止位。
 * @param {ds1307_device_t} *dev 设备
 * @param {uint8_t} halt 停止位
 * @return {0} 成功
 * @return {1, 2} 失败
 * @author: TOTHTOT
 * @date: 2023年8月13日
 */
uint8_t ds1307_set_clock_halt(ds1307_device_t *dev, uint8_t halt)
{
    uint8_t ch = (halt ? 1 << 7 : 0);
    uint8_t reg_value = 0;
    uint8_t hal_ret = HAL_OK;
    //   uint8_t ret = 0;

    hal_ret = dev->get_reg_byte_cb(DS1307_REG_SECOND, &reg_value);
    if (hal_ret != HAL_OK)
    {
        return 1;
    }

    hal_ret = dev->set_reg_byte_cb(DS1307_REG_SECOND, ch | (reg_value & 0x7f));
    if (hal_ret != HAL_OK)
    {
        return 2;
    }

    return 0;
}

/**
 * @name: ds1307_get_clock_halt
 * @msg: 获取时钟停止位。
 * @param {ds1307_device_t} *dev 设备
 * @param {uint8_t} *clock_halt_value 值
 * @return {0} 成功
 * @return {1} 失败
 * @author: TOTHTOT
 * @date: 2023年8月13日
 */
uint8_t ds1307_get_clock_halt(ds1307_device_t *dev, uint8_t *clock_halt_value)
{
    uint8_t reg_value = 0;
    uint8_t ret = 0;

    ret = dev->get_reg_byte_cb(DS1307_REG_SECOND, &reg_value);
    if (ret != HAL_OK)
    {
        return 1;
    }

    *clock_halt_value = (reg_value & 0x80) >> 7;

    return 0;
}

/**
 * @name: ds1307_encode_bcd
 * @msg: bcd 转码
 * @param {uint8_t} dec 值
 * @return {转码值}
 * @author: TOTHTOT
 * @date: 2023年8月13日
 */
uint8_t ds1307_encode_bcd(uint8_t dec)
{
    return (dec % 10 + ((dec / 10) << 4));
}
uint8_t ds1307_decode_bcd(uint8_t bin)
{
    return (((bin & 0xf0) >> 4) * 10) + (bin & 0x0f);
}
/* 以下为设置时间相关函数 */
uint8_t ds1307_set_sec(ds1307_device_t *dev, uint8_t second)
{
    uint8_t clock_halt_value = 0;
    uint8_t ch = ds1307_get_clock_halt(dev, &clock_halt_value);

    return dev->set_reg_byte_cb(DS1307_REG_SECOND, ds1307_encode_bcd(second | ch));
}

uint8_t ds1307_get_sec(ds1307_device_t *dev, uint8_t *sec)
{
    uint8_t reg_value = 0;

    uint8_t ret = dev->get_reg_byte_cb(DS1307_REG_SECOND, &reg_value);
    if (ret != HAL_OK)
    {
        return 1;
    }
    *sec = ds1307_encode_bcd(reg_value & 0x7f);
    return 0;
}

/**
 * @name: ds1307_set_time
 * @msg: 设置时间
 * @param {ds1307_device_t} *dev
 * @param {PAINT_TIME} *time_st
 * @return {0} 成功
 * @return {1} 失败
 * @author: TOTHTOT
 * @date: 2023年8月13日
 */
uint8_t ds1307_set_time(ds1307_device_t *dev, PAINT_TIME *time_st)
{
    if (dev->set_reg_byte_cb(DS1307_REG_SECOND, time_st->Sec) != 0)
        return 1;
    dev->set_reg_byte_cb(DS1307_REG_MINUTE, ds1307_encode_bcd(time_st->Min));
    dev->set_reg_byte_cb(DS1307_REG_HOUR, ds1307_encode_bcd(time_st->Hour));
    dev->set_reg_byte_cb(DS1307_REG_DATE, ds1307_encode_bcd(time_st->Day));
    dev->set_reg_byte_cb(DS1307_REG_MONTH, ds1307_encode_bcd(time_st->Month));
    dev->set_reg_byte_cb(DS1307_REG_CENT, ds1307_encode_bcd(time_st->Year / 100));
    dev->set_reg_byte_cb(DS1307_REG_YEAR, ds1307_encode_bcd(time_st->Year % 100));

    return 0;
}
/**
 * @name: ds1307_get_time
 * @msg: 获取时间, 时间保存在结构体的是时间中
 * @param {ds1307_device_t} *dev
 * @return {0} 成功
 * @return {1} 失败
 * @author: TOTHTOT
 * @date: 2023年8月13日
 */
uint8_t ds1307_get_time(ds1307_device_t *dev)
{
    uint8_t reg_value = 0;

    if (dev->get_reg_byte_cb(DS1307_REG_SECOND, &reg_value) != 0)
        return 1;

    dev->current_time->Sec = ds1307_decode_bcd(reg_value);

    dev->get_reg_byte_cb(DS1307_REG_MINUTE, &reg_value);
    dev->current_time->Min = ds1307_decode_bcd(reg_value);

    dev->get_reg_byte_cb(DS1307_REG_HOUR, &reg_value);
    dev->current_time->Hour = ds1307_decode_bcd(reg_value);

    dev->get_reg_byte_cb(DS1307_REG_DATE, &reg_value);
    dev->current_time->Day = ds1307_decode_bcd(reg_value);

    dev->get_reg_byte_cb(DS1307_REG_MONTH, &reg_value);
    dev->current_time->Month = ds1307_decode_bcd(reg_value);

    dev->get_reg_byte_cb(DS1307_REG_YEAR, &reg_value);
    dev->current_time->Year = ds1307_decode_bcd(reg_value);

    return 0;
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
    INFO_PRINT("date[%4d:%2d:%2d]\r\n", time->Year, time->Month, time->Day);
    INFO_PRINT("time[%2d:%2d:%2d]\r\n", time->Hour, time->Min, time->Sec);
}

/**
 * @name: ds1307_time_increase
 * @msg: 系统时间增加, 系统内部使用,与ds1307无关
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

    dev->current_time = time;
    /* 函数指针初始化 */
    dev->get_reg_byte_cb = ds1307_get_reg_byte;
    dev->set_reg_byte_cb = ds1307_set_reg_byte;
    dev->set_time = ds1307_set_time;
    dev->get_time = ds1307_get_time;
    dev->printf_time = print_current_time;
    dev->time_increase = ds1307_time_increase;
#ifdef REDEFINE_TIME
    ret = ds1307_set_clock_halt(dev, 0);
    ret = ds1307_set_time_zone(dev, 8, 0);
    ret = ds1307_set_time(dev, time);
    if (ret != 0)
    {
        ERROR_PRINT("ds1307_set_time() fault[%d]\r\n", ret);
    }
#endif /* REDEFINE_TIME */

#if 0
    ret = ds1307_get_time(dev, time);
    ret = ds1307_set_sec(dev, 12);
    if (ret != 0)
    {
        ERROR_PRINT("ds1307_set_sec() fault[%d]\r\n", ret);
    }

    delay_xms(1000);
    ret = ds1307_get_sec(dev, &sec);
    if (ret != 0)
    {
        ERROR_PRINT("ds1307_get_sec() fault[%d]\r\n", ret);
    }

    INFO_PRINT("sec = %d\r\n", sec);
#endif
    
    /* 上电获取时间 */
    ret = dev->get_time(dev);
    dev->printf_time(time);

    return ret;
}
