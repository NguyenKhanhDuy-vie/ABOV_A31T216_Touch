/**
 *******************************************************************************
 * @file        touch_config.c
 * @author      ABOV R&D Division
 * @brief       A31T21x Touch setting configure code
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
#include "A31T21x_driver_common.h" 

#include "user_timer.h"
#include "user_function.h"
#include "touch_config.h"
#include "touch_lib.h"
#if (UL60730_SELF_TEST_EN ==1)
#include "ul60730_lib_common_V01.h"
#endif

uint8_t  g_actv_ch_num[ACT_KEY_NUM];
uint16_t g_zero_sco[ACT_KEY_NUM];
uint8_t  g_rc_filter[ACT_KEY_NUM];
uint8_t  g_touch_sum_count[ACT_KEY_NUM];
uint16_t g_total_cap[ACT_KEY_NUM];
uint16_t g_one_sensing[ACT_KEY_NUM][2];
uint16_t g_one_sensing_pre[ACT_KEY_NUM][2]; 
uint16_t g_ref_ch_gain[ACT_KEY_NUM];
uint8_t  g_ref_ch_dir[ACT_KEY_NUM];
uint16_t g_one_sensing_compen[ACT_KEY_NUM][2];
uint16_t g_one_sensing_filter[ACT_KEY_NUM][2];
uint16_t g_one_sensing_filter_pre[ACT_KEY_NUM][2];
uint8_t  g_iir_offset_cnt[ACT_KEY_NUM];
uint16_t g_raw_data[ACT_KEY_NUM][2];
uint16_t g_baseline[ACT_KEY_NUM][2]; 
uint32_t g_base_sample_sum_data[ACT_KEY_NUM][2];
uint16_t g_base_sample_avg_data[ACT_KEY_NUM][2];
uint8_t  g_noise_thd_rate[ACT_KEY_NUM];
uint8_t  g_base_track_slow_gain[ACT_KEY_NUM];
uint8_t  g_base_track_fast_gain[ACT_KEY_NUM];
uint8_t  g_reverse_rate[ACT_KEY_NUM];
uint16_t g_diff_data[ACT_KEY_NUM]; 
uint16_t g_diff_data_filter[ACT_KEY_NUM];
uint16_t g_fluct_sum[ACT_KEY_NUM][2];   
uint8_t  g_fluct_cnt[ACT_KEY_NUM];
uint8_t  g_fluct_frq_sel[ACT_KEY_NUM];
uint16_t g_thd_detect[ACT_KEY_NUM];
uint8_t  g_release_rate[ACT_KEY_NUM];
uint8_t  g_debounce_cnt_detect[ACT_KEY_NUM];
uint8_t  g_debounce_cnt_release[ACT_KEY_NUM];
uint8_t  g_debounce_cnt[ACT_KEY_NUM];
uint8_t  g_base_hold_cnt[ACT_KEY_NUM];


/** *******************************************************************
 * @brief       initial for pointer variable
 * @param[in]   
 * @return      
 *
 *********************************************************************/
void Library_Data_Assign(void)
{
    ts.ch_num_buf           = &g_actv_ch_num[0];     

    ts.sco_buf              = &g_zero_sco[0];
    ts.total_cap            = &g_total_cap[0];
    ts.rc_filter_buf        = &g_rc_filter[0];
    ts.touch_sum_count      = &g_touch_sum_count[0];

    ts.one_sensing          = &g_one_sensing[0];
    ts.one_sensing_pre      = &g_one_sensing_pre[0];
    ts.ref_ch_gain          = &g_ref_ch_gain[0];
    ts.ref_ch_dir           = &g_ref_ch_dir[0];
    ts.one_sensing_compen   = &g_one_sensing_compen[0];
    ts.one_sensing_filter   = &g_one_sensing_filter[0];
    ts.one_sensing_filter_pre = &g_one_sensing_filter_pre[0];

    ts.iir_offset_cnt       = &g_iir_offset_cnt[0];
    ts.raw_data             = &g_raw_data[0];
    
    ts.baseline             = &g_baseline[0];
    ts.raw_sum_buf          = &g_base_sample_sum_data[0];
    ts.raw_avrg_buf         = &g_base_sample_avg_data[0];  
    ts.noise_thd_rate       = &g_noise_thd_rate[0];
    ts.base_track_slow_gain = &g_base_track_slow_gain[0];
    ts.base_track_fast_gain = &g_base_track_fast_gain[0];
    ts.reverse_rate         = &g_reverse_rate[0];

    ts.diff_data            = &g_diff_data[0];
    ts.diff_data_filter     = &g_diff_data_filter[0];
    ts.fluct_sum            = &g_fluct_sum[0];  
    ts.fluct_cnt            = &g_fluct_cnt[0];
    ts.fluct_frq_sel        = &g_fluct_frq_sel[0];  

    ts.thd_detect           = &g_thd_detect[0];
    ts.release_rate         = &g_release_rate[0];

    ts.debounce_cnt_detect  = &g_debounce_cnt_detect[0];
    ts.debounce_cnt_release = &g_debounce_cnt_release[0];
    ts.debounce_cnt_buf     = &g_debounce_cnt[0];

    ts.base_hold_cnt_buf    = &g_base_hold_cnt[0];
}

void Set_Touch_Config (void)
{
    uint8_t ch;

    /* Touch Library API */ 
    ts.state                    = TS_STATE_INIT;
    ts.actv_ch_cnt              = ACT_KEY_NUM; 
    ts.actv_ch_bit              = TS_COM_CFG[ACT_CH_BIT_CFG];
    ts.op_freq                  = TS_COM_CFG[TOUCH_CLK_CFG];
    ts.delta_osc                = TS_COM_CFG[TOUCH_CLK_OFFSET_CFG];
    ts.mode                     = TS_COM_CFG[TOUCH_MODE_CFG];
    ts.inact_port               = TS_COM_CFG[TOUCH_PORT_SEL_CFG];
    ts.vhs                      = TS_COM_CFG[TOUCH_VHS_CFG]/5;
    ts.comp_ref                 = TS_COM_CFG[TOUCH_COMP_REFV_CFG]/5;
    ts.init_delay               = TS_COM_CFG[TIMER_STARTUP_CFG];
    ts.base_track_period        = TS_COM_CFG[TIMER_BASE_SAMPLE_CFG];
    ts.base_release_hold_cnt    = TS_COM_CFG[BASE_RELEASE_HOLD_CNT_CFG]; 
    ts.reverse_time             = TS_COM_CFG[TIMER_BASE_REVERSE_SAMPLE_CFG];
    ts.flag.mesh_shld_en        = TS_COM_CFG[MESH_SHIELD_EN_CFG]&0x01;
    ts.flag.ch_shld_en          = TS_COM_CFG[CH_SHIELD_EN_CFG]&0x01;
    ts.shield_ch_bit            = TS_COM_CFG[CH_SHIELD_BIT_CFG];
    ts.flag.ref_ch_en           = TS_COM_CFG[REF_CH_EN_CFG]&0x01;
    ts.actv_ch_bit_ref          = TS_COM_CFG[REF_CH_BIT_CFG];
    ts.touch_sum_count_ref      = TS_COM_CFG[REF_CH_SUM_CNT_CFG];
    ts.rc_filter_buf_ref        = (TS_COM_CFG[REF_CH_LPF_C_CFG] | TS_COM_CFG[REF_CH_LPF_R_CFG]);

    for(ch = 0; ch < ts.actv_ch_cnt; ch++)
    {
        ts.touch_sum_count[ch]      = (uint8_t)TS_CH_CFG[ch][TOUCH_SUM_CNT_CH_CFG];
        ts.rc_filter_buf[ch]        = (uint8_t)(TS_CH_CFG[ch][TOUCH_LPF_C_CFG] | TS_CH_CFG[ch][TOUCH_LPF_R_CFG]);
        ts.thd_detect[ch]           = (uint16_t)TS_CH_CFG[ch][THD_DETECT_CH_CFG];
        ts.release_rate[ch]         = (uint8_t)TS_CH_CFG[ch][THD_RELEASE_RATE_CH_CFG]; 
        ts.noise_thd_rate[ch]       = (uint16_t)TS_CH_CFG[ch][BASE_TRACE_SPEED_CHECK_RATE_CH_CFG];
        ts.reverse_rate[ch]         = (uint8_t)TS_CH_CFG[ch][THD_REVERSE_RATE_CH_CFG];
        ts.base_track_slow_gain[ch] = (uint8_t)TS_CH_CFG[ch][BASE_TRACE_SPEED_SLOW_GAIN_CH_CFG];
        ts.base_track_fast_gain[ch] = (uint8_t)TS_CH_CFG[ch][BASE_TRACE_SPEED_FAST_GAIN_CH_CFG];
        ts.iir_offset_cnt[ch]       = (uint8_t)TS_CH_CFG[ch][FILTER_IIR_GAIN_CH_CFG];
        ts.debounce_cnt_detect[ch]  = (uint8_t)TS_CH_CFG[ch][DETECT_DEBOUNCE_CNT_CH_CFG];
        ts.debounce_cnt_release[ch] = (uint8_t)TS_CH_CFG[ch][RELEASE_DEBOUNCE_CNT_CH_CFG];
        ts.ref_ch_gain[ch]          = (uint16_t)TS_CH_CFG[ch][REF_CH_GAIN_CFG];
        ts.ref_ch_dir[ch]           = (uint8_t)TS_CH_CFG[ch][REF_CH_DIRECTION_CFG];
    }
}


void Init_Touch (void)
{
    ts.state = TS_STATE_INIT;
    ts.flag.touch_freq_sel_mode = TOUCH_FREQ_SEL_EN;
    ts.flag.user_filter_onesensing = USER_FILTER_ONESENSING_EN;      
    ts.flag.user_filter_diffdata = USER_FILTER_DIFFDATA_EN;      

    Library_Data_Assign();    
    Set_Touch_Config();
    Set_TS_IO();
    /* Default */
    t_sysbit.cal_ok = 0;       
    t_sysbit.led_io = 0;
    t_sysbit.dbg_trigger = 0;
    t_sysbit.time_div = TS_LED_TIME_DIV_EN; 
    UT_Run_Timer();                             /* use systick, touch library works with user-timer. */
}


/** *******************************************************************
 * @brief       Touch Interrupt Service Routine
                - Interrupt occurs whenever processing for one channel is completed.
                - in touch/LED time division operation mode
                : If the LED is working, this data is discarded and the touch operation for the 
                  next channel does not proceed until the LED operation is finished.
                : If the LED is not working, it performs a touch operation for the next channel
                - in touch/LED independent operation mode
                : If the LED is not working, it performs a touch operation for the next channel
 * @param[in]   
 * @return  void
 *
 *********************************************************************/
void TOUCH_IRQHandler(void)
{
    uint8_t addrIndx,i;

#if (UL60730_SELF_TEST_EN ==1)
    if(UL60730_g.touch_isr_en == 1)
    {
        UL60730_TOUCH_ISR();
        return;
    }
    else
#endif
    {
        if (t_sysbit.time_div != 0)
        {
            if (TS(SFR_TS_CON) & BIT(3))
            {
                ts.flag.ts_status = TOUCH_BREAK_TIME;
                NVIC_DisableIRQ(TOUCH_IRQn);
                TS(SFR_SHLD_CON) &= ~BIT(24);           /* CH & MESH Disable, Re-Enable in LED ISR */       
                TS(SFR_TS_CON) &= ~(BIT(3)+BIT(0));

                for(i=0;i<70;i++);                      /* 33us delay for 32Mhz MCLK */
                
                SCU->TIRCCON = 0;
                
                return;
            }
        }

        if(ts.actv_ch_idx_ref == 1)
        {
            addrIndx = ts.ch_num_buf_ref;
        }
        else
        {
            addrIndx = ts.ch_num_buf[ts.actv_ch_idx]; 
        }
        
        if((TSRAW(addrIndx*2) == 0) && (TSRAW(addrIndx*2+1) == 0))
        {
            TS(SFR_TS_CON) = 0;
            TS(SFR_TS_CON) = _TS_START;
            return;
        }
       
        /* get touch data */
        if(ts.actv_ch_idx_ref == 1)
        {
            addrIndx = ts.ch_num_buf_ref; 

            ts.one_sensing_ref[0] = (TSRAW(addrIndx*2));
            ts.one_sensing_ref[1] = (TSRAW(addrIndx*2+1));  

            ts.actv_ch_idx_ref = 2;
        }
        else
        {
            addrIndx = ts.ch_num_buf[ts.actv_ch_idx]; 

            ts.one_sensing[ts.actv_ch_idx][0] = (TSRAW(addrIndx*2));
            ts.one_sensing[ts.actv_ch_idx][1] = (TSRAW(addrIndx*2+1));
        }

        TS(SFR_TS_CON) = 0x00;
        
        ts.actv_ch_idx++;

        if(ts.actv_ch_idx_ref == 2)
        {
            ts.flag.ts_status = TOUCH_COMPLETE;
            ts.actv_ch_idx = 0;
            ts.actv_ch_idx_ref = 0;
            NVIC_DisableIRQ(TOUCH_IRQn);
        }
        else if (ts.actv_ch_idx == ts.actv_ch_cnt)
        {      
            if(ts.flag.ref_ch_en == 0)    
            {
                ts.flag.ts_status = TOUCH_COMPLETE;
                ts.actv_ch_idx = 0;
                ts.actv_ch_idx_ref = 0;
                NVIC_DisableIRQ(TOUCH_IRQn);
            }
            else
            {
                TS(SFR_TS_CH_SEL) = 0;
                addrIndx = ts.ch_num_buf_ref;
                TS(SFR_TS_CH_SEL) |= BIT(addrIndx);             
                TS(SFR_TS_SUM_CNT) = ts.touch_sum_count_ref;
                TS(SFR_TS_SLP_CON) = ts.rc_filter_buf_ref;

                TS(SFR_TS_CON) = _TS_START;
                ts.actv_ch_idx_ref = 1;
            }
        }
        else
        {
            TS(SFR_TS_CH_SEL) = 0;
            addrIndx = ts.ch_num_buf[ts.actv_ch_idx];
            TS(SFR_TS_CH_SEL) |= BIT(addrIndx);             
            TS(SFR_TS_SUM_CNT) = ts.touch_sum_count[ts.actv_ch_idx];       
            TS(SFR_TS_SLP_CON) = ts.rc_filter_buf[ts.actv_ch_idx];

            TS(SFR_TS_CON) = _TS_START; 
        }
    }
}


