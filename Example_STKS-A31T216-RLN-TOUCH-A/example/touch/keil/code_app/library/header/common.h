/**
 *******************************************************************************
 * @file        common.h
 * @author      ABOV R&D Division
 * @brief       A31T21x STKS board common header
 *
 * Copyright 2017 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "a31t21x.h"


#define _A31T21X_

#define ON          1
#define OFF         0
#define TS_MAX_KEY  24

#define MCLK_1MHZ	0
#define MCLK_2MHZ	1
#define MCLK_4MHZ	2
#define MCLK_8MHZ	3
#define MCLK_16MHZ	4
#define MCLK_32MHZ	5	

typedef struct
{
    uint16_t cal_ok         :1; 
    uint16_t led_end        :1;
    uint16_t dbg_trigger    :1;
    uint16_t led_en         :1;
    uint16_t dbg_en         :1;
    uint16_t led_io         :1; 
    uint16_t time_div       :1;        
    uint16_t reserved       :9;
}str_flag;

#define SFR_TS_CON          0x00        //0x0120
#define SFR_TS_MODE         0x01        //0x0124
#define SFR_TS_SUM_CNT      0x02        //0x0128
#define SFR_TS_CH_SEL       0x03        //0x012C
#define SFR_TS_S1_WIDTH     0x04        //0x0130
#define SFR_TS_SLP_CON      0x05        //0x0134
#define SFR_TS_TRIM         0x06        //0x0138
#define SFR_TS_CLK_CFG      0x07        //0x013C
#define SFR_TRIM_OSC        0x08        //0x0140
#define SFR_DELTA_OSC       0x09        //0x0144
#define SFR_TLED            0x0A        //0x0148
#define SFR_TS_VHS          0x0B        //0x014C
#define SFR_TS_VREF         0x0C        //0x0150
#define SFR_TS_RESERVE1     0x0D        //0x0154
#define SFR_TS_RESERVE2     0x0E        //0x0158
#define SFR_TS_RESERVE3     0x0F        //0x015C
#define SFR_SHLD_CON        0x10        //0x0160
#define SFR_TS_MAX          0x11

typedef struct touch_T214
{
    __IO uint32_t RAW[48];              //0x0000~0x00BC
    __IO uint32_t SCO[24];              //0x00C0~0x011C
    __IO uint32_t TS[SFR_TS_MAX];       //0x0120~0x0150
} touch_T214;

#define bSFR                0x40003600UL
#define tSFR                ((touch_T214*)bSFR)
#define TS(X)               (tSFR->TS[X])
#define TS_SCO(X)           (tSFR->SCO[X])
#define TSRAW(X)            (tSFR->RAW[X])

typedef enum{
    TS_RUN      = 0x01,
    TS_IF       = 0x04,                 //Touch Sensor Interrupt Flag
    LED_ST_FLAG = 0x08,                 //LED Driver Start Interrupt Flag
}TS_CON_TYPE;

#define _TS_START (TS_RUN)

enum{
    CS_NONE = 0x000000,
    CS00 = 0x000001,
    CS01 = 0x000002,
    CS02 = 0x000004,
    CS03 = 0x000008,
    CS04 = 0x000010,
    CS05 = 0x000020,
    CS06 = 0x000040,
    CS07 = 0x000080,
    CS08 = 0x000100,
    CS09 = 0x000200,
    CS10 = 0x000400,
    CS11 = 0x000800,
    CS12 = 0x001000,
    CS13 = 0x002000,
    CS14 = 0x004000,
    CS15 = 0x008000,
    CS16 = 0x010000,
    CS17 = 0x020000,
    CS18 = 0x040000,
    CS19 = 0x080000,
    CS20 = 0x100000,
    CS21 = 0x200000,
    CS22 = 0x400000,
    CS23 = 0x800000,
    CS_MAX,
};


extern str_flag t_sysbit;


#endif // __GLOBAL_H__

