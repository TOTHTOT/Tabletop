/*
 *                        _oo0oo_
 *                       o8888888o
 *                       88" . "88
 *                       (| -_- |)
 *                       0\  =  /0
 *                     ___/`---'\___
 *                   .' \\|     |// '.
 *                  / \\|||  :  |||// \
 *                 / _||||| -:- |||||- \
 *                |   | \\\  - /// |   |
 *                | \_|  ''\---/''  |_/ |
 *                \  .-\__  '-'  ___/-. /
 *              ___'. .'  /--.--\  `. .'___
 *           ."" '<  `.___\_<|>_/___.' >' "".
 *          | | :  `- \`.;`\ _ /`;.`/ - ` : | |
 *          \  \ `_.   \_ __\ /__ _/   .-` /  /
 *      =====`-.____`.___ \_____/___.-`___.-'=====
 *                        `=---='
 *
 *
 *      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *            佛祖保佑     永不宕机     永无BUG
 */

/*
 * @Description: 墨水屏驱动EPD_2in7_V2版本
 * @Author: TOTHTOT
 * @Date: 2023-07-21 23:19:15
 * @LastEditTime: 2023-07-30 08:52:21
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\Learn\stm32\My_Project\Tabletop\Code\STM32F103C8T6(HAL+FreeRTOS)\HARDWARE\EPaper\EPD_2in7_V2.c
 */

#include "EPD_2in7_V2.h"
#include "string.h"
#include "spi.h"
#include "GUI_Paint.h"
#include <stdio.h>
#include <string.h>

#if (EPD_USE_RTOS == 1)
#include "FreeRTOS.h"
#include "cmsis_os.h"
#endif /* EPD_USE_RTOS */

/* 全局变量 */
epd_dev_v2_t g_epd_dev = {0};

/* 用户代码开始, 实现结构体内的函数指针 */
/**
 * @name: epd_spi_write_byte
 * @msg: spi 写入一字节
 * @param {uint8_t} data 写入数据
 * @return {写入状态}
 * @author: TOTHTOT
 * @date: 2023年7月22日
 */
uint8_t epd_spi_write_byte(uint8_t data)
{
    return HAL_SPI_Transmit(&hspi1, &data, 1, 1000);
}

/**
 * @name: edp_pin_ctrl
 * @msg: 墨水屏的IO口控制函数
 * @param {epd_pin_ctrl_t} pin  控制的IO口
 * @param {uint8_t} state   IO口输出的状态
 * @return {== 0xff成功, == 0/1 IO口读取状态}
 * @author: TOTHTOT
 * @date: 2023年7月22日
 */
uint8_t edp_pin_ctrl(epd_pin_ctrl_t pin, uint8_t state)
{
    switch (pin)
    {
    case EPD_RST_PIN_EM:
        HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, state == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
        break;
    case EPD_DC_PIN_EM:
        HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, state == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
        break;
    case EPD_CS_PIN_EM:
        HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, state == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
        break;
    case EPD_READ_BUSY_PIN_EM:
        return HAL_GPIO_ReadPin(BUSY_GPIO_Port, BUSY_Pin);
    default:
        break;
    }
    return 0xff;
}

/**
 * @name: epd_start
 * @msg: 墨水屏启动控制
 * @return {无}
 * @author: TOTHTOT
 * @date: 2023年7月22日
 */
uint8_t epd_start(void)
{
    edp_pin_ctrl(EPD_CS_PIN_EM, 0);
    edp_pin_ctrl(EPD_RST_PIN_EM, 0);
    edp_pin_ctrl(EPD_DC_PIN_EM, 1);

    return 0;
}
/**
 * @name: epd_end
 * @msg: 墨水屏结束控制
 * @return {无}
 * @author: TOTHTOT
 * @date: 2023年7月22日
 */
uint8_t epd_end(void)
{
    edp_pin_ctrl(EPD_CS_PIN_EM, 0);
    edp_pin_ctrl(EPD_RST_PIN_EM, 0);
    edp_pin_ctrl(EPD_DC_PIN_EM, 0);

    return 0;
}
#if (EPD_USE_RTOS == 1)
/**
 * @name: epd_en_refresh
 * @msg: 调用一次就刷新一次屏幕
 * @param {epd_dev_v2_t} *dev 屏幕设备
 * @param {epd_screen_element_t} element 要刷新的组件
 * @return {*}
 * @author: TOTHTOT
 * @date:
 */
extern osSemaphoreId en_epd_refreshHandle;
uint8_t epd_en_refresh(epd_dev_v2_t *dev, epd_screen_element_t element)
{
    uint8_t ret = 0;

    if (dev->enter_system_flag == 1)
    {
        dev->refresh_element = element;
        // os_ret = osSemaphoreRelease(en_epd_refreshHandle);
        if (osSemaphoreRelease(en_epd_refreshHandle) != osOK)
        {
            ERROR_PRINT("release semaphore faile\r\n");
            ret = 1;
        }
        else
            INFO_PRINT("release refresh screen [%d]\r\n", element);
    }
    else
        ret = 2;

    return ret;
}
#endif /* EPD_USE_RTOS */
/* 用户代码结束 */

uint8_t LUT_DATA_4Gray[159] =
    {
        0x40, 0x48, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x8, 0x48, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x2, 0x48, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x20, 0x48, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0xA, 0x19, 0x0, 0x3, 0x8, 0x0, 0x0,
        0x14, 0x1, 0x0, 0x14, 0x1, 0x0, 0x3,
        0xA, 0x3, 0x0, 0x8, 0x19, 0x0, 0x0,
        0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x0, 0x0, 0x0,
        0x22, 0x17, 0x41, 0x0, 0x32, 0x1C};

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_2IN7_V2_Reset(epd_dev_v2_t *dev)
{
    dev->pin_ctrl_callback(EPD_RST_PIN_EM, 1);
    dev->delay_ms_callback(20);
    dev->pin_ctrl_callback(EPD_RST_PIN_EM, 0);
    dev->delay_ms_callback(20);
    dev->pin_ctrl_callback(EPD_RST_PIN_EM, 1);
    dev->delay_ms_callback(20);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_2IN7_V2_SendCommand(epd_dev_v2_t *dev, uint8_t Reg)
{
    dev->pin_ctrl_callback(EPD_DC_PIN_EM, 0);
    dev->pin_ctrl_callback(EPD_CS_PIN_EM, 0);
    dev->spi_write_byte_callback(Reg);
    dev->pin_ctrl_callback(EPD_CS_PIN_EM, 1);
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
static void EPD_2IN7_V2_SendData(epd_dev_v2_t *dev, uint8_t Data)
{
    dev->pin_ctrl_callback(EPD_DC_PIN_EM, 1);
    dev->pin_ctrl_callback(EPD_CS_PIN_EM, 0);
    dev->spi_write_byte_callback(Data);
    dev->pin_ctrl_callback(EPD_CS_PIN_EM, 1);
}

/******************************************************************************
function :	Wait until the busy_pin goes LOW
parameter:
******************************************************************************/
static void EPD_2IN7_V2_ReadBusy(epd_dev_v2_t *dev)
{
    // INFO_PRINT("e-Paper busy\r\n");
    while (1)
    {
        if (dev->pin_ctrl_callback(EPD_READ_BUSY_PIN_EM, 0) == 0)
            break;
    }
    // INFO_PRINT("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Turn on display
parameter:
******************************************************************************/
static void EPD_2IN7_V2_TurnOnDisplay(epd_dev_v2_t *dev)
{
    EPD_2IN7_V2_SendCommand(dev, 0x22); // Display Update Control
    EPD_2IN7_V2_SendData(dev, 0xF7);
    EPD_2IN7_V2_SendCommand(dev, 0x20); // Activate Display Update Sequence
    EPD_2IN7_V2_ReadBusy(dev);
}

static void EPD_2IN7_V2_TurnOnDisplay_Fast(epd_dev_v2_t *dev)
{
    EPD_2IN7_V2_SendCommand(dev, 0x22); // Display Update Control
    EPD_2IN7_V2_SendData(dev, 0xC7);
    EPD_2IN7_V2_SendCommand(dev, 0x20); // Activate Display Update Sequence
    EPD_2IN7_V2_ReadBusy(dev);
}

static void EPD_2IN7_V2_TurnOnDisplay_Partial(epd_dev_v2_t *dev)
{
    EPD_2IN7_V2_SendCommand(dev, 0x22);
    EPD_2IN7_V2_SendData(dev, 0xFF);
    EPD_2IN7_V2_SendCommand(dev, 0x20);
    EPD_2IN7_V2_ReadBusy(dev);
}

static void EPD_2IN7_V2_TurnOnDisplay_4GRAY(epd_dev_v2_t *dev)
{
    EPD_2IN7_V2_SendCommand(dev, 0x22);
    EPD_2IN7_V2_SendData(dev, 0xC7);
    EPD_2IN7_V2_SendCommand(dev, 0x20);
    EPD_2IN7_V2_ReadBusy(dev);
}
/******************************************************************************
function :	set the look-up tables
parameter:
******************************************************************************/
static void EPD_2IN7_V2_Lut(epd_dev_v2_t *dev)
{
    unsigned int count;
    EPD_2IN7_V2_SendCommand(dev, 0x32); // vcom
    for (count = 0; count < 153; count++)
    {
        EPD_2IN7_V2_SendData(dev, LUT_DATA_4Gray[count]);
    }
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_2IN7_V2_Init(epd_dev_v2_t *dev)
{
    EPD_2IN7_V2_Reset(dev);
    EPD_2IN7_V2_ReadBusy(dev);

    EPD_2IN7_V2_SendCommand(dev, 0x12); // SWRESET
    EPD_2IN7_V2_ReadBusy(dev);

    EPD_2IN7_V2_SendCommand(dev, 0x45); // set Ram-Y address start/end position
    EPD_2IN7_V2_SendData(dev, 0x00);
    EPD_2IN7_V2_SendData(dev, 0x00);
    EPD_2IN7_V2_SendData(dev, 0x07); // 0x0107-->(263+1)=264
    EPD_2IN7_V2_SendData(dev, 0x01);

    EPD_2IN7_V2_SendCommand(dev, 0x4F); // set RAM y address count to 0;
    EPD_2IN7_V2_SendData(dev, 0x00);
    EPD_2IN7_V2_SendData(dev, 0x00);

    EPD_2IN7_V2_SendCommand(dev, 0x11); // data entry mode
    EPD_2IN7_V2_SendData(dev, 0x03);
}
void EPD_2IN7_V2_Init_Fast(epd_dev_v2_t *dev)
{
    EPD_2IN7_V2_Reset(dev);
    EPD_2IN7_V2_ReadBusy(dev);

    EPD_2IN7_V2_SendCommand(dev, 0x12); // SWRESET
    EPD_2IN7_V2_ReadBusy(dev);

    EPD_2IN7_V2_SendCommand(dev, 0x18); // Read built-in temperature sensor
    EPD_2IN7_V2_SendData(dev, 0x80);

    EPD_2IN7_V2_SendCommand(dev, 0x22); // Load temperature value
    EPD_2IN7_V2_SendData(dev, 0xB1);
    EPD_2IN7_V2_SendCommand(dev, 0x20);
    EPD_2IN7_V2_ReadBusy(dev);

    EPD_2IN7_V2_SendCommand(dev, 0x1A); // Write to temperature register
    EPD_2IN7_V2_SendData(dev, 0x64);
    EPD_2IN7_V2_SendData(dev, 0x00);

    EPD_2IN7_V2_SendCommand(dev, 0x45); // set Ram-Y address start/end position
    EPD_2IN7_V2_SendData(dev, 0x00);
    EPD_2IN7_V2_SendData(dev, 0x00);
    EPD_2IN7_V2_SendData(dev, 0x07); // 0x0107-->(263+1)=264
    EPD_2IN7_V2_SendData(dev, 0x01);

    EPD_2IN7_V2_SendCommand(dev, 0x4F); // set RAM y address count to 0;
    EPD_2IN7_V2_SendData(dev, 0x00);
    EPD_2IN7_V2_SendData(dev, 0x00);

    EPD_2IN7_V2_SendCommand(dev, 0x11); // data entry mode
    EPD_2IN7_V2_SendData(dev, 0x03);

    EPD_2IN7_V2_SendCommand(dev, 0x22); // Load temperature value
    EPD_2IN7_V2_SendData(dev, 0x91);
    EPD_2IN7_V2_SendCommand(dev, 0x20);
    EPD_2IN7_V2_ReadBusy(dev);
}
void EPD_2IN7_V2_Init_4GRAY(epd_dev_v2_t *dev)
{
    EPD_2IN7_V2_Reset(dev);

    EPD_2IN7_V2_ReadBusy(dev);
    EPD_2IN7_V2_SendCommand(dev, 0x12); // soft reset
    EPD_2IN7_V2_ReadBusy(dev);

    EPD_2IN7_V2_SendCommand(dev, 0x74); // set analog block control
    EPD_2IN7_V2_SendData(dev, 0x54);
    EPD_2IN7_V2_SendCommand(dev, 0x7E); // set digital block control
    EPD_2IN7_V2_SendData(dev, 0x3B);

    EPD_2IN7_V2_SendCommand(dev, 0x01); // Driver output control
    EPD_2IN7_V2_SendData(dev, 0x07);
    EPD_2IN7_V2_SendData(dev, 0x01);
    EPD_2IN7_V2_SendData(dev, 0x00);

    EPD_2IN7_V2_SendCommand(dev, 0x11); // data entry mode
    EPD_2IN7_V2_SendData(dev, 0x03);

    EPD_2IN7_V2_SendCommand(dev, 0x44); // set Ram-X address start/end position
    EPD_2IN7_V2_SendData(dev, 0x00);
    EPD_2IN7_V2_SendData(dev, 0x15); // 0x15-->(21+1)*8=176

    EPD_2IN7_V2_SendCommand(dev, 0x45); // set Ram-Y address start/end position
    EPD_2IN7_V2_SendData(dev, 0x00);
    EPD_2IN7_V2_SendData(dev, 0x00);
    EPD_2IN7_V2_SendData(dev, 0x07); // 0x0107-->(263+1)=264
    EPD_2IN7_V2_SendData(dev, 0x01);

    EPD_2IN7_V2_SendCommand(dev, 0x3C); // BorderWavefrom
    EPD_2IN7_V2_SendData(dev, 0x00);

    EPD_2IN7_V2_SendCommand(dev, 0x2C);             // VCOM Voltage
    EPD_2IN7_V2_SendData(dev, LUT_DATA_4Gray[158]); // 0x1C

    EPD_2IN7_V2_SendCommand(dev, 0x3F); // EOPQ
    EPD_2IN7_V2_SendData(dev, LUT_DATA_4Gray[153]);

    EPD_2IN7_V2_SendCommand(dev, 0x03); // VGH
    EPD_2IN7_V2_SendData(dev, LUT_DATA_4Gray[154]);

    EPD_2IN7_V2_SendCommand(dev, 0x04);             //
    EPD_2IN7_V2_SendData(dev, LUT_DATA_4Gray[155]); // VSH1
    EPD_2IN7_V2_SendData(dev, LUT_DATA_4Gray[156]); // VSH2
    EPD_2IN7_V2_SendData(dev, LUT_DATA_4Gray[157]); // VSL

    EPD_2IN7_V2_Lut(dev); // LUT

    EPD_2IN7_V2_SendCommand(dev, 0x4E); // set RAM x address count to 0;
    EPD_2IN7_V2_SendData(dev, 0x00);
    EPD_2IN7_V2_SendCommand(dev, 0x4F); // set RAM y address count to 0X199;
    EPD_2IN7_V2_SendData(dev, 0x00);
    EPD_2IN7_V2_SendData(dev, 0x00);
    EPD_2IN7_V2_ReadBusy(dev);
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_2IN7_V2_Clear(epd_dev_v2_t *dev)
{
    uint16_t Width, Height;
    Width = (EPD_2IN7_V2_WIDTH % 8 == 0) ? (EPD_2IN7_V2_WIDTH / 8) : (EPD_2IN7_V2_WIDTH / 8 + 1);
    Height = EPD_2IN7_V2_HEIGHT;

    EPD_2IN7_V2_SendCommand(dev, 0x24);
    for (uint16_t j = 0; j < Height; j++)
    {
        for (uint16_t i = 0; i < Width; i++)
        {
            EPD_2IN7_V2_SendData(dev, 0XFF);
        }
    }

    EPD_2IN7_V2_TurnOnDisplay(dev);
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_2IN7_V2_Display(epd_dev_v2_t *dev, uint8_t *Image)
{
    uint16_t Width, Height;
    Width = (EPD_2IN7_V2_WIDTH % 8 == 0) ? (EPD_2IN7_V2_WIDTH / 8) : (EPD_2IN7_V2_WIDTH / 8 + 1);
    Height = EPD_2IN7_V2_HEIGHT;

    EPD_2IN7_V2_SendCommand(dev, 0x24);
    for (uint16_t j = 0; j < Height; j++)
    {
        for (uint16_t i = 0; i < Width; i++)
        {
            EPD_2IN7_V2_SendData(dev, Image[i + j * Width]);
        }
    }

    EPD_2IN7_V2_TurnOnDisplay(dev);
}

void EPD_2IN7_V2_Display_Fast(epd_dev_v2_t *dev, uint8_t *Image)
{
    uint16_t Width, Height;
    Width = (EPD_2IN7_V2_WIDTH % 8 == 0) ? (EPD_2IN7_V2_WIDTH / 8) : (EPD_2IN7_V2_WIDTH / 8 + 1);
    Height = EPD_2IN7_V2_HEIGHT;

    EPD_2IN7_V2_SendCommand(dev, 0x24);
    for (uint16_t j = 0; j < Height; j++)
    {
        for (uint16_t i = 0; i < Width; i++)
        {
            EPD_2IN7_V2_SendData(dev, Image[i + j * Width]);
        }
    }
    EPD_2IN7_V2_TurnOnDisplay_Fast(dev);
}

void EPD_2IN7_V2_Display_Base(epd_dev_v2_t *dev, uint8_t *Image)
{
    uint16_t Width, Height;
    Width = (EPD_2IN7_V2_WIDTH % 8 == 0) ? (EPD_2IN7_V2_WIDTH / 8) : (EPD_2IN7_V2_WIDTH / 8 + 1);
    Height = EPD_2IN7_V2_HEIGHT;

    EPD_2IN7_V2_SendCommand(dev, 0x24); // Write Black and White image to RAM
    for (uint16_t j = 0; j < Height; j++)
    {
        for (uint16_t i = 0; i < Width; i++)
        {
            EPD_2IN7_V2_SendData(dev, Image[i + j * Width]);
        }
    }
    EPD_2IN7_V2_SendCommand(dev, 0x26); // Write Black and White image to RAM
    for (uint16_t j = 0; j < Height; j++)
    {
        for (uint16_t i = 0; i < Width; i++)
        {
            EPD_2IN7_V2_SendData(dev, Image[i + j * Width]);
        }
    }
    EPD_2IN7_V2_TurnOnDisplay(dev);
}

void EPD_2IN7_V2_Display_Base_color(epd_dev_v2_t *dev, uint8_t color)
{
    uint16_t Width, Height;
    Width = (EPD_2IN7_V2_WIDTH % 8 == 0) ? (EPD_2IN7_V2_WIDTH / 8) : (EPD_2IN7_V2_WIDTH / 8 + 1);
    Height = EPD_2IN7_V2_HEIGHT;

    EPD_2IN7_V2_SendCommand(dev, 0x24); // Write Black and White image to RAM
    for (uint16_t j = 0; j < Height; j++)
    {
        for (uint16_t i = 0; i < Width; i++)
        {
            EPD_2IN7_V2_SendData(dev, color);
        }
    }
    EPD_2IN7_V2_SendCommand(dev, 0x26); // Write Black and White image to RAM
    for (uint16_t j = 0; j < Height; j++)
    {
        for (uint16_t i = 0; i < Width; i++)
        {
            EPD_2IN7_V2_SendData(dev, color);
        }
    }
    EPD_2IN7_V2_TurnOnDisplay(dev);
}

void EPD_2IN7_V2_Display_Partial(epd_dev_v2_t *dev, const uint8_t *Image, uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend)
{
    if ((Xstart % 8 + Xend % 8 == 8 && Xstart % 8 > Xend % 8) || Xstart % 8 + Xend % 8 == 0 || (Xend - Xstart) % 8 == 0)
    {
        Xstart = Xstart / 8;
        Xend = Xend / 8;
    }
    else
    {
        Xstart = Xstart / 8;
        Xend = Xend % 8 == 0 ? Xend / 8 : Xend / 8 + 1;
    }

    uint16_t i, Width;
    Width = Xend - Xstart;
    uint16_t IMAGE_COUNTER = Width * (Yend - Ystart);

    Xend -= 1;
    Yend -= 1;
    // Reset
    EPD_2IN7_V2_Reset(dev);

    EPD_2IN7_V2_SendCommand(dev, 0x3C); // BorderWavefrom
    EPD_2IN7_V2_SendData(dev, 0x80);
    //
    EPD_2IN7_V2_SendCommand(dev, 0x44);              // set RAM x address start/end, in page 35
    EPD_2IN7_V2_SendData(dev, Xstart & 0xff);        // RAM x address start at 00h;
    EPD_2IN7_V2_SendData(dev, Xend & 0xff);          // RAM x address end at 0fh(15+1)*8->128
    EPD_2IN7_V2_SendCommand(dev, 0x45);              // set RAM y address start/end, in page 35
    EPD_2IN7_V2_SendData(dev, Ystart & 0xff);        // RAM y address start at 0127h;
    EPD_2IN7_V2_SendData(dev, (Ystart >> 8) & 0x01); // RAM y address start at 0127h;
    EPD_2IN7_V2_SendData(dev, Yend & 0xff);          // RAM y address end at 00h;
    EPD_2IN7_V2_SendData(dev, (Yend >> 8) & 0x01);

    EPD_2IN7_V2_SendCommand(dev, 0x4E); // set RAM x address count to 0;
    EPD_2IN7_V2_SendData(dev, Xstart & 0xff);
    EPD_2IN7_V2_SendCommand(dev, 0x4F); // set RAM y address count to 0X127;
    EPD_2IN7_V2_SendData(dev, Ystart & 0xff);
    EPD_2IN7_V2_SendData(dev, (Ystart >> 8) & 0x01);

    EPD_2IN7_V2_SendCommand(dev, 0x24); // Write Black and White image to RAM
    for (i = 0; i < IMAGE_COUNTER; i++)
    {
        EPD_2IN7_V2_SendData(dev, Image[i]);
    }
    EPD_2IN7_V2_TurnOnDisplay_Partial(dev);
}

void EPD_2IN7_V2_4GrayDisplay(epd_dev_v2_t *dev, uint8_t *Image)
{
    uint32_t i, j, k;
    uint8_t temp1, temp2, temp3;

    // old  data
    EPD_2IN7_V2_SendCommand(dev, 0x24);
    for (i = 0; i < 5808; i++)
    { // 5808*4  46464
        temp3 = 0;
        for (j = 0; j < 2; j++)
        {
            temp1 = Image[i * 2 + j];
            for (k = 0; k < 2; k++)
            {
                temp2 = temp1 & 0xC0;
                if (temp2 == 0xC0)
                    temp3 |= 0x00;
                else if (temp2 == 0x00)
                    temp3 |= 0x01;
                else if (temp2 == 0x80)
                    temp3 |= 0x01;
                else // 0x40
                    temp3 |= 0x00;
                temp3 <<= 1;

                temp1 <<= 2;
                temp2 = temp1 & 0xC0;
                if (temp2 == 0xC0)
                    temp3 |= 0x00;
                else if (temp2 == 0x00)
                    temp3 |= 0x01;
                else if (temp2 == 0x80)
                    temp3 |= 0x01;
                else // 0x40
                    temp3 |= 0x00;
                if (j != 1 || k != 1)
                    temp3 <<= 1;

                temp1 <<= 2;
            }
        }
        EPD_2IN7_V2_SendData(dev, temp3);
        // printf("%x",temp3);
    }

    EPD_2IN7_V2_SendCommand(dev, 0x26); // write RAM for black(0)/white (1)
    for (i = 0; i < 5808; i++)
    { // 5808*4  46464
        temp3 = 0;
        for (j = 0; j < 2; j++)
        {
            temp1 = Image[i * 2 + j];
            for (k = 0; k < 2; k++)
            {
                temp2 = temp1 & 0xC0;
                if (temp2 == 0xC0)
                    temp3 |= 0x00; // white
                else if (temp2 == 0x00)
                    temp3 |= 0x01; // black
                else if (temp2 == 0x80)
                    temp3 |= 0x00; // gray1
                else               // 0x40
                    temp3 |= 0x01; // gray2
                temp3 <<= 1;

                temp1 <<= 2;
                temp2 = temp1 & 0xC0;
                if (temp2 == 0xC0) // white
                    temp3 |= 0x00;
                else if (temp2 == 0x00) // black
                    temp3 |= 0x01;
                else if (temp2 == 0x80)
                    temp3 |= 0x00; // gray1
                else               // 0x40
                    temp3 |= 0x01; // gray2
                if (j != 1 || k != 1)
                    temp3 <<= 1;

                temp1 <<= 2;
            }
        }
        EPD_2IN7_V2_SendData(dev, temp3);
        // printf("%x",temp3);
    }

    EPD_2IN7_V2_TurnOnDisplay_4GRAY(dev);
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_2IN7_V2_Sleep(epd_dev_v2_t *dev)
{
    EPD_2IN7_V2_SendCommand(dev, 0X10);
    EPD_2IN7_V2_SendData(dev, 0x01);
}

uint8_t epd_page_main(epd_dev_v2_t *dev)
{

    return 0;
}
#if (USE_ELEMENT_BUF == 1)
/**
 * @name:
 * @msg:
 * @param {epd_dev_v2_t} *dev
 * @return {*}
 * @author: TOTHTOT
 * @date:
 */
static uint8_t epd_page_main_time_init(epd_dev_v2_t *dev)
{
    uint8_t x_start = 8;
    INFO_PRINT("time_buf_size = %d\r\n", sizeof(dev->time_frame_buf));

    Paint_NewImage(dev->time_frame_buf, 50, 120, 90, WHITE);

    Paint_SelectImage(dev->time_frame_buf);
    Paint_SetScale(2);
    Paint_Clear(WHITE);
    EPD_2IN7_V2_Display_Base_color(dev, WHITE);

    // EPD_2IN7_V2_Display_Partial(dev, dev->frame_buf, x, 0, x + 50, 120); // Xstart must be a multiple of 8
    return 0;
}

static uint8_t epd_page_main_t_h_init(epd_dev_v2_t *dev)
{
    uint8_t x_start = 80;
    INFO_PRINT("t_h_buf_size = %d\r\n", sizeof(dev->t_h_frame_buf));

    Paint_NewImage(dev->t_h_frame_buf, 50, 120, 90, WHITE);

    Paint_SelectImage(dev->t_h_frame_buf);
    Paint_SetScale(2);
    Paint_Clear(WHITE);
    EPD_2IN7_V2_Display_Base_color(dev, WHITE);
    // EPD_2IN7_V2_Display_Partial(dev, dev->frame_buf, x, 0, x + 50, 120); // Xstart must be a multiple of 8

    return 0;
}
uint8_t epd_page_main_element_init(epd_dev_v2_t *dev)
{
    dev->module_start_callback(); // 激活屏幕

    EPD_2IN7_V2_Init(dev);  //
    EPD_2IN7_V2_Clear(dev); // 清屏

    // 设置一帧图片缓存, 接下来就是绘制缓存内容, 最后将一帧数据输出到屏幕
    Paint_NewImage(dev->frame_buf, EPD_2IN7_V2_WIDTH, EPD_2IN7_V2_HEIGHT, 90, WHITE);
    // 屏幕显存设置颜色
    Paint_Clear(WHITE);

    dev->main_element_attr[EPD_MAIN_SCREEN_ELEMENT_TIME].x_start = 10;
    dev->main_element_attr[EPD_MAIN_SCREEN_ELEMENT_TIME].y_start = 15;
    return 0;
}
#endif /* USE_ELEMENT_BUF */

const unsigned char gImage_wifi_linked_icon[288] = {
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
    0X1F,
    0XF8,
    0X00,
    0X00,
    0X00,
    0X03,
    0XFF,
    0XFF,
    0XC0,
    0X00,
    0X00,
    0X0F,
    0XFF,
    0XFF,
    0XF0,
    0X00,
    0X00,
    0X7F,
    0XFF,
    0XFF,
    0XFE,
    0X00,
    0X01,
    0XFF,
    0XFF,
    0XFF,
    0XFF,
    0X00,
    0X03,
    0XFF,
    0XFF,
    0XFF,
    0XFF,
    0XC0,
    0X07,
    0XFF,
    0XFF,
    0XFF,
    0XFF,
    0XE0,
    0X1F,
    0XFF,
    0X80,
    0X01,
    0XFF,
    0XF8,
    0X3F,
    0XFC,
    0X00,
    0X00,
    0X3F,
    0XFC,
    0X7F,
    0XF0,
    0X00,
    0X00,
    0X0F,
    0XFE,
    0XFF,
    0XC0,
    0X00,
    0X00,
    0X03,
    0XFF,
    0X7F,
    0X00,
    0X00,
    0X00,
    0X00,
    0XFE,
    0X3E,
    0X00,
    0X00,
    0X00,
    0X00,
    0X7C,
    0X1C,
    0X00,
    0X0F,
    0XF0,
    0X00,
    0X38,
    0X08,
    0X00,
    0X7F,
    0XFE,
    0X00,
    0X10,
    0X00,
    0X03,
    0XFF,
    0XFF,
    0XC0,
    0X00,
    0X00,
    0X0F,
    0XFF,
    0XFF,
    0XE0,
    0X00,
    0X00,
    0X1F,
    0XFF,
    0XFF,
    0XF8,
    0X00,
    0X00,
    0X3F,
    0XFF,
    0XFF,
    0XFC,
    0X00,
    0X00,
    0X7F,
    0XFF,
    0XFF,
    0XFE,
    0X00,
    0X00,
    0X7F,
    0XF0,
    0X0F,
    0XFE,
    0X00,
    0X00,
    0X3F,
    0X80,
    0X01,
    0XFC,
    0X00,
    0X00,
    0X1F,
    0X00,
    0X00,
    0XF8,
    0X00,
    0X00,
    0X0C,
    0X00,
    0X00,
    0X30,
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
    0X03,
    0XC0,
    0X00,
    0X00,
    0X00,
    0X00,
    0X1F,
    0XF8,
    0X00,
    0X00,
    0X00,
    0X00,
    0X3F,
    0XFC,
    0X00,
    0X00,
    0X00,
    0X00,
    0X3F,
    0XFC,
    0X00,
    0X00,
    0X00,
    0X00,
    0X1F,
    0XF8,
    0X00,
    0X00,
    0X00,
    0X00,
    0X0F,
    0XF0,
    0X00,
    0X00,
    0X00,
    0X00,
    0X07,
    0XE0,
    0X00,
    0X00,
    0X00,
    0X00,
    0X03,
    0XC0,
    0X00,
    0X00,
    0X00,
    0X00,
    0X01,
    0X80,
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
const unsigned char gImage_cloudy[288] = {
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

uint8_t epd_main_updata(epd_dev_v2_t *dev, epd_screen_element_t elememt)
{
    Paint_ClearWindows(dev->main_element_attr[elememt].x_start,
                       dev->main_element_attr[elememt].y_start,
                       dev->main_element_attr[elememt].x_end,
                       dev->main_element_attr[elememt].y_end,
                       WHITE);

    switch (elememt)
    {
    case EPD_MAIN_SCREEN_ELEMENT_TIME:
        Paint_DrawTime(dev->main_element_attr[EPD_MAIN_SCREEN_ELEMENT_TIME].x_start,
                       dev->main_element_attr[EPD_MAIN_SCREEN_ELEMENT_TIME].y_start,
                       &dev->current_time, &Font24, WHITE, BLACK); // 时间显示
        break;
    case EPD_MAIN_SCREEN_ELEMENT_DATE:
        sprintf((char *)dev->date, "%4d/%2d/%2d", dev->current_time.Year, dev->current_time.Month, dev->current_time.Day);
        Paint_DrawString_EN(dev->main_element_attr[EPD_MAIN_SCREEN_ELEMENT_DATE].x_start,
                            dev->main_element_attr[EPD_MAIN_SCREEN_ELEMENT_DATE].y_start,
                            (char *)dev->date, &Font20, WHITE, BLACK); // 日期显示
        break;

    case EPD_MAIN_SCREEN_ELEMENT_LINE:
        // 两条横线
        Paint_DrawLine(0, EPD_2IN7_V2_WIDTH / 3, EPD_2IN7_V2_HEIGHT, EPD_2IN7_V2_WIDTH / 3,
                       BLACK, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
        Paint_DrawLine(0, EPD_2IN7_V2_WIDTH / 3 * 2, EPD_2IN7_V2_HEIGHT - 1, EPD_2IN7_V2_WIDTH / 3 * 2,
                       BLACK, DOT_PIXEL_2X2, LINE_STYLE_SOLID);

        // 两条竖线在第一行
        Paint_DrawLine(EPD_2IN7_V2_HEIGHT / 3, 0, EPD_2IN7_V2_HEIGHT / 3, EPD_2IN7_V2_WIDTH / 3,
                       BLACK, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
        Paint_DrawLine((EPD_2IN7_V2_HEIGHT / 3 * 2 - 1), 0, (EPD_2IN7_V2_HEIGHT / 3 * 2) - 1, (EPD_2IN7_V2_WIDTH / 3),
                       BLACK, DOT_PIXEL_2X2, LINE_STYLE_SOLID); // 这里的xend必须减1 不然指针越界造成时间值错误!!!!!
        break;
    case EPD_MAIN_SCREEN_ELEMENT_T:
    case EPD_MAIN_SCREEN_ELEMENT_H:
        sprintf((char *)dev->temperature, "%3dC", 26);
        Paint_DrawString_EN(dev->main_element_attr[EPD_MAIN_SCREEN_ELEMENT_T].x_start,
                            dev->main_element_attr[EPD_MAIN_SCREEN_ELEMENT_T].y_start,
                            (char *)dev->temperature, &Font20, WHITE, BLACK); // 温度显示
        sprintf((char *)dev->humidity, "%3d%%", 50);
        Paint_DrawString_EN(dev->main_element_attr[EPD_MAIN_SCREEN_ELEMENT_H].x_start,
                            dev->main_element_attr[EPD_MAIN_SCREEN_ELEMENT_H].y_start,
                            (char *)dev->humidity, &Font20, WHITE, BLACK); // 湿度显示
        break;
    case EPD_MAIN_SCREEN_ELEMENT_WiFi_ICON:
        Paint_DrawBitMap_Paste(gImage_wifi_linked_icon,
                               dev->main_element_attr[EPD_MAIN_SCREEN_ELEMENT_WiFi_ICON].x_start,
                               dev->main_element_attr[EPD_MAIN_SCREEN_ELEMENT_WiFi_ICON].y_start,
                               48, 48, 1);
        break;
    case EPD_MAIN_SCREEN_ELEMENT_WEATHER_ICON:
        Paint_DrawBitMap_Paste(gImage_cloudy,
                               dev->main_element_attr[EPD_MAIN_SCREEN_ELEMENT_WEATHER_ICON].x_start,
                               dev->main_element_attr[EPD_MAIN_SCREEN_ELEMENT_WEATHER_ICON].y_start,
                               48, 48, 1);

        break;
    default:

        break;
    }
    return 0;
}

uint8_t epd_page_main_init(epd_dev_v2_t *dev)
{
    dev->module_start_callback(); // 激活屏幕

    EPD_2IN7_V2_Init(dev);  //
    EPD_2IN7_V2_Clear(dev); // 清屏

    // 设置一帧图片缓存, 接下来就是绘制缓存内容, 最后将一帧数据输出到屏幕
    Paint_NewImage(dev->frame_buf, EPD_2IN7_V2_WIDTH, EPD_2IN7_V2_HEIGHT, 90, WHITE);
    // 屏幕显存设置颜色
    Paint_Clear(WHITE);
    Paint_SelectImage(dev->frame_buf);

    // 初始化屏幕组件坐标
    dev->main_element_attr[EPD_MAIN_SCREEN_ELEMENT_TIME].x_start = EPD_2IN7_V2_WIDTH / 2 + 15;
    dev->main_element_attr[EPD_MAIN_SCREEN_ELEMENT_TIME].x_end = EPD_2IN7_V2_WIDTH / 2 + 115;
    dev->main_element_attr[EPD_MAIN_SCREEN_ELEMENT_TIME].y_start = 80;
    dev->main_element_attr[EPD_MAIN_SCREEN_ELEMENT_TIME].y_end = 80 + Font24.Height; // 时间

    dev->main_element_attr[EPD_MAIN_SCREEN_ELEMENT_DATE].x_start = EPD_2IN7_V2_HEIGHT / 5 + 10;
    dev->main_element_attr[EPD_MAIN_SCREEN_ELEMENT_DATE].x_end = EPD_2IN7_V2_HEIGHT / 5 + 10 + 10 * Font20.Width;
    dev->main_element_attr[EPD_MAIN_SCREEN_ELEMENT_DATE].y_end = EPD_2IN7_V2_WIDTH / 6 * 5 + Font20.Height;
    dev->main_element_attr[EPD_MAIN_SCREEN_ELEMENT_DATE].y_start = EPD_2IN7_V2_WIDTH / 6 * 5; // 日期

    dev->main_element_attr[EPD_MAIN_SCREEN_ELEMENT_T].x_start = EPD_2IN7_V2_HEIGHT / 3 + 10;
    dev->main_element_attr[EPD_MAIN_SCREEN_ELEMENT_T].y_start = 10; // 温度
    dev->main_element_attr[EPD_MAIN_SCREEN_ELEMENT_H].x_start = EPD_2IN7_V2_HEIGHT / 3 + 10;
    dev->main_element_attr[EPD_MAIN_SCREEN_ELEMENT_H].y_start = 30; // 湿度

    dev->main_element_attr[EPD_MAIN_SCREEN_ELEMENT_WEATHER_ICON].x_start = 20;
    dev->main_element_attr[EPD_MAIN_SCREEN_ELEMENT_WEATHER_ICON].y_start = 5; // 天气图片0

    dev->main_element_attr[EPD_MAIN_SCREEN_ELEMENT_WiFi_ICON].x_start = EPD_2IN7_V2_HEIGHT / 3 * 2 + 20;
    dev->main_element_attr[EPD_MAIN_SCREEN_ELEMENT_WiFi_ICON].y_start = 5; // WiFi图片

    // 显示主页面组件
    epd_main_updata(dev, EPD_MAIN_SCREEN_ELEMENT_LINE);
    epd_main_updata(dev, EPD_MAIN_SCREEN_ELEMENT_TIME);
    epd_main_updata(dev, EPD_MAIN_SCREEN_ELEMENT_DATE);
    epd_main_updata(dev, EPD_MAIN_SCREEN_ELEMENT_T);
    epd_main_updata(dev, EPD_MAIN_SCREEN_ELEMENT_H);
    epd_main_updata(dev, EPD_MAIN_SCREEN_ELEMENT_WEATHER_ICON);
    epd_main_updata(dev, EPD_MAIN_SCREEN_ELEMENT_WiFi_ICON);

    EPD_2IN7_V2_Display_Base(dev, dev->frame_buf);
    return 0;
}
/**
 * @name: epd_init
 * @msg: 墨水屏初始化
 * @param {epd_dev_v2_t} *dev           墨水屏设备结构体
 * @param { void }  delay_callback      延时回调函数, 用户实现
 * @param { uint8_t } write_callback    SPI写一字节回调函数, 用户实现
 * @param { uint8_t } start_callback    模块使能回调函数, 用户实现
 * @param { uint8_t } end_callback       模块失能回调函数, 用户实现
 * @param { uint8_t } pin_ctrl_callback  引脚控制回调函数, 用户实现
 * @param { uint8_t } en_refresh_callback  RTOS刷新页面回调函数, 用户实现
 * @return {*}
 * @author: TOTHTOT
 * @date:
 */
uint8_t epd_init(epd_dev_v2_t *dev,
                 void (*delay_callback)(uint32_t ms),
                 uint8_t (*write_callback)(uint8_t data),
                 uint8_t (*start_callback)(void),
                 uint8_t (*end_callback)(void),
                 uint8_t (*pin_ctrl_callback)(epd_pin_ctrl_t pin, uint8_t state)
#if (EPD_USE_RTOS == 1)
                     ,
                 uint8_t (*en_refresh_callback)(epd_dev_v2_t *dev, epd_screen_element_t element)
#endif /* EPD_USE_RTOS */
)
{
    uint8_t ret = 0;

    if (delay_callback == NULL || write_callback == NULL || start_callback == NULL ||
        end_callback == NULL || pin_ctrl_callback == NULL)
    {
        ERROR_PRINT("callback function error!!!");
        ret = 1;
    }

    /* 函数指针初始化 */
    dev->delay_ms_callback = delay_callback;
    dev->module_end_callback = end_callback;
    dev->module_start_callback = start_callback;
    dev->spi_write_byte_callback = write_callback;
    dev->pin_ctrl_callback = pin_ctrl_callback;
#if (EPD_USE_RTOS == 1)
    dev->en_refresh_callback = en_refresh_callback;
#endif /* EPD_USE_RTOS */
       // 清空缓存
    memset(dev->frame_buf, 0, EPD_FRAME_BUF_SIZE);

    dev->enter_system_flag = 0;
    dev->current_page = EPD_PAGE_MAIN;
    dev->current_time.Year = 2023;
    dev->current_time.Month = 1;
    dev->current_time.Day = 1;
    dev->current_time.Hour = 12;
    dev->current_time.Min = 0;
    dev->current_time.Sec = 20;

    epd_page_main_init(dev);
    // epd_page_main(dev);

    // dev->module_end_callback();

    return ret;
}
