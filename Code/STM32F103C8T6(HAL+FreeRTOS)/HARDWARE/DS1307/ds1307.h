/*
 * @Description: ds1307 驱动代码
 * @Author: TOTHTOT
 * @Date: 2023-07-23 14:59:53
 * @LastEditTime: 2023-07-23 15:03:51
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\Learn\stm32\My_Project\Tabletop\Code\STM32F103C8T6(HAL+FreeRTOS)\HARDWARE\DS1307\ds1307.h
 */
#ifndef __DS1307_H__
#define __DS1307_H__

#include "main.h"
#include "GUI_Paint.h"

typedef struct 
{
    // 时间结构体来自 GUI_Paint.h, 为了兼容GUI的时间刷新函数, 
    // 指向墨水屏设备里的时间结构体。
    PAINT_TIME *current_time;   

    /* 用户实现的回调函数 */

}ds1307_device_t;

/* 全局变量 */
extern ds1307_device_t g_ds1307_dev;

/* 全局函数 */
void ds1307_time_increase(PAINT_TIME *time_st);
void print_current_time(PAINT_TIME *time);


#endif /* __DS1307_H__ */

