/*
 * @Description: ds1307 驱动代码
 * @Author: TOTHTOT
 * @Date: 2023-07-23 14:59:53
 * @LastEditTime: 2023-08-13 22:17:57
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\Learn\stm32\My_Project\Tabletop\Code\STM32F103C8T6(HAL+FreeRTOS)\HARDWARE\DS1307\ds1307.h
 */
#ifndef __DS1307_H__
#define __DS1307_H__

#include "main.h"
#include "GUI_Paint.h"
/* DS1307 的相关指令 */
#define DS1307_I2C_ADDR 0x68
#define DS1307_REG_SECOND 0x00
#define DS1307_REG_MINUTE 0x01
#define DS1307_REG_HOUR 0x02
#define DS1307_REG_DOW 0x03
#define DS1307_REG_DATE 0x04
#define DS1307_REG_MONTH 0x05
#define DS1307_REG_YEAR 0x06
#define DS1307_REG_CONTROL 0x07
#define DS1307_REG_UTC_HR 0x08
#define DS1307_REG_UTC_MIN 0x09
#define DS1307_REG_CENT 0x10
#define DS1307_REG_RAM 0x11

// #define REDEFINE_TIME 1

typedef struct ds1307_device ds1307_device_t;
struct ds1307_device
{
    // 时间结构体来自 GUI_Paint.h, 为了兼容GUI的时间刷新函数,
    // 指向墨水屏设备里的时间结构体。
    PAINT_TIME *current_time;

    /* 用户实现的回调函数 */
    uint8_t (*set_reg_byte_cb)(uint8_t reg_adder, uint8_t value);
    uint8_t (*get_reg_byte_cb)(uint8_t reg_addr, uint8_t *reg_value);
    /* 外部函数 */
    uint8_t (*get_time)(ds1307_device_t *dev);
    uint8_t (*set_time)(ds1307_device_t *dev, PAINT_TIME *current_time);
    void (*printf_time)(PAINT_TIME *time_st);
    void (*time_increase)(PAINT_TIME *time_st);
};

/* 全局变量 */
extern ds1307_device_t g_ds1307_dev_st;

/* 全局函数 */
uint8_t ds1307_init(ds1307_device_t *dev, PAINT_TIME *time);

#endif /* __DS1307_H__ */
