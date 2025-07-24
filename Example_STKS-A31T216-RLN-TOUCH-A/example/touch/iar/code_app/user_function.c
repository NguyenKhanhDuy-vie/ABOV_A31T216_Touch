/**
 *******************************************************************************
 * @file        user_function.c
 * @author      ABOV R&D Division
 * @brief       A31T21x User Function Code
 *
 * Copyright 2017 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#include <stdlib.h>
#include "A31T21x.h"
#include "a31t21x_driver_common.h"

#include "common.h"
#include "touch_lib.h"
#include "user_timer.h"
#include "user_function.h"
#include "register_map.h"
#if (UL60730_SELF_TEST_EN == 1)
#include "ul60730_lib_common_V01.h"
#endif


#if (BIG_DIFF_CANCEL_EN == 1)
uint16_t big_diff_thd[TS_MAX_KEY] = {
    BIG_DIFF_THD_CH00,
    BIG_DIFF_THD_CH01,
    BIG_DIFF_THD_CH02,
    BIG_DIFF_THD_CH03,
    BIG_DIFF_THD_CH04,
    BIG_DIFF_THD_CH05,
    BIG_DIFF_THD_CH06,
    BIG_DIFF_THD_CH07,
    BIG_DIFF_THD_CH08,
    BIG_DIFF_THD_CH09,
    BIG_DIFF_THD_CH10,
    BIG_DIFF_THD_CH11,
    BIG_DIFF_THD_CH12,
    BIG_DIFF_THD_CH13,
    BIG_DIFF_THD_CH14,
    BIG_DIFF_THD_CH15,
    BIG_DIFF_THD_CH16,
    BIG_DIFF_THD_CH17,
    BIG_DIFF_THD_CH18,
    BIG_DIFF_THD_CH19,
    BIG_DIFF_THD_CH20,
    BIG_DIFF_THD_CH21,
    BIG_DIFF_THD_CH22,
    BIG_DIFF_THD_CH23,
};    
#endif

#if (CHANNEL_GROUPING_EN == 1)
uint8_t key_group_id[TS_MAX_KEY] = {
    CH00_GROUP_ID,
    CH01_GROUP_ID,
    CH02_GROUP_ID,
    CH03_GROUP_ID,
    CH04_GROUP_ID,
    CH05_GROUP_ID,
    CH06_GROUP_ID,
    CH07_GROUP_ID,
    CH08_GROUP_ID,
    CH09_GROUP_ID,
    CH10_GROUP_ID,
    CH11_GROUP_ID,
    CH12_GROUP_ID,
    CH13_GROUP_ID,
    CH14_GROUP_ID,
    CH15_GROUP_ID,
    CH16_GROUP_ID,
    CH17_GROUP_ID,
    CH18_GROUP_ID,
    CH19_GROUP_ID,
    CH20_GROUP_ID,
    CH21_GROUP_ID,
    CH22_GROUP_ID,
    CH23_GROUP_ID,
 };
 #endif

#if ((HOLD_KEY_RELEASE_EN | MULTI_KEY_RESET_EN | FIRST_KEY_PRIORITY_EN | BIG_DIFF_CANCEL_EN | CHANNEL_GROUPING_EN | USER_FILTER_DIFFDATA_EN | USER_FILTER_ONESENSING_EN | I2C_DEBUG_EN | UL60730_SELF_TEST_EN) == 1)
struct user_param user;
#endif

/** ********************************************************************
 * @brief       chech max value in buffer
 * @param[in]   None
 * @return      None
 * 
 **********************************************************************/
#if (USER_FILTER_DIFFDATA_EN == 1)
uint16_t get_max(uint16_t* mp_start, uint8_t m_offset, uint8_t m_len)
{
    uint8_t i;
    int max;
    
    max = 0;
    for (i=0; i<m_len; i++)
    {
        if(*(mp_start+i*m_offset) > max) 
        {
            max = *(mp_start+i*m_offset);
        }
    }

    return max;
}


/** ********************************************************************
 * @brief       Diff Value Filter for User
 * @param[in]   None
 * @return      None
 * 
 **********************************************************************/
void TS_User_Filter_diffdata(void)
{
    uint8_t i;
    uint8_t j;
    static uint8_t relLevel_n[TS_MAX_KEY] = {0,};
    
    for (i=0; i<ts.actv_ch_cnt; i++)
    {
        if (ts.detect_flag & BIT(i))
        {
            user.diff_queue[i][user.diff_queue_idx++] = ts.diff_data[i];
            user.diff_queue_idx %= RAW_Q_SIZE;          
            if (abs(ts.diff_data[i]) < (ts.thd_detect[i]*ts.release_rate[i]/100))
            {
                relLevel_n[i]++;

                if (relLevel_n[i] >= 2)
                {
                    relLevel_n[i] = 0;
                    for (j=0; j<RAW_Q_SIZE; j++)
                    {
                        user.diff_queue[i][j] = ts.diff_data[i];
                    }
                }
            }
            else
            {
                relLevel_n[i] = 0;
            }
            ts.diff_data_filter[i] = get_max(&user.diff_queue[i][0], 1 ,RAW_Q_SIZE);
        }
        else
        {
            relLevel_n[i] = 0;
            ts.diff_data_filter[i] = ts.diff_data[i];
        }
    }
}
#endif



/** ********************************************************************
 * @brief       Onesensing Value Filter for User
 * @param[in]   None
 * @return      None
 * 
 **********************************************************************/
#if (USER_FILTER_ONESENSING_EN == 1)
void TS_User_Filter_onesensingdata(void)
{
    uint8_t i = 0;
    uint32_t temp1, temp2 = 0;     
    
    for (i = 0; i < ts.actv_ch_cnt; i++)
    {
        // median filer
        temp1 = filter_median(ts.one_sensing_compen[i][0], user.medDataBuf[i][0]);
        temp2 = filter_median(ts.one_sensing_compen[i][1], user.medDataBuf[i][1]);

        // average filer
        ts.one_sensing_filter[i][0] = filter_moving_average(temp1, user.avgDataBuf[i][0]);
        ts.one_sensing_filter[i][1] = filter_moving_average(temp2, user.avgDataBuf[i][1]);
    }
}


/** ********************************************************************
 * @brief       Median Filter for User
 * @param[in]   None
 * @return      None
 * 
 **********************************************************************/
uint16_t filter_median (uint16_t input, uint16_t* medDataBuf)
{
    uint8_t zz;
    uint8_t yy;
    uint16_t tempBuf[FILTER_SIZE_MID];
    uint32_t temp;

    medDataBuf[FILTER_SIZE_MID] = input;
    for(zz=0; zz<FILTER_SIZE_MID; zz++){
        if(medDataBuf[zz]==0) {
            medDataBuf[zz] = input;
        }
        else{
            medDataBuf[zz]  = medDataBuf[zz+1];
        }
        // memcpy
        tempBuf[zz] = medDataBuf[zz];
    }
    
    // LowRawBuf[0] <-- lowest
    // LowRawBuf[FILTER_SIZE_MID]  <-- highest
    for(zz=0; zz<FILTER_SIZE_MID; zz++){
        for(yy=0; yy<(FILTER_SIZE_MID-1); yy++){
            if(tempBuf[yy+1]<tempBuf[yy]){
                temp=tempBuf[yy+1];
                tempBuf[yy+1]=tempBuf[yy];
                tempBuf[yy]=temp;
            }
        }
    }
    
    temp = (tempBuf[MEDIAN_AVG_FIRST]+tempBuf[MEDIAN_AVG_SEC])>>1;

    return (uint16_t)temp;
}


/** ********************************************************************
 * @brief       Average Filter for User
 * @param[in]   None
 * @return      None
 * 
 **********************************************************************/
uint16_t filter_moving_average (uint16_t input, uint16_t* avgDataBuf)
{
    uint8_t zz;
    uint32_t temp = 0;          

    avgDataBuf[FILTER_SIZE_MAX] = input;
    for(zz=0; zz<FILTER_SIZE_MAX; zz++){
        if(avgDataBuf[zz]==0) {
            avgDataBuf[zz] = input;
        }
        else{
            avgDataBuf[zz]  = avgDataBuf[zz+1];
        }
        temp += avgDataBuf[zz];
    }
    
    temp /= FILTER_SIZE_MAX;

    return (uint16_t)temp;
}
#endif


/** ********************************************************************
 * @brief       Algorithm for selecting sensing frequency with good SNR
 * @param[in]   None
 * @return      None
 * 
 **********************************************************************/
#if (TOUCH_FREQ_SEL_EN == 1)
void Touch_Freq_Sel(void)
{
    uint8_t i;
    int16_t delta[2];
    
    for (i = 0; i < ts.actv_ch_cnt; i++)
    {       
        ts.raw_data[i][0] = ((((uint32_t)ts.one_sensing_filter_pre[i][0]*ts.iir_offset_cnt[i])+ts.one_sensing_filter[i][0])/(ts.iir_offset_cnt[i]+1));
        ts.raw_data[i][1] = ((((uint32_t)ts.one_sensing_filter_pre[i][1]*ts.iir_offset_cnt[i])+ts.one_sensing_filter[i][1])/(ts.iir_offset_cnt[i]+1));

        delta[0] = (int16_t)(ts.raw_data[i][0]-ts.one_sensing_pre[i][0]);
        delta[1] = (int16_t)(ts.raw_data[i][1]-ts.one_sensing_pre[i][1]);

        ts.fluct_sum[i][0] += abs(delta[0]);
        ts.fluct_sum[i][1] += abs(delta[1]);
        ts.fluct_cnt[i]++;
        ts.fluct_cnt[i] %= 5;
        
        if (ts.fluct_cnt[i] == 0)
        {
            if (ts.fluct_sum[i][0] < ts.fluct_sum[i][1])
            {
                ts.fluct_frq_sel[i] = 0;
            }
            else
            {
                ts.fluct_frq_sel[i] = 1;
            }

            ts.fluct_sum[i][0] = 0;
            ts.fluct_sum[i][1] = 0;
        }
    }
}
#endif

/** ********************************************************************
 * @brief       User scenario process
 * @param[in]   None
 * @return      None
 * 
 **********************************************************************/
#if ((HOLD_KEY_RELEASE_EN | MULTI_KEY_RESET_EN | FIRST_KEY_PRIORITY_EN | BIG_DIFF_CANCEL_EN | CHANNEL_GROUPING_EN ) == 1)
void Touch_Key_Scenario (void)
{
    uint8_t i;
    
#if (MULTI_KEY_RESET_EN  == 1)
    user.multi_key_check = 0;

    for (i = 0; i < ts.actv_ch_cnt; i++)
    {
        if (ts.detect_flag & BIT(i))    /* release check */         
        {
            if((user.multi_ch_bit & BIT(i)))       
            {
                // if enter another key, count
                user.multi_key_check++;
            }
        }
    }

    if(user.multi_key_check > user.multi_key_limit)
    {
        SCU->RSER |= 0x10;  // S/W reset EN    
        SCU->SCR = 0x9EB30001;  // S/W reset 
    }
#endif

#if (HOLD_KEY_RELEASE_EN == 1)
    if (ts.detect_flag) 
    {
        if(ts.pre_detect_flag == 0)
        {
            UT_Set_Timer(UT_HOLD_KEY_RELEASE, user.hold_key_release_time);
        }
        else if (UT_Is_Expired(UT_HOLD_KEY_RELEASE) == 1)
        {
            for (i = 0; i < ts.actv_ch_cnt; i++)
            {
                ts.baseline[i][0] = ts.raw_data[i][0];
                ts.baseline[i][1] = ts.raw_data[i][1];

            #if (TOUCH_FREQ_SEL_EN == 1)
                ts.fluct_sum[i][0] = 0;
                ts.fluct_sum[i][1] = 0;
                ts.fluct_cnt[i] = 0;
            #endif
                ts.fluct_frq_sel[i] = 0;
            }                             
        }
    }
    else
    {
        UT_Clear_Timer(UT_HOLD_KEY_RELEASE);
    }
#endif

#if (FIRST_KEY_PRIORITY_EN == 1)
    if(user.priority_ch_idx & ts.detect_flag)
    {
        ts.detect_flag = user.priority_ch_idx & ts.detect_flag;
        ts.detect_key = user.priority_ch_idx & ts.detect_flag; 
    }
#endif

#if (BIG_DIFF_CANCEL_EN == 1)
    for (i = 0; i < ts.actv_ch_cnt; i++)
    {
        if(ts.diff_data_filter[i] > user.big_diff_thd[i])
        {
            ts.detect_flag &= ~(BIT(i));
            ts.detect_key &= ~(BIT(i));
        }
    }
#endif

#if (CHANNEL_GROUPING_EN == 1)
    for (i = 0; i < ts.actv_ch_cnt; i++)
    {
        if ((ts.detect_flag & BIT(i)) && (user.ch_group_set[i] != 0))  
        {
            if(user.group_idx == 0)
            {
                user.group_idx = user.ch_group_set[i];
            }
            else
            {
                if(user.group_idx != user.ch_group_set[i])
                {
                    ts.detect_flag &= ~(BIT(i));
                    ts.detect_key &= ~(BIT(i));
                }
            }
        }
    }

    // no-touch
    if(ts.detect_flag == 0)
    {
        user.group_idx = 0;
    }
   
#endif

    ts.pre_detect_flag = ts.detect_flag;
}
#endif



