/**
 *******************************************************************************
 * @file        led_driver.c
 * @author      ABOV R&D Division
 * @brief       A31T21x LED Function Code For Application
 *
 * Copyright 2017 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/



#include "A31T21x.h"
#include "A31T21x_gpio.h"
#include "A31T21x_scu.h"
#include "A31T21x_led.h"
#include "a31t21x_driver_common.h"

#include "touch_lib.h"
#include "user_function.h"
#include "led_driver.h"


#if (LED_DRV_EN == 1)

#define LED_CCSTRIM                 (*(volatile uint32_t*)0x40006074)

str_ledx led;   
str_led t_led;

void (*fpRenewLED) (void);


uint8_t const coT4Digit[4][10][8] = { 
{
    {0,     A1,     B1,     C1,     D1,     0,      E1,     F1},        // 0 : ABCDEF
    {0,     0,      B1,     C1,     0,      0,      0,      0},         // 1 : BC
    {0,     A1,     B1,     0,      D1,     0,      E1,     G1},        // 2 : ABDEG
    {0,     A1,     B1,     C1,     D1,     0,      0,      G1},        // 3 : ABCDG
    {0,     0,      B1,     C1,     0,      0,      0,      F1+G1},     // 4 : BCFG
    {0,     A1,     0,      C1,     D1,     0,      0,      F1+G1},     // 5 : ACDFG
    {0,     A1,     0,      C1,     D1,     0,      E1,     F1+G1},     // 6 : ACDEFG
    {0,     A1,     B1,     C1,     0,      0,      0,      F1},        // 7 : ABCF
    {0,     A1,     B1,     C1,     D1,     0,      E1,     F1+G1},     // 8 : ABCDEFG
    {0,     A1,     B1,     C1,     D1,     0,      0,      F1+G1}      // 9 : ABCDFG
},{
    {0,     0,      C2,     B2+D2,  A2+E2,  0,      F2,     0},         // 0 : ABCDEF
    {0,     0,      C2,     B2,     0,      0,      0,      0},         // 1 : BC
    {0,     0,      0,      B2+D2,  A2+E2,  0,      0,      G2},        // 2    : ABDEG
    {0,     0,      C2,     B2+D2,  A2,     0,      0,      G2},        // 3 : ABCDG
    {0,     0,      C2,     B2,     0,      0,      F2,     G2},        // 4 : BCFG
    {0,     0,      C2,     D2,     A2,     0,      F2,     G2},        // 5    : ACDFG
    {0,     0,      C2,     D2,     A2+E2,  0,      F2,     G2},        // 6 : ACDEFG
    {0,     0,      C2,     B2,     A2,     0,      F2,     G2},        // 7 : ABCF
    {0,     0,      C2,     B2+D2,  A2+E2,  0,      F2,     G2},        // 8 : ABCDEFG
    {0,     0,      C2,     B2+D2,  A2,     0,      F2,     G2}         // 9 : ABCDFG
},{
    {0,     0,      0,      0,      B3,     0,      A3+C3,  D3+E3+F3},  // 0 : ABCDEF
    {0,     0,      0,      0,      B3,     0,      C3,     0},         // 1 : BC
    {0,     0,      0,      0,      B3,     G3,     A3,     D3+E3},     // 2    : ABDEG 
    {0,     0,      0,      0,      B3,     G3,     A3+C3,  D3},        // 3 : ABCDG
    {0,     0,      0,      0,      B3,     G3,     C3,     F3},        // 4 : BCFG
    {0,     0,      0,      0,      0,      G3,     A3+C3,  D3+F3},     // 5    : ACDFG
    {0,     0,      0,      0,      0,      G3,     A3+C3,  D3+E3+F3},  // 6 : ACDEFG
    {0,     0,      0,      0,      B3,     0,      A3+C3,  F3},        // 7 : ABCF
    {0,     0,      0,      0,      B3,     G3,     A3+C3,  D3+E3+F3},  // 8 : ABCDEFG
    {0,     0,      0,      0,      B3,     G3,     A3+C3,  D3+F3}      // 9 : ABCDFG
},{
    {C4,    B4,     A4+F4,  E4,     0,      D4,     0,      0},         // 0 : ABCDEF
    {C4,    B4,         0,  0,      0,      0,      0,      0},         // 1 : BC
    {0,     B4+G4,  A4,     E4,     0,      D4,     0,      0},         // 2    : ABDEG
    {C4,    B4+G4,  A4,     0,      0,      D4,     0,      0},         // 3 : ABCDG
    {C4,    B4+G4,  F4,     0,      0,      0,      0,      0},         // 4 : BCFG
    {C4,    G4,     A4+F4,  0,      0,      D4,     0,      0},         // 5    : ACDFG
    {C4,    G4,     A4+F4,  E4,     0,      D4,     0,      0},         // 6 : ACDEFG
    {C4,    B4,     A4+F4,  0,      0,      0,      0,      0},         // 7 : ABCF
    {C4,    B4+G4,  A4+F4,  E4,     0,      D4,     0,      0},         // 8 : ABCDEFG
    {C4,    B4+G4,  A4+F4,  0,      0,      D4,     0,      0}          // 9 : ABCDFG
}}; 


uint8_t const coDigit[9][8] = {
    {0x00,0x7E,0xFF,0x81,0x81,0xFF,0x7E,0x00},  /* 0 */
    {0x00,0x02,0x02,0xFF,0xFF,0x00,0x00,0x00},  /* 1 */
    {0x00,0xC6,0xE7,0xB1,0x99,0x8F,0x86,0x00},  /* 2 */
    {0x00,0x42,0xCB,0x89,0x89,0xFF,0x76,0x00},  /* 3 */
    {0x00,0x30,0x3C,0x2F,0xFF,0xFF,0x20,0x00},  /* 4 */
    {0x00,0x4F,0xCF,0x89,0x89,0xF9,0x71,0x00},  /* 5 */
    {0x00,0x7E,0xFF,0x89,0x89,0xFB,0x72,0x00},  /* 6 */
    {0x00,0x01,0x01,0xE1,0xFD,0x1F,0x03,0x00},  /* 7 */
//  {0x00,0x76,0xFF,0x89,0x89,0xFF,0x76,0x00},  /* 8 */
//  {0x00,0x4E,0xDF,0x91,0x91,0xFF,0x7E,0x00},  /* 9 */
    {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}   /* nothing, */
};

uint32_t const DIGIT_HEX[16] = {DIGIT_0, DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4, DIGIT_5, DIGIT_6, DIGIT_7, DIGIT_8, DIGIT_9, DIGIT_A, DIGIT_B, DIGIT_C, DIGIT_D, DIGIT_E, DIGIT_F};

/** *******************************************************************
 * @brief       LED Interrupt call back
 * @param[in]   
 * @return
 *
 *********************************************************************/
void app_led_irq_handler(uint32_t event, void *context)
{
    uint8_t addrIndx;

    if (event & LED_EVENT_DONE)
    {
        LED->SR &= ~(1<<2);
        t_sysbit.led_end = 1;
        if ((t_sysbit.cal_ok == 1) && (t_sysbit.time_div != 0))
        {
            if((ts.flag.mesh_shld_en == 1) || (ts.flag.ch_shld_en == 1))
            {
                TS(SFR_SHLD_CON) |= BIT(24);    
            }

            if((ts.actv_ch_idx_ref == 1) && (ts.flag.ref_ch_en > 0))   
            {            
                ts.actv_ch_idx_ref = 1;
                addrIndx = ts.ch_num_buf_ref;
                TS(SFR_TS_CH_SEL) = 0;
                TS(SFR_TS_CH_SEL) |= BIT(addrIndx);             
                TS(SFR_TS_SUM_CNT) = ts.touch_sum_count_ref;
                TS(SFR_TS_SLP_CON) = ts.rc_filter_buf_ref;
                
                /* touch sensing */
                SCU->TIRCCON = 1;
                TS(SFR_TS_CON) = 0;
                NVIC_ClearPendingIRQ(TOUCH_IRQn);   
                NVIC_EnableIRQ(TOUCH_IRQn);
                
                TS(SFR_TS_CON) = _TS_START;
            }
            else
            {
                Touch_Start_Touch(ts.actv_ch_idx);
            }

        }
    }
}

/** *******************************************************************
 * @brief       LED display data update
 * @param[in]   
 * @return
 *
 *********************************************************************/
void led_RenewData(void)
{
    uint16_t seg_data[13];      /* COM0 ~ COM12 */
    uint32_t* ptrDisp;

    uint8_t comIndx;
    uint8_t numIndx;
    uint8_t i;

    /***********  clear led data  *************/
    for (comIndx = 0; comIndx < 13; comIndx++)
    {
        seg_data[comIndx] = 0;
    }

    /***********  M-TYPE 8x8 : 0~C표시 ************
        SEG : 6/9/10/11/12/13/14/15
        COM : 0/1/2/3/4/5/7/8
    **********************************************/
    for (numIndx = 0; numIndx < 8; numIndx++)
    {
        if (ts.detect_key & BIT(numIndx))      
        {
            break;
        }
    }   
    /* T-Type */
    for (comIndx = 0; comIndx < 8; comIndx++)
    {
        for (i = 0; i < numIndx+1; i++)
        {
            if (i < 6)      /* 0,1,2,3,4,5 */
            {
                seg_data[comIndx] |= BIT(i);
            }
            else            /* 7,8 */
            {
                seg_data[comIndx] |= BIT(i+1);
            }
        }
    }
    /* M-Type */
    for (comIndx = 0; comIndx < 8; comIndx++)   /* COM0/1/2/3/4/5/7/8 */    
    {
        if (coDigit[numIndx][comIndx] & BIT(0))
        {
            seg_data[comIndx] |= BIT(6);        /* SEG6 */
        }
        seg_data[comIndx] |= (uint16_t)(coDigit[numIndx][comIndx]<<8)&0xfe00;   /* SEG9/10/11/12/13/14/15 */
    }

    /***********  renew led data  *************/
    ptrDisp = (uint32_t*)(&LED->DISPRAM0);  
    for (comIndx = 0; comIndx < 8; comIndx++)
    {
        if (comIndx < 6)    /* COM0 ~ COM5 */
        {
            *(ptrDisp+comIndx) = seg_data[comIndx];
        }
        else                /* COM7 ~ COM8 */
        {
            *(ptrDisp+(comIndx+1)) = seg_data[comIndx];
        }
    }   
}


/** *******************************************************************
 * @brief       Set the LED operation duration
 * @param[in]   com_cnt : number of COMs to use
                duration : LED operation section in milli second.
 * @return      void
 *
 *********************************************************************/

void LED_Set_Actv_Time(uint8_t com_cnt, uint8_t slot_ms, uint8_t period)
{
    uint32_t temp;
    struct led_cfg ledcfg = APP_LED_CONFIG;

    HAL_LED_Init(&ledcfg, app_led_irq_handler, NULL);

    /* *************************************************
    slotMs = (1/16M)*(PRESD+1)*(COMPWID+1)*COM_N*1000
    slotMs*16000/((PRESD+1)*COM_N)-1 = COMPWID
    if PRESD = 127, then (16000>>7) = 125;
    ************************************************* */
#if MCLK_SPEED == MCLK_16MHZ
    LED->PRESD      = 127;      
    LED->LEDPD      = (uint32_t)(period * 16000);
#endif  
#if MCLK_SPEED == MCLK_32MHZ
    LED->PRESD      = 255;      
    LED->LEDPD      = (uint32_t)(period * 32000); 
#endif

    temp = (((uint32_t)slot_ms*125/(uint32_t)com_cnt)-1);   
    if(temp > 0xff)
    {
        LED->COMPWID = 0xff;
    }
    else
    {
        LED->COMPWID = (uint8_t)temp; 
    }

    if (t_sysbit.time_div != 0)
    {
        led.flag.op_mode = 2;
    }
    else
    {
        led.flag.op_mode = 0;
    }   
    t_sysbit.led_en = 1;
}


/** *******************************************************************
 * @brief       Set the LED seg current.
 * @param[in]   current_indx : define the current amount of the SEG-pads.
 * @return      void
 *
 *********************************************************************/
void LED_Set_Current(uint8_t currLev)
{
    /*----------------------------
    0 : 8.47 mA
    1 : 13.61 mA
    2 : 22.09 mA
    3 : 26.20 mA
    ----------------------------*/
    if (currLev > 3)
    {
        led.flag.seg_current = 3;
    }
    else
    {
        led.flag.seg_current = currLev;
    }
}

/** *******************************************************************
 * @brief       When the LED is in the inactive state, it sets the SEG/
                COM I/O state.
 * @param[in]   none
 * @return      void
 *
 *********************************************************************/
void LED_Init_IO(void)
{
    /* ****************************************************************************
        SEG00-PE7, SEG01-PE6, SEG02-PE5, SEG03-PE4, SEG04-PE3, SEG05-PE2,SEG06-PE1, 
        SEG07-PE0, SEG08-PD5, SEG09-PD4, SEG10-PD3, SEG11-PD2, SEG12-PD1,SEG13-PD0
        SEG14-PC4, SEG15-PC3
    ******************************************************************************/
#if (0) /* Hisense Mode 에서 floating */
    if (led.seg_sel & BIT(0))   /* PE7 */
    {
        HAL_GPIO_ClearPin(PORTE, PIN7);
    }
    if (led.seg_sel & BIT(1))   /* PE6 */
    {
        HAL_GPIO_ClearPin(PORTE, PIN6);
    }
    if (led.seg_sel & BIT(2))   /* PE5 */
    {
        HAL_GPIO_ClearPin(PORTE, PIN5);
    }
    if (led.seg_sel & BIT(3))   /* PE4 */
    {
        HAL_GPIO_ClearPin(PORTE, PIN4);
    }
    if (led.seg_sel & BIT(4))   /* PE3 */
    {
        HAL_GPIO_ClearPin(PORTE, PIN3);
    }
    if (led.seg_sel & BIT(5))   /* PE2 */
    {
        HAL_GPIO_ClearPin(PORTE, PIN2);
    }
    if (led.seg_sel & BIT(6))   /* PE1 */
    {
        HAL_GPIO_ClearPin(PORTE, PIN1);
    }
    if (led.seg_sel & BIT(7))   /* PE0 */
    {
        HAL_GPIO_ClearPin(PORTE, PIN0);
    }
    if (led.seg_sel & BIT(8))   /* PD5 */
    {
        HAL_GPIO_ClearPin(PORTD, PIN5);
    }
    if (led.seg_sel & BIT(9))   /* PD4 */
    {
        HAL_GPIO_ClearPin(PORTD, PIN4);
    }   
    if (led.seg_sel & BIT(10))  /* PD3 */
    {
        HAL_GPIO_ClearPin(PORTD, PIN3);
    }
    if (led.seg_sel & BIT(11))  /* PD2 */
    {
        HAL_GPIO_ClearPin(PORTD, PIN2);
    }
    if (led.seg_sel & BIT(12))  /* PD1 */
    {
        HAL_GPIO_ClearPin(PORTD, PIN1);
    }
    if (led.seg_sel & BIT(13))  /* PD0 */
    {   
        HAL_GPIO_ClearPin(PORTD, PIN0);
    }
    if (led.seg_sel & BIT(14))  /* PC4 */
    {
        HAL_GPIO_ClearPin(PORTC, PIN4);
    }
    if (led.seg_sel & BIT(15))  /* PC3 */
    {
        HAL_GPIO_ClearPin(PORTC, PIN3);
    }
#endif  
    /* ****************************************************************************
        COM00-PE7, COM01-PE6, COM02-PE5, COM03-PE4, COM04-PE3, COM05-PE2,COM06-PE1, 
        COM07-PE0, COM08-PD5, COM09-PD4, COM10-PD3, COM11-PD2, COM12-PD1
    ******************************************************************************/
    if (led.com_sel & BIT(0))   /* PE7 */
    {
        HAL_GPIO_SetPin(PORTE, PIN7);
    }
    if (led.com_sel & BIT(1))   /* PE6 */
    {
        HAL_GPIO_SetPin(PORTE, PIN6);
    }
    if (led.com_sel & BIT(2))   /* PE5 */
    {
        HAL_GPIO_SetPin(PORTE, PIN5);
    }
    if (led.com_sel & BIT(3))   /* PE4 */
    {
        HAL_GPIO_SetPin(PORTE, PIN4);
    }
    if (led.com_sel & BIT(4))   /* PE3 */
    {
        HAL_GPIO_SetPin(PORTE, PIN3);
    }
    if (led.com_sel & BIT(5))   /* PE2 */
    {
        HAL_GPIO_SetPin(PORTE, PIN2);
    }
    if (led.com_sel & BIT(6))   /* PE1 */
    {
        HAL_GPIO_SetPin(PORTE, PIN1);
    }
    if (led.com_sel & BIT(7))   /* PE0 */
    {
        HAL_GPIO_SetPin(PORTE, PIN0);
    }
    if (led.com_sel & BIT(8))   /* PD5 */
    {
        HAL_GPIO_SetPin(PORTD, PIN5);
    }
    if (led.com_sel & BIT(9))   /* PD4 */
    {
        HAL_GPIO_SetPin(PORTD, PIN4);
    }
    if (led.com_sel & BIT(10))  /* PD3 */
    {
        HAL_GPIO_SetPin(PORTD, PIN3);
    }
    if (led.com_sel & BIT(11))  /* PD2 */
    {
        HAL_GPIO_SetPin(PORTD, PIN2);
    }
    if (led.com_sel & BIT(12))  /* PD1 */
    {
        HAL_GPIO_SetPin(PORTD, PIN1);
    }

    t_sysbit.led_io = 1;
}
//---------------------------------------------------------------------------------------------------

/** *******************************************************************
 * @brief       Set alternative function for led
 * @param[in]   
 * @return      
 *
 *********************************************************************/
void LED_Init_Reg(void) 
{
#if ((COM00_EN == 1) || (SEG00_EN == 1))
    {
        HAL_GPIO_SetFunction(PORTE, PIN7, Pn_MUX_AF5);
        HAL_GPIO_SetMode(PORTE, PIN7, GPIO_MODE_FUNC);          
    }
#endif
#if ((COM01_EN == 1) || (SEG01_EN == 1))
    {
        HAL_GPIO_SetFunction(PORTE, PIN6, Pn_MUX_AF5);
        HAL_GPIO_SetMode(PORTE, PIN6, GPIO_MODE_FUNC);  
    }
#endif
#if ((COM02_EN == 1) || (SEG02_EN == 1))
    {
        HAL_GPIO_SetFunction(PORTE, PIN5, Pn_MUX_AF5);
        HAL_GPIO_SetMode(PORTE, PIN5, GPIO_MODE_FUNC);  
    }
#endif
#if ((COM03_EN == 1) || (SEG03_EN == 1))
    {
        HAL_GPIO_SetFunction(PORTE, PIN4, Pn_MUX_AF5);
        HAL_GPIO_SetMode(PORTE, PIN4, GPIO_MODE_FUNC);  
    }
#endif
#if ((COM04_EN == 1) || (SEG04_EN == 1))
    {
        HAL_GPIO_SetFunction(PORTE, PIN3, Pn_MUX_AF5);
        HAL_GPIO_SetMode(PORTE, PIN3, GPIO_MODE_FUNC);  
    }
#endif
#if ((COM05_EN == 1) || (SEG05_EN == 1))
    {
        HAL_GPIO_SetFunction(PORTE, PIN2, Pn_MUX_AF5);
        HAL_GPIO_SetMode(PORTE, PIN2, GPIO_MODE_FUNC);  
    }
#endif
#if ((COM06_EN == 1) || (SEG06_EN == 1))
    {
        HAL_GPIO_SetFunction(PORTE, PIN1, Pn_MUX_AF5);
        HAL_GPIO_SetMode(PORTE, PIN1, GPIO_MODE_FUNC);  
    }
#endif
#if ((COM07_EN == 1) || (SEG07_EN == 1))
    {
        HAL_GPIO_SetFunction(PORTE, PIN0, Pn_MUX_AF5);
        HAL_GPIO_SetMode(PORTE, PIN0, GPIO_MODE_FUNC);  
    }
#endif
#if ((COM08_EN == 1) || (SEG08_EN == 1))
    {
        HAL_GPIO_SetFunction(PORTD, PIN5, Pn_MUX_AF5);
        HAL_GPIO_SetMode(PORTD, PIN5, GPIO_MODE_FUNC);  
    }
#endif
#if ((COM09_EN == 1) || (SEG09_EN == 1))
    {
        HAL_GPIO_SetFunction(PORTD, PIN4, Pn_MUX_AF5);
        HAL_GPIO_SetMode(PORTD, PIN4, GPIO_MODE_FUNC);  
    }
#endif
#if ((COM10_EN == 1) || (SEG10_EN == 1))
    {
        HAL_GPIO_SetFunction(PORTD, PIN3, Pn_MUX_AF5);
        HAL_GPIO_SetMode(PORTD, PIN3, GPIO_MODE_FUNC);  
    }
#endif
#if ((COM11_EN == 1) || (SEG11_EN == 1))
    {
        HAL_GPIO_SetFunction(PORTD, PIN2, Pn_MUX_AF5);
        HAL_GPIO_SetMode(PORTD, PIN2, GPIO_MODE_FUNC);  
    }
#endif
#if ((COM12_EN == 1) || (SEG12_EN == 1))
    {
        HAL_GPIO_SetFunction(PORTD, PIN1, Pn_MUX_AF5);
        HAL_GPIO_SetMode(PORTD, PIN1, GPIO_MODE_FUNC);  
    }
#endif
#if (SEG13_EN == 1)
    {
        HAL_GPIO_SetFunction(PORTD, PIN0, Pn_MUX_AF5);
        HAL_GPIO_SetMode(PORTD, PIN0, GPIO_MODE_FUNC);  
    }
#endif
#if (SEG14_EN == 1)
    {
        HAL_GPIO_SetFunction(PORTC, PIN4, Pn_MUX_AF5);
        HAL_GPIO_SetMode(PORTC, PIN4, GPIO_MODE_FUNC);  
    }
#endif
#if (SEG15_EN == 1)
    {
        HAL_GPIO_SetFunction(PORTC, PIN3, Pn_MUX_AF5);
        HAL_GPIO_SetMode(PORTC, PIN3, GPIO_MODE_FUNC);  
    }
#endif

    LED->LEDCON1 =  (led.flag.op_mode<<2) |     /* 0:Auto, 1:HandShake, 2:StopCount, 3:SmartShare, 4:Alone          */
                    (1<<1) |                    /* Enable                                                           */
                    (0<<0);                     /* Start                                                            */
    LED->LEDCON2 =  (0<<4) |                    /* 0:SEG floating, 1: SEG LOW                                       */
                    (1<<3) |                    /* 1: overlap time = 1/8MHz Fix. 125ns                              */
                    (7<<0);                     /* overlap time, 0:64us, 1:32, 2:24, 3:21, 4:12, 5:9, 6:6, 7:3us    */
    LED->LEDCON3 =  (5<<4) |                    /* sst time, 0:1us, 1:2, 2:4, 3:6, ... 15:32us          */
                    (5<<0);                     /* clear time, 0:1us, 1:2, 2:4, 3:6, ... 15:32us        */

    //LED->COMDRIVE     /* LED Current : GPIO N-MOS PAD + */

    LED->SR = 0;
    LED->SR |= (1<<1);
    LED->COMOE  = led.com_sel;
    LED->SEGOE = led.seg_sel;
    LED->COMER = led.com_sel; 

    LED->COMDIMM0 = 0;
    LED->COMDIMM1 = 0;
    LED->COMDIMM2 = 0;
    LED->COMDIMM3 = 0;

    LED->DISPRAM0 = LED_INIT_DATA;
    LED->DISPRAM1 = LED_INIT_DATA;
    LED->DISPRAM2 = LED_INIT_DATA;
    LED->DISPRAM3 = LED_INIT_DATA;
    LED->DISPRAM4 = LED_INIT_DATA;
    LED->DISPRAM5 = LED_INIT_DATA;
    LED->DISPRAM6 = LED_INIT_DATA;
    LED->DISPRAM7 = LED_INIT_DATA;
    LED->DISPRAM8 = LED_INIT_DATA;
    LED->DISPRAM9 = LED_INIT_DATA;
    LED->DISPRAM10 = LED_INIT_DATA;
    LED->DISPRAM11 = LED_INIT_DATA; 
    LED->DISPRAM12 = LED_INIT_DATA;

    LED->PORTCTRL = 0x0000; /* 0: floating, 1: Output */

    LED_CCSTRIM = led.flag.seg_current; /* ### GUN */

    NVIC_SetPriority(LED_IRQn, 2);
    NVIC_EnableIRQ(LED_IRQn);
    __enable_irq();
    
    LED->LEDCON1 |= BIT(0);     /* LED Start ! */
}


/** *******************************************************************
 * @brief       led process
 * @param[in]   
 * @return      
 *
 *********************************************************************/
void LED_Do_Task(void)
{
    static uint8_t state = 0;

    if (t_sysbit.led_en == 0)
    {
        return;
    }
    switch (state)
    {
        case LED_STATE_INIT_IO:
            LED_Init_IO();
            state = LED_STATE_INIT_REG;
            break;
        case LED_STATE_INIT_REG:
            if (t_sysbit.cal_ok == 1)
            {
                LED_Init_Reg();
                state = LED_STATE_LED_RENEW;    
            }
            break;
        case LED_STATE_LED_RENEW:
            fpRenewLED();
            break;
        default:
            state = LED_STATE_INIT_IO;
            break;
    }
}

#endif
