/*
 * @Description: ds1307 驱动代码
 * @Author: TOTHTOT
 * @Date: 2023-07-23 14:59:44
 * @LastEditTime: 2023-07-23 15:26:48
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\Learn\stm32\My_Project\Tabletop\Code\STM32F103C8T6(HAL+FreeRTOS)\HARDWARE\DS1307\ds1307.c
 */

#include "ds1307.h"
#include "usart1.h"
#include <stdio.h>

/* 全局变量 */
ds1307_device_t g_ds1307_dev = {0};
/* 用户实现代码开始 */

/* 用户实现代码结束 */


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
 * @date: 
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
    
    

    return ret;
}
