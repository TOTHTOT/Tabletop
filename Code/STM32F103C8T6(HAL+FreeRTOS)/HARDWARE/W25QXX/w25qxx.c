/*
 * @Description: w25qxx 的驱动代码, 项目中使用的是W25Q128
 * @Author: TOTHTOT
 * @Date: 2023-07-18 21:32:10
 * @LastEditTime: 2023-08-01 21:38:17
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\Learn\stm32\My_Project\Tabletop\Code\STM32F103C8T6(HAL+FreeRTOS)\HARDWARE\W25QXX\w25qxx.c
 */

#include "w25qxx.h"
#include "spi.h"
#include "delay.h"
#include <string.h>
#include "usart1.h"
#include <stdio.h>

/* 全局变量 */
w25qxx_device_t g_w25qxx_dev_st = {0};
/* 内部函数 */
void w25qxx_wait_busy(w25qxx_device_t *dev);
void w25qxx_erase_sector(uint32_t Dst_Addr, w25qxx_device_t *dev);

/* 用户代码开始 */
/**
 * @name: w25qxx_wr_byte
 * @msg: 读取或者写入一个字节
 * @param {uint8_t} send_data 写入的数据
 * @return {收到的数据}
 * @author: TOTHTOT
 * @date: 2023年7月18日22:22:50
 */
uint8_t w25qxx_wr_byte(uint8_t send_data)
{
    uint8_t recv_data = 0;

    HAL_SPI_TransmitReceive(&hspi1, &send_data, &recv_data, 1, 1000);

    return recv_data; // 返回收到的数据
}

/**
 * @name: w25qxx_cs_ctrl
 * @msg: w25qxx 的cs 引脚控制
 * @param {uint8_t} state == 1 取消片选, == 0 选中
 * @return {== -1 失败, 参数错误; == 0 ,成功}
 * @author: TOTHTOT
 * @date: 2023年7月18日22:31:17
 */
int w25qxx_cs_ctrl(uint8_t state)
{
    if (state == 0 || state == 1)
        HAL_GPIO_WritePin(W25QXX_CS_GPIO_Port, W25QXX_CS_Pin, (GPIO_PinState)state);
    else
        return -1;

    return 0;
}

/**
 * @name: w25qxx_spi_set_speed
 * @msg: 设置spi速度
 * @param {uint8_t} speed 速度
 * @return {*}
 * @author: TOTHTOT
 * @date:
 */
void w25qxx_spi_set_speed(uint8_t speed)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler)); // 判断有效性
    __HAL_SPI_DISABLE(&hspi1);                                      // 关闭SPI
    hspi1.Instance->CR1 &= 0XFFC7;                                  // 位3-5清零，用来设置波特率
    hspi1.Instance->CR1 |= speed;                                   // 设置SPI速度
    __HAL_SPI_ENABLE(&hspi1);                                       // 使能SPI
}
/* 用户代码结束 */

/**
 * @name: w25qxx_read_sr
 * @msg: 读取W25QXX的状态寄存器，W25QXX一共有3个状态寄存器
 * 状态寄存器1：
    BIT7  6   5   4   3   2   1   0
    SPR   RV  TB BP2 BP1 BP0 WEL BUSY
    SPR:默认0,状态寄存器保护位,配合WP使用
    TB,BP2,BP1,BP0:FLASH区域写保护设置
    WEL:写使能锁定
    BUSY:忙标记位(1,忙;0,空闲)
    默认:0x00
    状态寄存器2：
    BIT7  6   5   4   3   2   1   0
    SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
    状态寄存器3：
    BIT7      6    5    4   3   2   1   0
    HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
 * @param {uint8_t} regno   regno:状态寄存器号，范:1~3
 * @param {w25qxx_device_t} *dev
 * @return {状态寄存器值}
 * @author: TOTHTOT
 * @date: 2023年7月18日22:38:45
 */
uint8_t w25qxx_read_sr(uint8_t regno, w25qxx_device_t *dev)
{
    uint8_t byte = 0, command = 0;
    switch (regno)
    {
    case 1:
        command = W25X_ReadStatusReg1; // 读状态寄存器1指令
        break;
    case 2:
        command = W25X_ReadStatusReg2; // 读状态寄存器2指令
        break;
    case 3:
        command = W25X_ReadStatusReg3; // 读状态寄存器3指令
        break;
    default:
        command = W25X_ReadStatusReg1;
        break;
    }
    dev->cs_ctrl_callback(0);      // 使能器件
    dev->wr_callback(command);     // 发送读取状态寄存器命令
    byte = dev->wr_callback(0Xff); // 读取一个字节
    dev->cs_ctrl_callback(1);      // 取消片选
    return byte;
}

/**
 * @name: w25qxx_write_sr
 * @msg: 写W25QXX状态寄存器
 * @param {uint8_t} regno 写那个寄存器 1~3
 * @param {uint8_t} sr 状态
 * @param {w25qxx_device_t} *dev
 * @return {*}
 * @author: TOTHTOT
 * @date: 2023年7月18日22:49:02
 */
void w25qxx_write_sr(uint8_t regno, uint8_t sr, w25qxx_device_t *dev)
{
    uint8_t command = 0;

    switch (regno)
    {
    case 1:
        command = W25X_WriteStatusReg1; // 写状态寄存器1指令
        break;
    case 2:
        command = W25X_WriteStatusReg2; // 写状态寄存器2指令
        break;
    case 3:
        command = W25X_WriteStatusReg3; // 写状态寄存器3指令
        break;
    default:
        command = W25X_WriteStatusReg1;
        break;
    }

    dev->cs_ctrl_callback(0);  // 使能器件
    dev->wr_callback(command); // 发送写取状态寄存器命令
    dev->wr_callback(sr);      // 写入一个字节
    dev->cs_ctrl_callback(1);  // 取消片选
}

/**
 * @name: w25qxx_write_enable
 * @msg: W25QXX写使能, 将WEL置位
 * @param {w25qxx_device_t} *dev    设备
 * @return {无}
 * @author: TOTHTOT
 * @date: 2023年7月18日22:49:41
 */
void w25qxx_write_enable(w25qxx_device_t *dev)
{
    dev->cs_ctrl_callback(0);           // 使能器件
    dev->wr_callback(W25X_WriteEnable); // 发送写使能
    dev->cs_ctrl_callback(1);           // 取消片选
}

/**
 * @name: w25qxx_write_disable
 * @msg: W25QXX写禁止, 将WEL清零
 * @param {w25qxx_device_t} *dev
 * @return {无}
 * @author: TOTHTOT
 * @date: 2023年7月18日22:50:18
 */
void w25qxx_write_disable(w25qxx_device_t *dev)
{
    dev->cs_ctrl_callback(0);            // 使能器件
    dev->wr_callback(W25X_WriteDisable); // 发送写禁止指令
    dev->cs_ctrl_callback(1);            // 取消片选
}

//
// 返回值如下:
/**
 * @name: w25qxx_readid
 * @msg: 读取芯片ID
 * @param {w25qxx_device_t} *dev
 * @return {0XEF13,表示芯片型号为W25Q80
 *          0XEF14,表示芯片型号为W25Q16
 *          0XEF15,表示芯片型号为W25Q32
 *          0XEF16,表示芯片型号为W25Q64
 *          0XEF17,表示芯片型号为W25Q128
 *          0XEF18,表示芯片型号为W25Q256}
 * @author: TOTHTOT
 * @date: 2023年7月18日22:52:50
 */
uint16_t w25qxx_readid(w25qxx_device_t *dev)
{
    uint16_t Temp = 0;

    dev->cs_ctrl_callback(0);
    dev->wr_callback(0x90); // 发送读取ID命令
    dev->wr_callback(0x00);
    dev->wr_callback(0x00);
    dev->wr_callback(0x00);
    Temp |= dev->wr_callback(0xFF) << 8;
    Temp |= dev->wr_callback(0xFF);
    dev->cs_ctrl_callback(1);

    return Temp;
}

/**
 * @name: w25qxx_read_data
 * @msg: 读取SPI FLASH, 在指定地址开始读取指定长度的数据
 * @param {uint8_t} *pBuffer    数据存储区
 * @param {uint32_t} ReadAddr   开始读取的地址(24bit)
 * @param {uint16_t} NumByteToRead  要读取的字节数(最大65535)
 * @param {w25qxx_device_t} *dev    设备
 * @return {无}
 * @author: TOTHTOT
 * @date: 2023年7月18日22:55:47
 */
void w25qxx_read_data(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead, w25qxx_device_t *dev)
{
    uint16_t i;

    dev->cs_ctrl_callback(0);        // 使能器件
    dev->wr_callback(W25X_ReadData); // 发送读取命令

    if (dev->device_type == W25Q256) // 如果是W25Q256的话地址为4字节的，要发送最高8位
    {
        dev->wr_callback((uint8_t)((ReadAddr) >> 24));
    }

    dev->wr_callback((uint8_t)((ReadAddr) >> 16)); // 发送24bit地址
    dev->wr_callback((uint8_t)((ReadAddr) >> 8));
    dev->wr_callback((uint8_t)ReadAddr);

    for (i = 0; i < NumByteToRead; i++)
    {
        pBuffer[i] = dev->wr_callback(0XFF); // 循环读数
    }

    dev->cs_ctrl_callback(1);
}

/**
 * @name: w25qxx_write_page
 * @msg: SPI在一页(0~65535)内写入少于256个字节的数据, 在指定地址开始写入最大256字节的数据
 * @param {uint8_t} *pBuffer    数据存储区
 * @param {uint32_t} WriteAddr  始写入的地址(24bit)
 * @param {uint16_t} NumByteToWrite 要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
 * @param {w25qxx_device_t} *dev    设备
 * @return {无}
 * @author: TOTHTOT
 * @date: 2023年7月18日22:58:33
 */
void w25qxx_write_page(const uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite, w25qxx_device_t *dev)
{
    uint16_t i;

    w25qxx_write_enable(dev);           // SET WEL
    dev->cs_ctrl_callback(0);           // 使能器件
    dev->wr_callback(W25X_PageProgram); // 发送写页命令

    if (dev->device_type == W25Q256) // 如果是W25Q256的话地址为4字节的，要发送最高8位
    {
        dev->wr_callback((uint8_t)((WriteAddr) >> 24));
    }

    dev->wr_callback((uint8_t)((WriteAddr) >> 16)); // 发送24bit地址
    dev->wr_callback((uint8_t)((WriteAddr) >> 8));
    dev->wr_callback((uint8_t)WriteAddr);

    for (i = 0; i < NumByteToWrite; i++)
        dev->wr_callback(pBuffer[i]); // 循环写数

    dev->cs_ctrl_callback(1); // 取消片选
    w25qxx_wait_busy(dev);    // 等待写入结束
}

/**
 * @name: w25qxx_write_nocheck
 * @msg:    无检验写SPI FLASH, 必须确保所写的地址范围内的数据全部为0XFF,
 *          否则在非0XFF处写入的数据将失败!, 具有自动换页功能, 必须确保
 *          所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
 *          在指定地址开始写入指定长度的数据,但是要确保地址不越界!
 * @param {uint8_t} *pBuffer    数据存储区
 * @param {uint32_t} WriteAddr  开始写入的地址(24bit)
 * @param {uint16_t} NumByteToWrite 要写入的字节数(最大65535)
 * @param {w25qxx_device_t} *dev
 * @return {无}
 * @author: TOTHTOT
 * @date: 2023年7月18日23:03:41
 */
void w25qxx_write_nocheck(const uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite, w25qxx_device_t *dev)
{
    uint16_t pageremain;
    pageremain = 256 - WriteAddr % 256; // 单页剩余的字节数
    if (NumByteToWrite <= pageremain)
        pageremain = NumByteToWrite; // 不大于256个字节
    while (1)
    {
        w25qxx_write_page(pBuffer, WriteAddr, pageremain, dev);
        if (NumByteToWrite == pageremain)
            break; // 写入结束了
        else       // NumByteToWrite>pageremain
        {
            pBuffer += pageremain;
            WriteAddr += pageremain;

            NumByteToWrite -= pageremain; // 减去已经写入了的字节数
            if (NumByteToWrite > 256)
                pageremain = 256; // 一次可以写入256个字节
            else
                pageremain = NumByteToWrite; // 不够256个字节了
        }
    };
}

/**
 * @name: w25qxx_write_data
 * @msg: 写SPI FLASH, 在指定地址开始写入指定长度的数据,
 *       该函数带擦除操作!
 * @param {uint8_t} *pBuffer    数据存储区
 * @param {uint32_t} WriteAddr  开始写入的地址(24bit)
 * @param {uint16_t} NumByteToWrite 要写入的字节数(最大65535)
 * @param {w25qxx_device_t} *dev
 * @return {*}
 * @author: TOTHTOT
 * @date:
 */
void w25qxx_write_data(const uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite, w25qxx_device_t *dev)
{
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;
    uint8_t *W25QXX_BUF;

    W25QXX_BUF = dev->read_buf;
    secpos = WriteAddr / 4096; // 扇区地址
    secoff = WriteAddr % 4096; // 在扇区内的偏移
    secremain = 4096 - secoff; // 扇区剩余空间大小
    // printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
    if (NumByteToWrite <= secremain)
        secremain = NumByteToWrite; // 不大于4096个字节
    while (1)
    {
        w25qxx_read_data(W25QXX_BUF, secpos * 4096, 4096, dev); // 读出整个扇区的内容
        for (i = 0; i < secremain; i++)                         // 校验数据
        {
            if (W25QXX_BUF[secoff + i] != 0XFF)
                break; // 需要擦除
        }
        if (i < secremain) // 需要擦除
        {
            w25qxx_erase_sector(secpos, dev); // 擦除这个扇区
            for (i = 0; i < secremain; i++)   // 复制
            {
                W25QXX_BUF[i + secoff] = pBuffer[i];
            }
            w25qxx_write_nocheck(W25QXX_BUF, secpos * 4096, 4096, dev); // 写入整个扇区
        }
        else
            w25qxx_write_nocheck(pBuffer, WriteAddr, secremain, dev); // 写已经擦除了的,直接写入扇区剩余区间.
        if (NumByteToWrite == secremain)
            break; // 写入结束了
        else       // 写入未结束
        {
            secpos++;   // 扇区地址增1
            secoff = 0; // 偏移位置为0

            pBuffer += secremain;        // 指针偏移
            WriteAddr += secremain;      // 写地址偏移
            NumByteToWrite -= secremain; // 字节数递减
            if (NumByteToWrite > 4096)
                secremain = 4096; // 下一个扇区还是写不完
            else
                secremain = NumByteToWrite; // 下一个扇区可以写完了
        }
    };
}

/**
 * @name: w25qxx_erase_chip
 * @msg: 擦除整个芯片, 等待时间超长
 * @param {w25qxx_device_t} *dev 设备
 * @return {无}
 * @author: TOTHTOT
 * @date: 2023年7月18日23:07:36
 */
void w25qxx_erase_chip(w25qxx_device_t *dev)
{
    w25qxx_write_enable(dev); // SET WEL

    w25qxx_wait_busy(dev);

    dev->cs_ctrl_callback(0);         // 使能器件
    dev->wr_callback(W25X_ChipErase); // 发送片擦除命令
    dev->cs_ctrl_callback(1);         // 取消片选

    w25qxx_wait_busy(dev); // 等待芯片擦除结束
}

/**
 * @name: w25qxx_erase_sector
 * @msg: 擦除一个扇区, 最少时间:150ms
 * @param {uint32_t} Dst_Addr   扇区地址 根据实际容量设置
 * @param {w25qxx_device_t} *dev    设备
 * @return {*}
 * @author: TOTHTOT
 * @date: 2023年7月18日23:09:19
 */
void w25qxx_erase_sector(uint32_t Dst_Addr, w25qxx_device_t *dev)
{
    Dst_Addr *= 4096;
    w25qxx_write_enable(dev); // SET WEL
    w25qxx_wait_busy(dev);
    dev->cs_ctrl_callback(0);           // 使能器件
    dev->wr_callback(W25X_SectorErase); // 发送扇区擦除指令
    if (dev->device_type == W25Q256)    // 如果是W25Q256的话地址为4字节的，要发送最高8位
    {
        dev->wr_callback((uint8_t)((Dst_Addr) >> 24));
    }
    dev->wr_callback((uint8_t)((Dst_Addr) >> 16)); // 发送24bit地址
    dev->wr_callback((uint8_t)((Dst_Addr) >> 8));
    dev->wr_callback((uint8_t)Dst_Addr);
    dev->cs_ctrl_callback(1); // 取消片选
    w25qxx_wait_busy(dev);    // 等待擦除完成
}

/**
 * @name: w25qxx_wait_busy
 * @msg: 等待空闲
 * @param {w25qxx_device_t} *dev    设备
 * @return {*}
 * @author: TOTHTOT
 * @date:
 */
void w25qxx_wait_busy(w25qxx_device_t *dev)
{
    while ((w25qxx_read_sr(1, dev) & 0x01) == 0x01)
        ; // 等待BUSY位清空
}

/**
 * @name: w25qxx_power_down
 * @msg: 进入掉电模式
 * @param {w25qxx_device_t} *dev
 * @return {*}
 * @author: TOTHTOT
 * @date:
 */
void w25qxx_power_down(w25qxx_device_t *dev)
{
    dev->cs_ctrl_callback(0);         // 使能器件
    dev->wr_callback(W25X_PowerDown); // 发送掉电命令
    dev->cs_ctrl_callback(1);         // 取消片选
    delay_us(3);                      // 等待TPD
}

/**
 * @name: w25qxx_wakeup
 * @msg: 唤醒
 * @param {w25qxx_device_t} *dev
 * @return {*}
 * @author: TOTHTOT
 * @date:
 */
void w25qxx_wakeup(w25qxx_device_t *dev)
{
    dev->cs_ctrl_callback(0);                // 使能器件
    dev->wr_callback(W25X_ReleasePowerDown); //  send W25X_PowerDown command 0xAB
    dev->cs_ctrl_callback(1);                // 取消片选
    delay_us(3);                             // 等待TRES1
}

/**
 * @name:
 * @msg:
 * @param {w25qxx_device_t} *dev 结构体
 * @return {== 0 数据有效, == 1 数据无效}
 * @author: TOTHTOT
 * @date:
 */
/**
 * @name: w25qxx_check_flag
 * @msg: 检查芯片数据是否有效
 * @param {w25qxx_device_t} *dev
 * @param {uint32_t} *flag_adder 检查的标志的地址
 * @param {uint8_t} *flag 检查的标志
 * @param {uint8_t} flag_len 标志长度
 * @return {== 0 数据有效, == 1 数据无效}
 * @author: TOTHTOT
 * @date: 2023年8月1日
 */
uint8_t w25qxx_check_flag(w25qxx_device_t *dev, uint32_t flag_adder, const uint8_t *flag, uint8_t flag_len)
{
    uint8_t ret = 0;

    dev->read_data_cb(dev->read_buf, flag_adder, flag_len, dev);
    if (strncmp((const char *)dev->read_buf, (const char *)flag, flag_len) != 0)
    {
        ret = 1;
    }

    return ret;
}

/**
 * @name: w25qxx_init
 * @msg: w25qxx 初始化
 * @param {w25qxx_device_t} *dev
 * @return {*}
 * @author: TOTHTOT
 * @date: 2023年7月18日21:52:59
 */
uint32_t w25qxx_init(w25qxx_device_t *dev)
{
    uint32_t ret = 0;
    uint8_t temp = 0;

    // 初始化函数指针
    dev->wr_callback = w25qxx_wr_byte;
    dev->cs_ctrl_callback = w25qxx_cs_ctrl;
    dev->set_speed_callback = w25qxx_spi_set_speed;
    dev->read_data_cb = w25qxx_read_data;
    dev->write_data_cb = w25qxx_write_data;
    dev->erase_sector_cb = w25qxx_erase_sector;
    dev->check_flag_cb = w25qxx_check_flag;

    dev->cs_ctrl_callback(1); // 取消选中

    dev->set_speed_callback(SPI_BAUDRATEPRESCALER_2); // 设置为42M时钟,高速模式
    dev->device_type = w25qxx_readid(dev);            // 读取FLASH ID.

    // 读取ID错误的话就标记设备不在线
    if (dev->device_type == 0 || dev->device_type == 0xffff)
    {
        INFO_PRINT("offline\r\n");
        dev->state_em = W25QXX_STATE_OFFLINE;
    }
    else
    {
        INFO_PRINT("online\r\n");
        dev->state_em = W25QXX_STATE_ONLINE;
    }

    if (dev->device_type == W25Q256) // SPI FLASH为W25Q256
    {
        temp = w25qxx_read_sr(3, dev); // 读取状态寄存器3，判断地址模式
        if ((temp & 0X01) == 0)        // 如果不是4字节地址模式,则进入4字节地址模式
        {
            dev->cs_ctrl_callback(0);               // 选中
            dev->wr_callback(W25X_Enable4ByteAddr); // 发送进入4字节地址模式指令
            dev->cs_ctrl_callback(1);               // 取消片选
        }
    }

    ret = dev->device_type;

    return ret;
}
