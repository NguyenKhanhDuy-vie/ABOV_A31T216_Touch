/**
 *******************************************************************************
 * @file        user_function.c
 * @author      ABOV R&D Division
 * @brief       A31T21x Timer Code for Application
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

#include "common.h"
#include "touch_lib.h"
#include "user_timer.h"
#include "user_function.h"
#if (UL60730_SELF_TEST_EN == 1)
#include "ul60730_lib_common_V01.h"
#endif

static uint16_t ut_set_flag;
static uint16_t ut_expire_flag;
static uint32_t ut_init_delay_timer;
static uint32_t ut_reverse_timer;
static uint32_t ut_base_update_timer;
static uint32_t ut_hold_key_timer;
#if (SLIDE_FUNCTION_EN == 1)
static uint32_t ut_slide_event_hold_timer;
#endif
#if (WHEEL_FUNCTION_EN == 1)
static uint32_t ut_wheel_event_hold_timer;
#endif

void UT_Run_Timer(void)
{
    ut_init_delay_timer = 0;
    ut_reverse_timer = 0;
    ut_base_update_timer = 0;
    ut_hold_key_timer = 0;
#if (SLIDE_FUNCTION_EN == 1)
    ut_slide_event_hold_timer = 0;
#endif
#if (WHEEL_FUNCTION_EN == 1)
    ut_wheel_event_hold_timer = 0;
#endif

    ut_set_flag = 0;
    ut_expire_flag = 0;
}

void SysTick_Handler(void)
{
#if (UL60730_SELF_TEST_EN == 1)
    if(UL60730_g.intr_post_en || UL60730_g.intr_bist_en)
    {
        UL60730_SYSTICK_ISR();
    }
    else
#endif
    {
        /* User Code */
        if(ut_init_delay_timer)
            ut_init_delay_timer--;
        if(ut_reverse_timer)
            ut_reverse_timer--;
        if(ut_base_update_timer)
            ut_base_update_timer--;
        if(ut_hold_key_timer)
            ut_hold_key_timer--;
#if (SLIDE_FUNCTION_EN == 1)
        if(ut_slide_event_hold_timer)
            ut_slide_event_hold_timer--;
#endif
#if (WHEEL_FUNCTION_EN == 1)
        if(ut_wheel_event_hold_timer)
            ut_wheel_event_hold_timer--;
#endif
    }
}

/** *******************************************************************
 * @brief       set user-timer
 * @param[in]   utid : user-timer id
                timeout : timeout in milli second.
 * @return      void
 *
 *********************************************************************/
void UT_Set_Timer(uint16_t utid, uint32_t timeout)
{

    switch (utid)
    {   
        case UT_INIT_DELAY : 
            ut_init_delay_timer = timeout;
            break;
        case UT_REVERSE : 
            ut_reverse_timer = timeout;
            break;
        case UT_BASE_UPDATE: 
            ut_base_update_timer = timeout;
            break;  
        case UT_HOLD_KEY_RELEASE: 
            ut_hold_key_timer = timeout;
            break;
#if (SLIDE_FUNCTION_EN == 1)        
        case UT_SLIDE_EVENT_HOLD: 
            ut_slide_event_hold_timer = timeout;
            break;
#endif
#if (WHEEL_FUNCTION_EN == 1)
        case UT_WHEEL_EVENT_HOLD: 
            ut_wheel_event_hold_timer = timeout;
            break;
#endif
    }
    /* set timer-set flag and clear time-out flag */
    if(utid)
    {
        ut_set_flag |= utid;
        ut_expire_flag &= ~utid;
    }
}

#if (1)
/** *******************************************************************
 * @brief       get user-timer set infomation
 * @param[in]   none
 * @return      ut_set_flag
 *
 *********************************************************************/
uint16_t UT_Get_Status(void)
{
    return ut_set_flag;
}
#endif


/** *******************************************************************
 * @brief       clear user-timer set information & count
 * @param[in]   utid
 * @return      void
 *
 *********************************************************************/
void UT_Clear_Timer(uint16_t utid)
{
    /* clear user-timer */
    switch (utid)
    {       
        case UT_INIT_DELAY : 
            ut_init_delay_timer = 0;
            break;  
        case UT_REVERSE : 
            ut_reverse_timer = 0;
            break;      
        case UT_BASE_UPDATE : 
            ut_base_update_timer = 0;
            break;      
        case UT_HOLD_KEY_RELEASE : 
            ut_hold_key_timer = 0;
            break;
#if (SLIDE_FUNCTION_EN == 1)
        case UT_SLIDE_EVENT_HOLD : 
            ut_slide_event_hold_timer = 0;
            break;  
#endif
#if (WHEEL_FUNCTION_EN == 1)
        case UT_WHEEL_EVENT_HOLD : 
            ut_wheel_event_hold_timer = 0;
            break;
#endif
    }

    /* clear timer-set flag and time-out flag */
    if (utid)
    {
        ut_set_flag &= ~utid;
        ut_expire_flag &= ~utid;
    }
}


/** *******************************************************************
 * @brief       update user-timer expiration flag
 * @param[in]   none
 * @return      void
 *
 *********************************************************************/
static void UT_Check_Expire(void)
{
    uint16_t wFlag = 0;

    if (ut_set_flag)
    {
        if (ut_set_flag & UT_INIT_DELAY)
        {
            if(ut_init_delay_timer == 0)
            {
                wFlag |= UT_INIT_DELAY;
            }
        }
        if (ut_set_flag & UT_REVERSE)
        {
            if(ut_reverse_timer == 0)
            {
                wFlag |= UT_REVERSE;
            }
        }
        if (ut_set_flag & UT_BASE_UPDATE)
        {
            if(ut_base_update_timer == 0)
            {
                wFlag |= UT_BASE_UPDATE;
            }
        }
        if (ut_set_flag & UT_HOLD_KEY_RELEASE)
        {
            if(ut_hold_key_timer == 0)
            {
                wFlag |= UT_HOLD_KEY_RELEASE;
            }
        }
#if (SLIDE_FUNCTION_EN == 1)
        if (ut_set_flag & UT_SLIDE_EVENT_HOLD)
        {
            if(ut_slide_event_hold_timer == 0)
            {
                wFlag |= UT_SLIDE_EVENT_HOLD;
            }
        }
#endif
#if (WHEEL_FUNCTION_EN == 1)
        if (ut_set_flag & UT_WHEEL_EVENT_HOLD)
        {
            if(ut_wheel_event_hold_timer == 0)
            {
                wFlag |= UT_WHEEL_EVENT_HOLD;
            }
        }
#endif

        /* user-timer-out-flag set */
        if (wFlag)
        {
            ut_expire_flag |= wFlag;
        }
    }
}


/** *******************************************************************
 * @brief       update user-timer expiration information
 * @param[in]   utid : user-timer id
 * @return      rtn : if a user-timer is expired, return 1
                      else, return 0
 *
 *********************************************************************/
uint8_t UT_Is_Expired(uint16_t utid)
{
    uint8_t rtn;

    UT_Check_Expire();

    rtn = 0;
    /* if time-out flag is setted */
    if (ut_expire_flag)
    {
        if (utid)
        {
            if ((ut_expire_flag & utid) == utid)
            {
                rtn = 1;
                UT_Clear_Timer(utid);           
            }
        }
    }
    
    return(rtn);
}


