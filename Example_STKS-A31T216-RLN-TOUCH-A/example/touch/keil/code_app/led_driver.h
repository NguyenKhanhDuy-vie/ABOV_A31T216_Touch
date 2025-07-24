/**
 *******************************************************************************
 * @file        led_driver.h
 * @author      ABOV R&D Division
 * @brief       A31T21x LED Function Header For Application
 *
 * Copyright 2017 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#include "common.h"

#if (LED_DRV_EN == 1)

#ifndef __LEDDRV_H__
#define __LEDDRV_H__

#define LED_INIT_DATA   0xFFFF
#define SEG(x)  ((u16)1<<x)
#define COM(x)  ((u8)1<<x)
//---------------


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

typedef enum
{
    LED_STATE_INIT_IO = 0x00,
    LED_STATE_INIT_REG,
    LED_STATE_LED_RENEW,    
}LED_state;

enum{
    LED_CURRENT_8mA     = 0,
    LED_CURRENT_13mA    = 1,
    LED_CURRENT_22mA    = 2,
    LED_CURRENT_26mA    = 3,
};

typedef struct {
    uint16_t com_sel;
    uint16_t seg_sel;
    uint8_t com_cnt; 
    uint8_t slot_ms;
    uint8_t period;
    struct{
        uint8_t seg_current :2;
        uint8_t op_mode :3;
        uint8_t rsrvd   :3;
    }flag;
} str_ledx;


extern str_ledx led;
extern void (*fpRenewLED)(void);
	

void LED_Do_Task(void);
void LED_Set_Actv_Time(uint8_t comCnt, uint8_t slotMs, uint8_t period);
void LED_Set_Current(uint8_t currLev);
void LedDriver_ISR(void);
void LED_Init_Reg(void);
void LED_Init_IO(void);
#endif  // __LEDDRV_H__

#endif

