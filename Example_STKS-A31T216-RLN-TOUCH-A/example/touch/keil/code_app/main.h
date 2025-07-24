/**
 *******************************************************************************
 * @file        main.h
 * @author      ABOV R&D Division
 * @brief       A31T21x STKS Board Aplication Main Header
 *
 * Copyright 2017 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#ifndef _MAIN_H_
#define _MAIN_H_


/* Includes ------------------------------------------------------------------*/
#include "a31t21x.h"
#include "a31t21x_pwr.h"
#include "a31t21x_scu.h"
#include "a31t21x_gpio.h"
//#include "A31T21x_hal_fmc.h"
//#include "A31T21x_hal_dmac.h"
#include "a31t21x_wdt.h"
//#include "A31T21x_hal_wt.h"
#include "a31T21x_timer.h"
//#include "A31T21x_hal_timer2n.h"
//#include "A31T21x_hal_timer30.h"
#include "A31T21x_usart1n.h"
#include "A31T21x_uart.h"
//#include "A31T21x_hal_spi.h"
//#include "A31T21x_hal_i2c.h"
//#include "A31T21x_hal_adc.h"
//#include "A31T21x_hal_lcd.h"
//#include "A31T21x_hal_crc.h"
#include "A31T21x_led.h"
#include "a31t21x_driver_common.h"

#include "common.h"
#include "touch_lib.h"
#include "user_timer.h"
#include "user_function.h"
#include "debug.h"
#if (LED_DRV_EN == 1)
#include "led_driver.h"
#endif
#if (UL60730_SELF_TEST_EN == 1)
#include "self_test.h"
#include "UL60730_lib_common_V01.h"
#endif
#if (I2C_DEBUG_EN == 1)
#include "register_map.h"
#include "i2c.h"
#endif
#if ((SLIDE_FUNCTION_EN | WHEEL_FUNCTION_EN) == 1)
#include "gesture.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif  


#if (1)
/* Private typedef -----------------------------------------------------------*/
enum system_clock
{
    SYS_CLK_32MHZ,
    SYS_CLK_16MHZ,
    SYS_CLK_8MHZ,
    SYS_CLK_4MHZ,
    SYS_CLK_2MHZ,
    SYS_CLK_1MHZ
};
typedef enum
{   
    TASK_LED = 0,
    TASK_TOUCH,
    TASK_GESTURE,
    TASK_DEBUG,
    TASK_MAX
} e_task;
#endif

#define APP_WDT_CONFIG                  \
{                                       \
    .clk_src = WDT_CLK_SRC_WDTRC,       \
    .clk_div = WDT_CLK_DIV_4,           \
    .mode = WDT_MODE_RESET,             \
    .window_count = 0x00FFFFFF,         \
    .irq_prio = DRIVER_PRIORITY_LOWEST, \
}


/* Public variables ---------------------------------------------------------*/
extern uint32_t SystemCoreClock;        /*!< System Clock Frequency (Core Clock)  */
extern uint32_t SystemPeriClock;            /*!< System Clock Frequency (Peri Clock)  */
/* Public function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#if (0)
#define APP_LED_CONFIG                      \
{                                           \
    .tPRESD     = 15,                       \
    .tCOMPWID   = 99,                       \
    .tLEDPD     = 1000,                     \
    .tOVERLAP   = 0,                        \
    .tSR_INTE   = 1,                        \
    .tMODE      = LED_AUTO_MODE,            \
    .clk_sel    = LED_CLK_MCCLK,            \
    .irq_prio   = DRIVER_PRIORITY_LOWEST,   \
}
 
#define APP_UART_CONFIG                     \
{                                           \
    .baudrate = 115200,                     \
    .data_bit = UART_DATA_BIT_8,            \
    .parity_bit = UART_PARITY_BIT_NONE,     \
    .stop_bit = UART_STOP_BIT_1,            \
    .irq_prio = DRIVER_PRIORITY_LOWEST,     \
}
#endif       
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Port_Init(void);
void System_Setting_Config(void);
void Port_Configure_I2C(uint8_t id);


#ifdef __cplusplus
}
#endif


#endif /* _MAIN_H_ */


