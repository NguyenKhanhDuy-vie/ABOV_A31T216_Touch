/**
 *******************************************************************************
 * @file        user_function.h
 * @author      ABOV R&D Division
 * @brief       A31T21x Timer Header for Application
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

#ifndef __USERTIMER_H__
#define __USERTIMER_H__

typedef enum 
{
    UT_MIN              = 0x00,
    UT_INIT_DELAY       = 0x01,  
    UT_REVERSE          = 0x02,  
    UT_BASE_UPDATE      = 0x04,
    UT_HOLD_KEY_RELEASE = 0x08,
    UT_SLIDE_EVENT_HOLD = 0x10, 
    UT_WHEEL_EVENT_HOLD = 0x20,
    UT_7                = 0x40,
    UT_8                = 0x80,
    UT_MAX              = 0xff
} eUTIDType;


void UT_Set_Timer(uint16_t m_utid, uint32_t time_ms);
uint16_t UT_Get_Status(void);
void UT_Clear_Timer(uint16_t m_utid);
uint8_t UT_Is_Expired(uint16_t m_utid);
void UT_Run_Timer(void);
void Timer1_ISR(void);


#endif  // __USERTIMER_H__

