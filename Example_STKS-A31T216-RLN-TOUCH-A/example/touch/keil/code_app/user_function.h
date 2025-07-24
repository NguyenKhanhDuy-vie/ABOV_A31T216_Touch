/**
 *******************************************************************************
 * @file        user_function.h
 * @author      ABOV R&D Division
 * @brief       A31T21x User Function Header
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

#ifndef _USER_H_
#define _USER_H_


/*************************
    Function Feature
*************************/
#define MCLK_SPEED              MCLK_32MHZ      /* MCLK_32MHZ or MCLK_16MHZ */

#define LED_DRV_EN              1
#define TS_LED_TIME_DIV_EN      1

#define UART_DEBUG_EN           1
#define I2C_DEBUG_EN            0
#define WDT_EN                  1

#define TOUCH_FREQ_SEL_EN           1
#define USER_FILTER_ONESENSING_EN   0
#define USER_FILTER_DIFFDATA_EN     0     

#define HOLD_KEY_RELEASE_EN     1
#define BIG_DIFF_CANCEL_EN      1
#define MULTI_KEY_RESET_EN      0
#define FIRST_KEY_PRIORITY_EN   0   
#define CHANNEL_GROUPING_EN     0

/************************************************************/
/* Option Function. This feature requires the addition of a separate UL60730 Library files */
#define UL60730_SELF_TEST_EN    0               
/************************************************************/
/* Option Function. This feature requires the addition of a separate gesture code file */
#define WHEEL_FUNCTION_EN       0               
#define SLIDE_FUNCTION_EN       0               
/************************************************************/


/*************************
    WDT Parameter
*************************/
#if (WDT_EN == 1)
#define WDT_RESET_TIME_RATE     3
#endif

/*************************
    LED Parameter
*************************/
#if (LED_DRV_EN == 1)
#define LED_CURRENT             LED_CURRENT_13mA

#define LED_USE_COM_NUM         8
#define TIME_LED_OPTERATING     8
#define TIME_LED_INTERVAL       15

/*************************
    LED COM-SEG
*************************/
#define COM00_EN    1
#define COM01_EN    1
#define COM02_EN    1
#define COM03_EN    1
#define COM04_EN    1
#define COM05_EN    1
#define COM06_EN    0
#define COM07_EN    1
#define COM08_EN    1
#define COM09_EN    0
#define COM10_EN    0
#define COM11_EN    0
#define COM12_EN    0
#define COM13_EN    0
#define SEG00_EN    1
#define SEG01_EN    1
#define SEG02_EN    1
#define SEG03_EN    1
#define SEG04_EN    1
#define SEG05_EN    1
#define SEG06_EN    1
#define SEG07_EN    1
#define SEG08_EN    1
#define SEG09_EN    1
#define SEG10_EN    1
#define SEG11_EN    1
#define SEG12_EN    1
#define SEG13_EN    1
#define SEG14_EN    1
#define SEG15_EN    1

/*************************
    T-Type LED
*************************/
#define A1  BIT(0)
#define B1  BIT(0)
#define C1  BIT(0)
#define D1  BIT(0)
#define E1  BIT(0)
#define F1  BIT(0)
#define G1  BIT(1)
#define A2  BIT(1)
#define B2  BIT(1)
#define C2  BIT(1)
#define D2  BIT(2)
#define E2  BIT(2)
#define F2  BIT(2)
#define G2  BIT(2)
#define A3  BIT(3)
#define B3  BIT(3)
#define C3  BIT(4)
#define D3  BIT(4)
#define E3  BIT(6)
#define F3  BIT(3)
#define G3  BIT(3)
#define A4  BIT(3)
#define B4  BIT(3)
#define C4  BIT(3)
#define D4  BIT(7)
#define E4  BIT(7)
#define F4  BIT(7)
#define G4  BIT(7)

/*************************
    M-Type LED
*************************/
#define SEG7_A  BIT(8)  
#define SEG7_B  BIT(9)
#define SEG7_C  BIT(10)
#define SEG7_D  BIT(11) 
#define SEG7_E  BIT(12)
#define SEG7_F  BIT(13) 
#define SEG7_G  BIT(14)
#define DIGIT_0 (SEG7_A|SEG7_B|SEG7_C|SEG7_D|SEG7_E|SEG7_F)
#define DIGIT_1 (SEG7_B|SEG7_C)
#define DIGIT_2 (SEG7_A|SEG7_B|SEG7_D|SEG7_E|SEG7_G)
#define DIGIT_3 (SEG7_A|SEG7_B|SEG7_C|SEG7_D|SEG7_G)
#define DIGIT_4 (SEG7_B|SEG7_C|SEG7_F|SEG7_G)
#define DIGIT_5 (SEG7_A|SEG7_C|SEG7_D|SEG7_F|SEG7_G)
#define DIGIT_6 (SEG7_A|SEG7_C|SEG7_D|SEG7_E|SEG7_F|SEG7_G)
#define DIGIT_7 (SEG7_A|SEG7_B|SEG7_C|SEG7_F)
#define DIGIT_8 (SEG7_A|SEG7_B|SEG7_C|SEG7_D|SEG7_E|SEG7_F|SEG7_G)
#define DIGIT_9 (SEG7_A|SEG7_B|SEG7_C|SEG7_D|SEG7_F|SEG7_G)
#define DIGIT_A (SEG7_A|SEG7_B|SEG7_C|SEG7_E|SEG7_F|SEG7_G)
#define DIGIT_B (SEG7_C|SEG7_D|SEG7_E|SEG7_F|SEG7_G)
#define DIGIT_C (SEG7_D|SEG7_E|SEG7_G)
#define DIGIT_D (SEG7_B|SEG7_C|SEG7_D|SEG7_E|SEG7_G)
#define DIGIT_E (SEG7_A|SEG7_D|SEG7_E|SEG7_F|SEG7_G)
#define DIGIT_F (SEG7_A|SEG7_E|SEG7_F|SEG7_G)

#define SEG8    BIT(0)
#define SEG9    BIT(1)
#define SEG10   BIT(2)
#define SEG11   BIT(3)
#define SEG12   BIT(4)
#define SEG13   BIT(5)
#define SEG14   BIT(6)
#define SEG15   BIT(7)

typedef struct
{
    uint8_t digit[4];
    uint8_t preDigit[4];
} str_led;

/*************************
    don't modify
*************************/
#define COMSEL_00_12    ((COM12_EN<<12)+(COM11_EN<<11)+(COM10_EN<<10)+(COM09_EN<<9)+(COM08_EN<<8)+(COM07_EN<<7)+(COM06_EN<<6)+(COM05_EN<<5)+    \
                         (COM04_EN<<4)+(COM03_EN<<3)+(COM02_EN<<2)+(COM01_EN<<1)+(COM00_EN<<0))
#define SEGSEL_00_15    ((SEG15_EN<<15)+(SEG14_EN<<14)+(SEG13_EN<<13)+(SEG12_EN<<12)+(SEG11_EN<<11)+(SEG10_EN<<10)+(SEG09_EN<<9)+(SEG08_EN<<8)+ \
                         (SEG07_EN<<7)+(SEG06_EN<<6)+(SEG05_EN<<5)+(SEG04_EN<<4)+(SEG03_EN<<3)+(SEG02_EN<<2)+(SEG01_EN<<1)+(SEG00_EN<<0))
#endif


/*************************
(mapping: software base)
        MULTI KEY CH : Allow multi key
*************************/
#if (MULTI_KEY_RESET_EN  == 1)
#define MAX_MULTY_KEY_NUM   3

#define CH00_MULTI_USE      1   
#define CH01_MULTI_USE      1   
#define CH02_MULTI_USE      1   
#define CH03_MULTI_USE      1   
#define CH04_MULTI_USE      0   
#define CH05_MULTI_USE      0   
#define CH06_MULTI_USE      0   
#define CH07_MULTI_USE      0   
#define CH08_MULTI_USE      0   
#define CH09_MULTI_USE      0   
#define CH10_MULTI_USE      0
#define CH11_MULTI_USE      0
#define CH12_MULTI_USE      0
#define CH13_MULTI_USE      0
#define CH14_MULTI_USE      0
#define CH15_MULTI_USE      0
#define CH16_MULTI_USE      0
#define CH17_MULTI_USE      0
#define CH18_MULTI_USE      0
#define CH19_MULTI_USE      0
#define CH20_MULTI_USE      0
#define CH21_MULTI_USE      0
#define CH22_MULTI_USE      0
#define CH23_MULTI_USE      0   

#define MULTY_CH_BIT    (((uint32_t)CH00_MULTI_USE<<0)+((uint32_t)CH01_MULTI_USE<<1)+((uint32_t)CH02_MULTI_USE<<2)+((uint32_t)CH03_MULTI_USE<<3)+((uint32_t)CH04_MULTI_USE<<4)+ \
                        ((uint32_t)CH05_MULTI_USE<<5)+((uint32_t)CH06_MULTI_USE<<6)+((uint32_t)CH07_MULTI_USE<<7)+((uint32_t)CH08_MULTI_USE<<8)+((uint32_t)CH09_MULTI_USE<<9)+        \
                        ((uint32_t)CH10_MULTI_USE<<10)+((uint32_t)CH11_MULTI_USE<<11)+((uint32_t)CH12_MULTI_USE<<12)+((uint32_t)CH13_MULTI_USE<<13)+((uint32_t)CH14_MULTI_USE<<14)+\
                        ((uint32_t)CH15_MULTI_USE<<15)+((uint32_t)CH16_MULTI_USE<<16)+((uint32_t)CH17_MULTI_USE<<17)+((uint32_t)CH18_MULTI_USE<<18)+((uint32_t)CH19_MULTI_USE<<19)+\
                        ((uint32_t)CH20_MULTI_USE<<20)+((uint32_t)CH21_MULTI_USE<<21)+((uint32_t)CH22_MULTI_USE<<22)+((uint32_t)CH23_MULTI_USE<<23))
#endif

/*************************
(mapping: software base)
        priority KEY CH : high (1) / Low (0)
*************************/
#if (FIRST_KEY_PRIORITY_EN == 1)
#define CH00_PRIORITY_LEVEL     0
#define CH01_PRIORITY_LEVEL     0
#define CH02_PRIORITY_LEVEL     0   
#define CH03_PRIORITY_LEVEL     0   
#define CH04_PRIORITY_LEVEL     0   
#define CH05_PRIORITY_LEVEL     0   
#define CH06_PRIORITY_LEVEL     0   
#define CH07_PRIORITY_LEVEL     0  
#define CH08_PRIORITY_LEVEL     0   
#define CH09_PRIORITY_LEVEL     0   
#define CH10_PRIORITY_LEVEL     0
#define CH11_PRIORITY_LEVEL     0
#define CH12_PRIORITY_LEVEL     0
#define CH13_PRIORITY_LEVEL     0
#define CH14_PRIORITY_LEVEL     0
#define CH15_PRIORITY_LEVEL     0
#define CH16_PRIORITY_LEVEL     0
#define CH17_PRIORITY_LEVEL     0
#define CH18_PRIORITY_LEVEL     0
#define CH19_PRIORITY_LEVEL     0
#define CH20_PRIORITY_LEVEL     0
#define CH21_PRIORITY_LEVEL     0
#define CH22_PRIORITY_LEVEL     0   
#define CH23_PRIORITY_LEVEL     0   

/************************************************************** don't modify *************************************************************/
#define PRIORITY_CH_BIT (((uint32_t)CH00_PRIORITY_LEVEL<<0)|((uint32_t)CH01_PRIORITY_LEVEL<<1)|((uint32_t)CH02_PRIORITY_LEVEL<<2)|((uint32_t)CH03_PRIORITY_LEVEL<<3)|((uint32_t)CH04_PRIORITY_LEVEL<<4)| \
                        ((uint32_t)CH05_PRIORITY_LEVEL<<5)|((uint32_t)CH06_PRIORITY_LEVEL<<6)|((uint32_t)CH07_PRIORITY_LEVEL<<7)|((uint32_t)CH08_PRIORITY_LEVEL<<8)|((uint32_t)CH09_PRIORITY_LEVEL<<9)| \
                        ((uint32_t)CH10_PRIORITY_LEVEL<<10)|((uint32_t)CH11_PRIORITY_LEVEL<<11)|((uint32_t)CH12_PRIORITY_LEVEL<<12)|((uint32_t)CH13_PRIORITY_LEVEL<<13)|((uint32_t)CH14_PRIORITY_LEVEL<<14)| \
                        ((uint32_t)CH15_PRIORITY_LEVEL<<15)|((uint32_t)CH16_PRIORITY_LEVEL<<16)|((uint32_t)CH17_PRIORITY_LEVEL<<17)|((uint32_t)CH18_PRIORITY_LEVEL<<18)|((uint32_t)CH19_PRIORITY_LEVEL<<19)| \
                        ((uint32_t)CH20_PRIORITY_LEVEL<<20)|((uint32_t)CH21_PRIORITY_LEVEL<<21)|((uint32_t)CH22_PRIORITY_LEVEL<<22)|((uint32_t)CH23_PRIORITY_LEVEL<<23)) 

/******************************************************************************************************************************************/
#endif

#if (CHANNEL_GROUPING_EN == 1)
/*************************
(mapping: software base)
        Group ID : 1~255 (0: no group)
*************************/
#define CH00_GROUP_ID       0
#define CH01_GROUP_ID       0
#define CH02_GROUP_ID       0
#define CH03_GROUP_ID       0
#define CH04_GROUP_ID       0
#define CH05_GROUP_ID       0
#define CH06_GROUP_ID       0
#define CH07_GROUP_ID       0
#define CH08_GROUP_ID       0
#define CH09_GROUP_ID       0
#define CH10_GROUP_ID       0
#define CH11_GROUP_ID       0
#define CH12_GROUP_ID       0
#define CH13_GROUP_ID       0
#define CH14_GROUP_ID       0
#define CH15_GROUP_ID       0
#define CH16_GROUP_ID       0
#define CH17_GROUP_ID       0
#define CH18_GROUP_ID       0
#define CH19_GROUP_ID       0
#define CH20_GROUP_ID       0
#define CH21_GROUP_ID       0
#define CH22_GROUP_ID       0
#define CH23_GROUP_ID       0
#endif


/*************************
    TOUCH POS NOISE Parameter
*************************/
#if (BIG_DIFF_CANCEL_EN == 1)
#define BIG_DIFF_THD_CH00   2000 
#define BIG_DIFF_THD_CH01   2000 
#define BIG_DIFF_THD_CH02   2000 
#define BIG_DIFF_THD_CH03   2000 
#define BIG_DIFF_THD_CH04   2000 
#define BIG_DIFF_THD_CH05   2000 
#define BIG_DIFF_THD_CH06   2000 
#define BIG_DIFF_THD_CH07   2000 
#define BIG_DIFF_THD_CH08   2000 
#define BIG_DIFF_THD_CH09   2000 
#define BIG_DIFF_THD_CH10   2000 
#define BIG_DIFF_THD_CH11   2000 
#define BIG_DIFF_THD_CH12   2000 
#define BIG_DIFF_THD_CH13   2000 
#define BIG_DIFF_THD_CH14   2000 
#define BIG_DIFF_THD_CH15   2000 
#define BIG_DIFF_THD_CH16   2000 
#define BIG_DIFF_THD_CH17   2000 
#define BIG_DIFF_THD_CH18   2000 
#define BIG_DIFF_THD_CH19   2000 
#define BIG_DIFF_THD_CH20   2000 
#define BIG_DIFF_THD_CH21   2000 
#define BIG_DIFF_THD_CH22   2000 
#define BIG_DIFF_THD_CH23   2000 
#endif

#if (HOLD_KEY_RELEASE_EN == 1)
#define TIME_HOLD_KEY_RELEASE   10000
#endif

#if (USER_FILTER_DIFFDATA_EN == 1)
#define RAW_Q_SIZE              3       
#endif

#if (USER_FILTER_ONESENSING_EN == 1)
// median filter
#define FILTER_SIZE_MID         4
#define MEDIAN_AVG_FIRST        (FILTER_SIZE_MID>>1)
#define MEDIAN_AVG_SEC          (MEDIAN_AVG_FIRST+1)
// average filter
#define FILTER_SIZE_MAX         10
#endif

/*************************
    UART DEBUG Parameter
*************************/
#if (UART_DEBUG_EN == 1)
#define UART_BAUDRATE           115200
#endif

#if (I2C_DEBUG_EN == 1)
#define SLAVE_ADDR              0x20                        // Real Address : 0x40 (0x20 << 1)
#define I2C_GENERAL_CALL        0                           // Enable :1 , Disable : 0
#define I2C_FREQ                I2C_FREQ_400KHZ
#define I2C_IRQ_PRIO            DRIVER_PRIORITY_HIGHEST     //[Highst : Lowest = 0:15]
#endif

#if ((HOLD_KEY_RELEASE_EN | MULTI_KEY_RESET_EN | FIRST_KEY_PRIORITY_EN | BIG_DIFF_CANCEL_EN | CHANNEL_GROUPING_EN | USER_FILTER_DIFFDATA_EN | USER_FILTER_ONESENSING_EN | I2C_DEBUG_EN | UL60730_SELF_TEST_EN) == 1)
struct user_param
{
#if (CHANNEL_GROUPING_EN == 1)
    uint8_t ch_group_set[TS_MAX_KEY];
    uint8_t group_idx;
#endif

#if (HOLD_KEY_RELEASE_EN == 1)
    uint16_t hold_key_release_time;
#endif
#if (FIRST_KEY_PRIORITY_EN == 1)
    uint32_t priority_ch_idx;  
#endif

#if (MULTI_KEY_RESET_EN  == 1)
    uint32_t multi_ch_bit;
    uint8_t  multi_key_limit;
    uint16_t multi_key_check;
#endif

#if (BIG_DIFF_CANCEL_EN == 1)
    uint16_t big_diff_thd[TS_MAX_KEY];
#endif

#if (USER_FILTER_DIFFDATA_EN == 1)
    uint16_t diff_queue[TS_MAX_KEY][RAW_Q_SIZE];
    uint8_t diff_queue_idx; 
#endif

#if (USER_FILTER_ONESENSING_EN == 1)
    uint16_t medDataBuf[TS_MAX_KEY][2][FILTER_SIZE_MID+1];
    uint16_t avgDataBuf[TS_MAX_KEY][2][FILTER_SIZE_MAX+1]; 
#endif

#if (I2C_DEBUG_EN == 1)
    uint16_t *dataBuff;
#endif

#if (UL60730_SELF_TEST_EN == 1)
    uint32_t ul60730_post_result;
    uint32_t ul60730_bist_result;
#endif
};

extern struct user_param user;
#endif


#if (LED_DRV_EN == 1)
void led_RenewData(void);
#endif

#if (USER_FILTER_DIFFDATA_EN == 1)
void TS_User_Filter_diffdata(void);
#endif

#if (USER_FILTER_ONESENSING_EN == 1)
void TS_User_Filter_onesensingdata(void);
uint16_t filter_median (uint16_t input, uint16_t* medDataBuf);
uint16_t filter_moving_average (uint16_t input, uint16_t* avgDataBuf);
#endif

#if (TOUCH_FREQ_SEL_EN == 1)
void Touch_Freq_Sel(void);
#endif

#if (BIG_DIFF_CANCEL_EN == 1)
extern uint16_t big_diff_thd[TS_MAX_KEY];
#endif

#if (CHANNEL_GROUPING_EN == 1)
extern uint8_t key_group_id[TS_MAX_KEY];
#endif

#if ((HOLD_KEY_RELEASE_EN | MULTI_KEY_RESET_EN | FIRST_KEY_PRIORITY_EN | BIG_DIFF_CANCEL_EN | CHANNEL_GROUPING_EN ) == 1)
extern void Touch_Key_Scenario (void);
#endif

#endif




