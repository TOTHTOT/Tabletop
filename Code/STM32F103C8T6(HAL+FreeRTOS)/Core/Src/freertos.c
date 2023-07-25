/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "semphr.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "string.h"
#include <stdio.h>
#include "usart1.h"
#include "EPD_Test.h"
#include "EPD_2in7_V2.h"
#include "w25qxx.h"
#include "ds1307.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId LED_TASKHandle;
osThreadId USART1_TASKHandle;
osThreadId TASK_EPDHandle;
osSemaphoreId Usart1_Receive_BinSemaphoreHandle;
osSemaphoreId en_epd_refreshHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const *argument);
void led_task(void const *argument);
void usart1_task(void const *argument);
void epd_refresh(void const *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = &xIdleStack[0];
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
    /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* Create the semaphores(s) */
    /* definition and creation of Usart1_Receive_BinSemaphore */
    osSemaphoreDef(Usart1_Receive_BinSemaphore);
    Usart1_Receive_BinSemaphoreHandle = osSemaphoreCreate(osSemaphore(Usart1_Receive_BinSemaphore), 1);

    /* definition and creation of en_epd_refresh */
    osSemaphoreDef(en_epd_refresh);
    en_epd_refreshHandle = osSemaphoreCreate(osSemaphore(en_epd_refresh), 1);

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
    xSemaphoreTake(Usart1_Receive_BinSemaphoreHandle, portMAX_DELAY);
    // xSemaphoreTake(en_epd_refreshHandle, portMAX_DELAY);
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */

    /* Create the thread(s) */
    /* definition and creation of defaultTask */
    osThreadDef(defaultTask, StartDefaultTask, osPriorityHigh, 0, 512);
    defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

    /* definition and creation of LED_TASK */
    osThreadDef(LED_TASK, led_task, osPriorityLow, 0, 128);
    LED_TASKHandle = osThreadCreate(osThread(LED_TASK), NULL);

    /* definition and creation of USART1_TASK */
    osThreadDef(USART1_TASK, usart1_task, osPriorityLow, 0, 256);
    USART1_TASKHandle = osThreadCreate(osThread(USART1_TASK), NULL);

    /* definition and creation of TASK_EPD */
    osThreadDef(TASK_EPD, epd_refresh, osPriorityLow, 0, 256);
    TASK_EPDHandle = osThreadCreate(osThread(TASK_EPD), NULL);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const *argument)
{
    /* USER CODE BEGIN StartDefaultTask */
    /* Infinite loop */
    INFO_PRINT("system version %s\r\nbuild date %s, time %s\r\n", APP_VERSION, __DATE__, __TIME__);
    extern uint8_t epd_spi_write_byte(uint8_t data);
    extern uint8_t edp_pin_ctrl(epd_pin_ctrl_t pin, uint8_t state);
    extern uint8_t epd_start(void);
    extern uint8_t epd_end(void);
    extern uint8_t epd_en_refresh(epd_dev_v2_t * dev, epd_screen_element_t element);

    delay_xms(500);
    epd_init(&g_epd_dev, delay_xms, epd_spi_write_byte, epd_start, epd_end, edp_pin_ctrl, epd_en_refresh);
    g_epd_dev.enter_system_flag = 1;
    // 初始化w25qxx
    w25qxx_init(&g_w25qxx_dev);

    vTaskDelete(NULL);
    /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_led_task */
/**
 * @brief Function implementing the LED_TASK thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_led_task */
void led_task(void const *argument)
{
    /* USER CODE BEGIN led_task */
    /* Infinite loop */
    for (;;)
    {
        // INFO_PRINT("led\r\n");
        LED0_TOGGLE;
        delay_ms(500);
    }
    /* USER CODE END led_task */
}

/* USER CODE BEGIN Header_usart1_task */
/**
 * @brief Function implementing the USART1_TASK thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_usart1_task */
void usart1_task(void const *argument)
{
    /* USER CODE BEGIN usart1_task */
    uint8_t len;
    /* Infinite loop */
    for (;;)
    {
        xSemaphoreTake(Usart1_Receive_BinSemaphoreHandle, portMAX_DELAY);
        len = USART_RX_STA & 0x3fff; // get lenth of the data
        printf("data:%s len:%d\r\n", USART_RX_BUF, len);
        memset(USART_RX_BUF, '\0', sizeof(USART_RX_BUF)); // initialization buf
        USART_RX_STA = 0;
        // osDelay(1);
        delay_ms(1);
    }
    /* USER CODE END usart1_task */
}

/* USER CODE BEGIN Header_epd_refresh */
/**
 * @brief Function implementing the TASK_EPD thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_epd_refresh */
void epd_refresh(void const *argument)
{
    /* USER CODE BEGIN epd_refresh */
    /* Infinite loop */
    BaseType_t pxHigherPriorityTaskWoken;
    g_epd_dev.module_start_callback();
    for (;;)
    {
        // xSemaphoreTake(en_epd_refreshHandle, portMAX_DELAY);
        osSemaphoreWait(en_epd_refreshHandle, 1000);
        // xSemaphoreTakeFromISR(en_epd_refreshHandle, &pxHigherPriorityTaskWoken);
        INFO_PRINT("refresh screen element[%d].\r\n", g_epd_dev.refresh_element);
        switch (g_epd_dev.refresh_element)
        {
        case EPD_MAIN_SCREEN_ELEMENT_TIME:
            // g_epd_dev.module_start_callback();
            Paint_Clear(WHITE);
            Paint_DrawRectangle(1, 1, 120, 50, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
            Paint_DrawLine(0, 0, 40, 20, BLACK, DOT_PIXEL_4X4, LINE_STYLE_SOLID);
            Paint_DrawTime(10, 15, &g_epd_dev.current_time, &Font20, WHITE, BLACK);
            uint16_t x = 8;
            EPD_2IN7_V2_Display_Partial(&g_epd_dev, g_epd_dev.frame_buf, x, 0, x+50, 254); // Xstart must be a multiple of 8

            print_current_time(&g_epd_dev.current_time);
            // g_epd_dev.module_end_callback();
            break;
        default:
            break;
        }
        g_epd_dev.refresh_element = EPD_MAIN_SCREEN_ELEMENT_NONE;
        delay_ms(10);
    }
    /* USER CODE END epd_refresh */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
