/*****************************************************************************
* | File      	:   EPD_2in7_V2.h
* | Author      :   Waveshare team
* | Function    :   2.7inch V2 e-paper
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2022-08-18
* | Info        :
* -----------------------------------------------------------------------------
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#ifndef __EPD_2IN7_V2_H_
#define __EPD_2IN7_V2_H_

#include "main.h"
#include "usart1.h" // 该头文件包含调试系统
#include "GUI_Paint.h"

// Display resolution
#define EPD_2IN7_V2_WIDTH 176
#define EPD_2IN7_V2_HEIGHT 264
// 缓存大小
#define EPD_FRAME_BUF_SIZE ((EPD_2IN7_V2_WIDTH % 8 == 0) ? (EPD_2IN7_V2_WIDTH / 8) : (EPD_2IN7_V2_WIDTH / 8 + 1)) * EPD_2IN7_V2_HEIGHT

typedef enum
{
    EPD_RST_PIN_EM,
    EPD_DC_PIN_EM,
    EPD_CS_PIN_EM,
    EPD_READ_BUSY_PIN_EM,
    EPD_TOTAL_PIN_EM
} epd_pin_ctrl_t;

typedef enum
{
    EPD_PAGE_NONE,
    EPD_PAGE_MAIN,
    EPD_TOTAL_PAGE
}epd_page_t;

typedef struct epd_dev_v2_t
{
    uint8_t frame_buf[EPD_FRAME_BUF_SIZE]; // 一帧图片缓存
    
    epd_page_t current_page;
    PAINT_TIME current_time;

    /* 函数指针 */
    void (*delay_ms_callback)(uint32_t ms);
    uint8_t (*spi_write_byte_callback)(uint8_t data);
    uint8_t (*module_start_callback)(void);
    uint8_t (*module_end_callback)(void);
    uint8_t (*pin_ctrl_callback)(epd_pin_ctrl_t pin, uint8_t state);
} epd_dev_v2_t;

/* 全局变量 */
extern epd_dev_v2_t g_epd_dev;

/* 全局函数 */
uint8_t epd_init(epd_dev_v2_t *dev,
                 void (*delay_callback)(uint32_t ms),
                 uint8_t (*write_callback)(uint8_t data),
                 uint8_t (*start_callback)(void),
                 uint8_t (*end_callback)(void),
                 uint8_t (*pin_ctrl_callback)(epd_pin_ctrl_t pin, uint8_t state));
void EPD_2IN7_V2_Init(epd_dev_v2_t *dev);
void EPD_2IN7_V2_Init_Fast(epd_dev_v2_t *dev);
void EPD_2IN7_V2_Init_4GRAY(epd_dev_v2_t *dev);
void EPD_2IN7_V2_Clear(epd_dev_v2_t *dev);
void EPD_2IN7_V2_Display(epd_dev_v2_t *dev, uint8_t *Image);
void EPD_2IN7_V2_Display_Fast(epd_dev_v2_t *dev, uint8_t *Image);
void EPD_2IN7_V2_Display_Base(epd_dev_v2_t *dev, uint8_t *Image);
void EPD_2IN7_V2_Display_Base_color(epd_dev_v2_t *dev, uint8_t color);
void EPD_2IN7_V2_Display_Partial(epd_dev_v2_t *dev, const uint8_t *Image, uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yende);
void EPD_2IN7_V2_4GrayDisplay(epd_dev_v2_t *dev, uint8_t *Image);
void EPD_2IN7_V2_Sleep(epd_dev_v2_t *dev);

#endif
