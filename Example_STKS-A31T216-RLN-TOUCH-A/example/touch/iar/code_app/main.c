/**
 *******************************************************************************
 * @file        main.c
 * @author      ABOV R&D Division
 * @brief       A31T21x STKS Board Aplication Main Code
 *
 * Copyright 2017 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Change_SysClock(enum system_clock sysclk);
/* Private variables ---------------------------------------------------------*/
str_flag t_sysbit;


/**********************************************************************
 * @brief       SysTick_Configure
 * @param[in]   None
 * @return      None
 *
 **********************************************************************/
void SysTick_Configure(void)
{
    /* 1_msec interrupt */
    SysTick_Config(SystemCoreClock/1000);
}


void Change_SysClock(enum system_clock sysclk)
{
    struct scu_clock_cfg cfg;

    switch (sysclk)
    {
        case SYS_CLK_32MHZ :
            cfg.mclk_sel = SCU_CLOCK_HSI;
            cfg.mclk_div = SCU_CLOCK_DIV_1;
            cfg.mclk_hz = 32000000;
            break;
        case SYS_CLK_16MHZ :
            cfg.mclk_sel = SCU_CLOCK_HSI;
            cfg.mclk_div = SCU_CLOCK_DIV_2;
            cfg.mclk_hz = 16000000;
            break;
        case SYS_CLK_8MHZ :
            cfg.mclk_sel = SCU_CLOCK_HSI;
            cfg.mclk_div = SCU_CLOCK_DIV_4;
            cfg.mclk_hz = 8000000;
            break;
        case SYS_CLK_4MHZ :
            cfg.mclk_sel = SCU_CLOCK_HSI;
            cfg.mclk_div = SCU_CLOCK_DIV_8;
            cfg.mclk_hz = 4000000;
            break;
        case SYS_CLK_2MHZ :
            cfg.mclk_sel = SCU_CLOCK_HSI;
            cfg.mclk_div = SCU_CLOCK_DIV_16;
            cfg.mclk_hz = 2000000;
            break;
        case SYS_CLK_1MHZ :
            cfg.mclk_sel = SCU_CLOCK_HSI;
            cfg.mclk_div = SCU_CLOCK_DIV_32;
            cfg.mclk_hz = 1000000;
            break;
    }
    /* Update Main Clock */
    HAL_SCU_UpdateMainClock(&cfg);
}


/** *******************************************************************
 * @brief       initial for user function
 * @param[in]   
 * @return      
 *
 *********************************************************************/
void Init_User_Function(void)
{   
#if ((BIG_DIFF_CANCEL_EN | CHANNEL_GROUPING_EN) == 1)
    uint8_t ch;
#endif

#if (I2C_DEBUG_EN == 1)
    Port_Configure_I2C(I2C_ID_1);
    HAL_I2C_Init(I2C_ID_1);
    user.dataBuff = (uint16_t*)&RegMap[REG_DEBUG_10];
    RegMap[REG_LIB_VERSION] = ts.touch_lib_ver;
    RegMap[REG_MODEL_NO] = 0x01;
    RegMap[REG_VENDOR_ID] = 0xAB;
#endif

#if (UART_DEBUG_EN == 1)
    DBG_Set_Baudrate(UART_BAUDRATE);
#endif

#if (USER_FILTER_ONESENSING_EN == 1)
    func_p_user_filter_onesensingdata = TS_User_Filter_onesensingdata;
#endif

#if (USER_FILTER_DIFFDATA_EN == 1)
    func_p_user_filter_diffdata = TS_User_Filter_diffdata;
#endif
    
#if (TOUCH_FREQ_SEL_EN == 1)
    func_p_Touch_Freq_Sel = Touch_Freq_Sel;
#endif
    
#if (FIRST_KEY_PRIORITY_EN == 1)
    user.priority_ch_idx = PRIORITY_CH_BIT;
#endif

#if (CHANNEL_GROUPING_EN == 1)
    for(ch = 0; ch < ts.actv_ch_cnt; ch++)
    {
        user.ch_group_set[ch] = key_group_id[ch];
    }
#endif

#if (HOLD_KEY_RELEASE_EN == 1)
    user.hold_key_release_time = TIME_HOLD_KEY_RELEASE;
#endif
            
#if (MULTI_KEY_RESET_EN == 1)          
    user.multi_key_check = 0;
    user.multi_ch_bit = MULTY_CH_BIT;
    user.multi_key_limit = MAX_MULTY_KEY_NUM;    
    if(user.multi_key_limit < 1)    
    {
        user.multi_key_limit = 1;
    }
#endif

#if (BIG_DIFF_CANCEL_EN == 1)
    for(ch = 0; ch < ts.actv_ch_cnt; ch++)
    {
        user.big_diff_thd[ch] = big_diff_thd[ch];
        if(user.big_diff_thd[ch] > 0x7FFF)
        {
            user.big_diff_thd[ch] = 0x7FFF;
        }
    }    
#endif

#if (SLIDE_FUNCTION_EN == 1)
    Slide_ch_set();
#endif
        
#if (WHEEL_FUNCTION_EN == 1)
    Wheel_ch_set();
#endif

#if (LED_DRV_EN == 1)
    /* LED Block Set */    
    SCU->PER2 &= ~(1<<29);
    SCU->PCER2 &= ~(1<<29);
    SCU->PER2 |= (1<<29);
    SCU->PCER2 |= (1<<29);

    fpRenewLED = led_RenewData;
    led.flag.seg_current = 1;
    led.flag.op_mode = 2;
    led.com_sel = COMSEL_00_12;
    led.seg_sel = SEGSEL_00_15;

    LED_Set_Current(LED_CURRENT_13mA);    
    LED_Set_Actv_Time(LED_USE_COM_NUM, TIME_LED_OPTERATING, TIME_LED_INTERVAL);
#endif
}


/** *******************************************************************
 * @brief       main function
 * @param[in]   none
 * @return      void
 *
 *********************************************************************/
int main (void)
{
    e_task task;

#if (UL60730_SELF_TEST_EN == 1)
    uint32_t ul_test_func;
    ul_test_func = USE_UL60730_REGISTER_BIST;
#endif

#if (WDT_EN == 1)
    struct wdt_cfg cfg = APP_WDT_CONFIG;
    HAL_WDT_Init(&cfg, NULL, NULL);
    HAL_WDT_Start(7812*WDT_RESET_TIME_RATE);    /* 7812 = 1s */
#endif

    Port_Init();                                /* Initialize all port */
    System_Setting_Config();                    /* Configure system settings */

#if (MCLK_SPEED == MCLK_16MHZ)
    Change_SysClock(SYS_CLK_16MHZ);
#endif
#if (MCLK_SPEED == MCLK_32MHZ)
    Change_SysClock(SYS_CLK_32MHZ);
#endif
   
    SysTick_Config(SystemCoreClock/1000);       /* 1ms tick */  

    Init_Touch();  
    Init_User_Function();

#if (UL60730_SELF_TEST_EN == 1)
    user.ul60730_post_result = UL60730_selfTest_POST(POST_TEST_FUNCTION,ts.actv_ch_bit,0);    
#endif
   
    __enable_irq();                             /* Global Interrupt Enable */
    
    task = TASK_LED;
    
    while(1)
    {
        switch(task)
        {
            case TASK_LED :
                #if (LED_DRV_EN == 1)
                LED_Do_Task();
                #endif
                break;          
                
            case TASK_TOUCH :
                Touch_Do_Task();
                #if ((HOLD_KEY_RELEASE_EN | MULTI_KEY_RESET_EN | FIRST_KEY_PRIORITY_EN | BIG_DIFF_CANCEL_EN | CHANNEL_GROUPING_EN ) == 1)
                if (ts.flag.touch_sensing_end == 1)
                {
                    Touch_Key_Scenario();
                    ts.flag.touch_sensing_end = 0;
                }
                #endif
                break;          

            case TASK_GESTURE :
                #if ((SLIDE_FUNCTION_EN | WHEEL_FUNCTION_EN) == 1)
                GESTURE_Do_Task(&g_diff_data[0]);
                #endif
                break;
                
            case TASK_DEBUG :
                #if (UART_DEBUG_EN == 1)
                DBG_Do_Task();
                #endif
                #if (I2C_DEBUG_EN == 1)
                DBG_Do_Task_I2C();
                #endif
                break;          
                
            default :
                break;
        }

        task++; 
        if(task == TASK_MAX)
        {
            task = TASK_LED;
        }

#if (UL60730_SELF_TEST_EN == 1)
        if(ts.flag.ts_status == TOUCH_COMPLETE)
        {
            if( (ul_test_func & POST_TEST_FUNCTION) && (ul_test_func == USE_UL60730_TOUCH_IP_BIST) 
             || (ul_test_func & POST_TEST_FUNCTION) && (ul_test_func == USE_UL60730_TOUCH_CH_BIST) 
             || (ul_test_func & POST_TEST_FUNCTION) && (ul_test_func == USE_UL60730_TOUCH_REG_BIST) 
            )       
            {
                if( (t_sysbit.led_en == 1) && (t_sysbit.time_div == 1) )
                {
                    LED->LEDCON1 &= (~0x00000001);
                }
                TS(SFR_TS_CON) = 0;
            }

            user.ul60730_bist_result = UL60730_selfTest_BIST(ul_test_func, ts.actv_ch_bit, 0);    

            if( (ul_test_func & POST_TEST_FUNCTION) && (ul_test_func == USE_UL60730_TOUCH_IP_BIST) 
             || (ul_test_func & POST_TEST_FUNCTION) && (ul_test_func == USE_UL60730_TOUCH_CH_BIST) 
             || (ul_test_func & POST_TEST_FUNCTION) && (ul_test_func == USE_UL60730_TOUCH_REG_BIST) 
            )       
            {
                if( (t_sysbit.led_en == 1) && (t_sysbit.time_div == 1) )
                {
                    LED->LEDCON1 |= BIT(0);
                }
            }

            ul_test_func = (ul_test_func << 1);
                
            if(ul_test_func > USE_UL60730_TOUCH_REG_BIST){
                ul_test_func = USE_UL60730_REGISTER_BIST;
            }            
        }
#endif
            
#if (WDT_EN == 1)
        HAL_WDT_Feed();         /* WDT Clear */
#endif   
    }
}
