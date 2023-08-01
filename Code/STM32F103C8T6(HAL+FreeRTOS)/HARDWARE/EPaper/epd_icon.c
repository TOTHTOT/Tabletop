/*
 * @Description: 墨水屏的默认图标
 * @Author: TOTHTOT
 * @Date: 2023-08-01 19:49:40
 * @LastEditTime: 2023-08-01 20:11:09
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\Learn\stm32\My_Project\Tabletop\Code\STM32F103C8T6(HAL+FreeRTOS)\HARDWARE\EPaper\epd_icon.c
 */
#include "fonts.h"
#include <stdint.h>

const uint8_t gImage_wifi_unlink_icon[288] = {/* 0X00,0X01,0X30,0X00,0X30,0X00, */
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0XFF,0XC0,0X00,0X00,0X00,0X0F,0XFF,0XFC,
0X00,0X00,0X00,0X7F,0XE3,0XFF,0X00,0X00,0X01,0XF8,0X00,0X0F,0XC0,0X00,0X03,0XE0,
0X00,0X01,0X80,0X00,0X0F,0X80,0X00,0X00,0X03,0X00,0X1E,0X00,0X00,0X00,0X1F,0XE0,
0X3C,0X00,0XFF,0X80,0X3F,0XF0,0X30,0X07,0XFF,0XF0,0X7F,0XF8,0X00,0X1F,0XE3,0XF8,
0XF7,0XFC,0X00,0X7C,0X00,0X39,0XE3,0X1E,0X00,0XF0,0X00,0X01,0XE0,0X1E,0X01,0XE0,
0X00,0X01,0XF0,0X3E,0X01,0X80,0X00,0X01,0XF8,0X3E,0X00,0X00,0X7F,0X81,0XF0,0X3E,
0X00,0X03,0XFF,0XE1,0XE0,0X1E,0X00,0X07,0XF3,0XF0,0XE3,0X1E,0X00,0X0F,0X00,0X7C,
0XFF,0XFC,0X00,0X0C,0X00,0X18,0X7F,0XF8,0X00,0X00,0X00,0X00,0X3F,0XF0,0X00,0X00,
0X00,0X00,0X1F,0XE0,0X00,0X00,0X00,0X00,0X03,0X00,0X00,0X00,0X3E,0X00,0X00,0X00,
0X00,0X00,0X7F,0X00,0X00,0X00,0X00,0X00,0X3E,0X00,0X00,0X00,0X00,0X00,0X1C,0X00,
0X00,0X00,0X00,0X00,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00
};
const uint8_t gImage_wifi_linked_icon[288] = {/* 0X00,0X01,0X30,0X00,0X30,0X00, */
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X3C,0X00,0X00,0X00,0X00,0X0F,0XFF,0XF0,0X00,0X00,0X00,0X3F,
0XFF,0XFC,0X00,0X00,0X01,0XFF,0X00,0XFF,0X00,0X00,0X03,0XF0,0X00,0X0F,0X80,0X00,
0X0F,0XC0,0X00,0X03,0X00,0X00,0X1F,0X00,0X00,0X00,0X0F,0X00,0X3C,0X00,0X18,0X00,
0X3F,0XE0,0X78,0X07,0XFF,0XC0,0X7F,0XF0,0X20,0X1F,0XFF,0XF0,0XFF,0XF8,0X00,0X7F,
0X01,0XF1,0XFF,0X98,0X00,0XF8,0X00,0X31,0XFF,0X0C,0X01,0XE0,0X00,0X03,0XFE,0X0C,
0X03,0XC0,0X00,0X03,0X8C,0X1C,0X01,0X00,0X00,0X03,0X80,0X3C,0X00,0X01,0XFF,0X83,
0XC0,0X7C,0X00,0X07,0XFF,0XE3,0XE0,0XFC,0X00,0X0F,0X83,0XF1,0XF1,0XFC,0X00,0X1E,
0X00,0X79,0XFF,0XF8,0X00,0X08,0X00,0X30,0XFF,0XF0,0X00,0X00,0X00,0X00,0X7F,0XE0,
0X00,0X00,0X00,0X00,0X1F,0XC0,0X00,0X00,0X3C,0X00,0X00,0X00,0X00,0X00,0X7E,0X00,
0X00,0X00,0X00,0X00,0X7E,0X00,0X00,0X00,0X00,0X00,0X3C,0X00,0X00,0X00,0X00,0X00,
0X18,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};
const uint8_t gImage_cloudy_icon[288] = {
    /* 0X00,0X01,0X30,0X00,0X30,0X00, */
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0XFE,
    0X00,
    0X00,
    0X00,
    0X00,
    0X03,
    0XFF,
    0X80,
    0X00,
    0X00,
    0X00,
    0X07,
    0XC7,
    0XE0,
    0X00,
    0X00,
    0X00,
    0X0F,
    0X00,
    0XE0,
    0X00,
    0X00,
    0X00,
    0X0E,
    0X00,
    0X70,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X38,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X3F,
    0X00,
    0X00,
    0X00,
    0X1F,
    0XE0,
    0X3F,
    0XC0,
    0X00,
    0X00,
    0X7F,
    0XF8,
    0X1F,
    0XE0,
    0X00,
    0X00,
    0XFF,
    0XFC,
    0X10,
    0X70,
    0X00,
    0X01,
    0XE0,
    0X1E,
    0X00,
    0X38,
    0X00,
    0X03,
    0XC0,
    0X0F,
    0X00,
    0X38,
    0X00,
    0X07,
    0X80,
    0X07,
    0X00,
    0X38,
    0X00,
    0X07,
    0X00,
    0X03,
    0X80,
    0X38,
    0X00,
    0XFF,
    0X00,
    0X03,
    0XFC,
    0X38,
    0X03,
    0XFE,
    0X00,
    0X03,
    0XFF,
    0X10,
    0X07,
    0XFE,
    0X00,
    0X03,
    0XFF,
    0X80,
    0X0F,
    0X8E,
    0X00,
    0X01,
    0XC7,
    0XC0,
    0X0E,
    0X02,
    0X00,
    0X01,
    0X01,
    0XC0,
    0X1C,
    0X00,
    0X00,
    0X00,
    0X01,
    0XC0,
    0X1C,
    0X00,
    0X00,
    0X00,
    0X00,
    0XE0,
    0X1C,
    0X00,
    0X00,
    0X00,
    0X00,
    0XE0,
    0X1C,
    0X00,
    0X00,
    0X00,
    0X00,
    0XE0,
    0X1E,
    0X00,
    0X00,
    0X00,
    0X01,
    0XE0,
    0X0E,
    0X00,
    0X00,
    0X00,
    0X01,
    0XC0,
    0X0F,
    0X00,
    0X00,
    0X00,
    0X07,
    0XC0,
    0X07,
    0XFF,
    0XFF,
    0XFF,
    0XFF,
    0X80,
    0X03,
    0XFF,
    0XFF,
    0XFF,
    0XFF,
    0X00,
    0X00,
    0XFF,
    0XFF,
    0XFF,
    0XFC,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
    0X00,
};
const uint8_t gImage_rain_icon[288] = { /* 0X00,0X01,0X30,0X00,0X30,0X00, */
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0XF0,0X00,0X00,
0X00,0X00,0X1F,0XFC,0X00,0X00,0X00,0X00,0X7F,0XFE,0X00,0X00,0X00,0X00,0XF8,0X0F,
0X00,0X00,0X00,0X00,0XF0,0X07,0X80,0X00,0X00,0X01,0XE0,0X03,0XC0,0X00,0X00,0X03,
0XC0,0X01,0XC0,0X00,0X00,0X1F,0X80,0X01,0XFC,0X00,0X00,0XFF,0X80,0X00,0XFF,0X80,
0X01,0XFF,0X80,0X00,0XFF,0XC0,0X03,0XE7,0X80,0X00,0XF3,0XE0,0X07,0X81,0X00,0X00,
0XC0,0XF0,0X07,0X00,0X00,0X00,0X00,0X70,0X07,0X00,0X00,0X00,0X00,0X70,0X0E,0X00,
0X00,0X00,0X00,0X70,0X07,0X00,0X00,0X00,0X00,0X70,0X07,0X00,0X00,0X00,0X00,0X70,
0X07,0X80,0X00,0X00,0X00,0XF0,0X03,0X80,0X00,0X00,0X01,0XE0,0X03,0XFF,0XFF,0XFF,
0XFF,0XE0,0X01,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,0X7F,0XFF,0XFF,0XFF,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X60,0X02,
0X00,0X00,0X00,0X00,0XE0,0X07,0X00,0X00,0X00,0X01,0XF0,0X0F,0X80,0X00,0X00,0X01,
0XF8,0X0F,0X80,0X00,0X00,0X01,0XF8,0X0F,0X80,0X00,0X00,0X01,0XF8,0X0F,0X80,0X00,
0X00,0X00,0XF0,0X07,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,};
const uint8_t gImage_snow_icon[288] = { /* 0X00,0X01,0X30,0X00,0X30,0X00, */
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0XF0,
0X00,0X00,0X00,0X00,0X1F,0XFC,0X00,0X00,0X00,0X00,0X7F,0XFE,0X00,0X00,0X00,0X00,
0XF8,0X0F,0X00,0X00,0X00,0X00,0XF0,0X07,0X80,0X00,0X00,0X01,0XE0,0X03,0XC0,0X00,
0X00,0X03,0XC0,0X01,0XC0,0X00,0X00,0X1F,0X80,0X01,0XFC,0X00,0X00,0XFF,0X80,0X00,
0XFF,0X80,0X01,0XFF,0X80,0X00,0XFF,0XC0,0X03,0XE7,0X80,0X00,0XF3,0XE0,0X07,0X81,
0X00,0X00,0XC0,0XF0,0X07,0X00,0X00,0X00,0X00,0X70,0X07,0X00,0X00,0X00,0X00,0X70,
0X0F,0X00,0X00,0X00,0X00,0X70,0X07,0X00,0X00,0X00,0X00,0X70,0X07,0X00,0X00,0X00,
0X00,0X70,0X07,0X80,0X00,0X00,0X00,0XF0,0X07,0X80,0X00,0X00,0X01,0XE0,0X03,0XC0,
0XC0,0X03,0X83,0XE0,0X01,0XC0,0XC0,0X03,0X83,0XC0,0X00,0X4E,0XCC,0X3B,0XB3,0X00,
0X00,0X0F,0XFC,0X3F,0XF8,0X00,0X00,0X07,0XF8,0X0F,0XF0,0X00,0X00,0X03,0XF0,0X07,
0XE0,0X00,0X00,0X07,0XF8,0X1F,0XE0,0X00,0X00,0X0F,0XFC,0X3F,0XF0,0X00,0X00,0X0E,
0XCC,0X3B,0XB0,0X00,0X00,0X00,0XC0,0X03,0X80,0X00,0X00,0X00,0XC0,0X03,0X80,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};
const uint8_t gImage_sunny_icon[288] = { /* 0X00,0X01,0X30,0X00,0X30,0X00, */
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X00,
0X00,0X00,0X00,0X00,0X01,0X80,0X00,0X00,0X00,0X00,0X03,0X80,0X00,0X00,0X00,0X00,
0X03,0X80,0X00,0X00,0X00,0X00,0X03,0X80,0X00,0X00,0X00,0X00,0X03,0X80,0X00,0X00,
0X00,0X60,0X01,0X80,0X06,0X00,0X00,0XF0,0X00,0X00,0X0F,0X00,0X00,0X78,0X00,0X00,
0X1E,0X00,0X00,0X78,0X00,0X00,0X3C,0X00,0X00,0X3C,0X00,0X00,0X38,0X00,0X00,0X08,
0X0F,0XF0,0X10,0X00,0X00,0X00,0X3F,0XFC,0X00,0X00,0X00,0X00,0X7F,0XFE,0X00,0X00,
0X00,0X00,0XF8,0X1F,0X00,0X00,0X00,0X01,0XE0,0X07,0X80,0X00,0X00,0X03,0XC0,0X03,
0XC0,0X00,0X00,0X03,0X80,0X01,0XC0,0X00,0X00,0X07,0X00,0X01,0XE0,0X00,0X00,0X07,
0X00,0X00,0XE0,0X00,0X0E,0X07,0X00,0X00,0XE0,0X70,0X3F,0X87,0X00,0X00,0XE1,0XF8,
0X3F,0X87,0X00,0X00,0XE1,0XF8,0X0E,0X07,0X00,0X00,0XE0,0X70,0X00,0X07,0X00,0X00,
0XE0,0X00,0X00,0X07,0X00,0X01,0XE0,0X00,0X00,0X03,0X80,0X01,0XC0,0X00,0X00,0X03,
0XC0,0X03,0XC0,0X00,0X00,0X01,0XE0,0X07,0X80,0X00,0X00,0X00,0XF8,0X1F,0X00,0X00,
0X00,0X00,0X7F,0XFE,0X00,0X00,0X00,0X00,0X3F,0XFC,0X00,0X00,0X00,0X18,0X0F,0XF0,
0X18,0X00,0X00,0X3C,0X00,0X00,0X3C,0X00,0X00,0X78,0X00,0X00,0X3E,0X00,0X00,0XF0,
0X00,0X00,0X1E,0X00,0X00,0XE0,0X00,0X00,0X0F,0X00,0X00,0X40,0X01,0X80,0X06,0X00,
0X00,0X00,0X03,0X80,0X00,0X00,0X00,0X00,0X03,0X80,0X00,0X00,0X00,0X00,0X03,0X80,
0X00,0X00,0X00,0X00,0X03,0X80,0X00,0X00,0X00,0X00,0X01,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00
};


