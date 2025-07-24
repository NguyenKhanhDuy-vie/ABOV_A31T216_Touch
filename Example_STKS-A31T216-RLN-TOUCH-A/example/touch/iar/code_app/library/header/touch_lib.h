/**
 *******************************************************************************
 * @file        touch_lib.h
 * @author      ABOV R&D Division
 * @brief       A31T21x Touch Sensor Operation Header for Library
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

#ifndef __TOUCH_H__
#define __TOUCH_H__


enum{
    TOUCH_FREQ_16M      = 0x00,
    TOUCH_FREQ_8M       = 0x01,
    TOUCH_FREQ_4M       = 0x02,
    TOUCH_FREQ_2M       = 0x03,
    TOUCH_FREQ_1M       = 0x04,
    TOUCH_FREQ_0_5M     = 0x05,
    TOUCH_FREQ_0_25M    = 0x06,
    TOUCH_FREQ_0_125M   = 0x07,
};

enum{
    MODE_FLOATING   = 0x00,
    MODE_OUT_LOW    = 0x01,
    MODE_OUT_HIGH   = 0x02
};

enum{
    TOUCH_BREAK_TIME    = 0x01,
    TOUCH_COMPLETE      = 0x02
};

enum{
    TS_MODE_NORMAL    = 0x00,
    TS_MODE_HISENSE   = 0x01,
    TS_MODE_ADJUST    = 0x02,
};

enum{
    INTENAL_CAPACITOR_0_pF        = 0x00,
    INTENAL_CAPACITOR_0_8pF       = 0x10,
    INTENAL_CAPACITOR_1_6pF       = 0x20,
    INTENAL_CAPACITOR_3_2pF       = 0x40,
    INTENAL_CAPACITOR_5_6pF       = 0x70,   /* Default */
    INTENAL_CAPACITOR_7_2pF       = 0x80,
    INTENAL_CAPACITOR_13_2pF      = 0xF0,
};

enum{
    SERIAL_REGISTER_OPEN    = 0,
    SERIAL_REGISTER_SHORT   = 1,
    SERIAL_REGISTER_5K      = 2,
    SERIAL_REGISTER_100K    = 4,    /* Default */
    SERIAL_REGISTER_200K    = 8,
    SERIAL_REGISTER_33K     = 6,
    SERIAL_REGISTER_67K     = 12,
    SERIAL_REGISTER_40K     = 10,
    SERIAL_REGISTER_28K     = 14,
};

typedef enum
{
    ACT_CH_BIT_CFG = 0x00,             
    TOUCH_CLK_CFG, 
    TOUCH_CLK_OFFSET_CFG,
    TOUCH_MODE_CFG,
    TOUCH_PORT_SEL_CFG,
    TOUCH_VHS_CFG,
    TOUCH_COMP_REFV_CFG,
    TIMER_STARTUP_CFG,
    TIMER_BASE_SAMPLE_CFG,
    TIMER_BASE_REVERSE_SAMPLE_CFG,    
    BASE_RELEASE_HOLD_CNT_CFG,
    MESH_SHIELD_EN_CFG,
    CH_SHIELD_EN_CFG,
    CH_SHIELD_BIT_CFG,
    REF_CH_EN_CFG,
    REF_CH_BIT_CFG,
    REF_CH_SUM_CNT_CFG,
    REF_CH_LPF_C_CFG,
    REF_CH_LPF_R_CFG,
    TOUCH_COM_CFG_MAX,
}ts_common_cfg;

typedef enum
{
    TOUCH_SUM_CNT_CH_CFG = 0x00,
    TOUCH_LPF_C_CFG,
    TOUCH_LPF_R_CFG,
    THD_DETECT_CH_CFG,
    THD_RELEASE_RATE_CH_CFG,
    THD_REVERSE_RATE_CH_CFG,
    BASE_TRACE_SPEED_CHECK_RATE_CH_CFG,
    BASE_TRACE_SPEED_FAST_GAIN_CH_CFG,
    BASE_TRACE_SPEED_SLOW_GAIN_CH_CFG,
    FILTER_IIR_GAIN_CH_CFG,
    DETECT_DEBOUNCE_CNT_CH_CFG,
    RELEASE_DEBOUNCE_CNT_CH_CFG,
    REF_CH_GAIN_CFG,
    REF_CH_DIRECTION_CFG,
    TOUCH_CH_CFG_MAX,
}ts_channel_cfg;


enum{
    CH_00 = 0,
    CH_01,
    CH_02,
    CH_03,
    CH_04,
    CH_05,
    CH_06,
    CH_07,
    CH_08,
    CH_09,
    CH_10,
    CH_11,
    CH_12,
    CH_13,
    CH_14,
    CH_15,
    CH_16,
    CH_17,
    CH_18,
    CH_19,
    CH_20,
    CH_21,
    CH_22,
    CH_23,
    CH_MAX
};

typedef enum
{
    TS_STATE_INIT = 0x00,
    TS_STATE_CAL,
    TS_STATE_SENSING,
    TS_STATE_COMPLETE,
    TS_STATE_INIT_BASE,
    TS_STATE_MAKE_DATA,
    TS_STATE_DICISION_KEY,
    TS_STATE_UPDATE_BASE,
    TS_STATE_NONE
}ts_state ;

struct ts_param
{
    ts_state state;
    uint8_t touch_lib_ver;

    uint8_t mode;
    uint8_t max_cap_idx;
    uint16_t init_delay;

    uint8_t inact_port;
    uint8_t op_freq;
    uint8_t delta_osc;
    uint8_t *touch_sum_count;
    uint8_t touch_sum_count_ref;
    uint8_t *rc_filter_buf;
    uint8_t rc_filter_buf_ref;
    uint16_t comp_ref;
    uint16_t vhs;
    uint16_t *sco_buf;
    uint32_t shield_ch_bit;
    uint16_t *total_cap;

    uint8_t *ch_num_buf;         
    uint8_t ch_num_buf_ref;         
    uint8_t actv_ch_idx;
    uint8_t actv_ch_idx_ref;
    uint8_t actv_ch_cnt;
    uint32_t actv_ch_bit;         
    uint32_t actv_ch_bit_ref;     

    uint16_t one_sensing_ref[2];
    uint16_t one_sensing_ref_first_data[2];

    uint16_t *ref_ch_gain;
    uint8_t *ref_ch_dir;
    uint8_t *iir_offset_cnt;
    uint8_t *fluct_cnt;
    uint8_t *fluct_frq_sel;

    uint8_t *base_track_slow_gain;
    uint8_t *base_track_fast_gain;
    uint16_t base_track_period;
    uint16_t raw_sum_cnt;
    uint8_t base_release_hold_cnt;
    uint8_t *reverse_rate;
    uint8_t reverse_ch_idx;
    uint16_t reverse_time;
    
    uint16_t *diff_data;
    uint16_t *diff_data_filter; 

    uint16_t *thd_detect;
    uint8_t *release_rate;

    uint8_t *noise_thd_rate;

    uint8_t *debounce_cnt_buf;    
    uint8_t *debounce_cnt_detect;
    uint8_t *debounce_cnt_release;

    uint32_t detect_key;    
    uint32_t detect_flag;
    uint32_t pre_detect_flag;
        
    uint8_t *base_hold_cnt_buf;

    uint32_t wheel_detect;
    uint32_t wheel_actv_ch_bit;
    uint32_t slide_detect;
    uint32_t slide_actv_ch_bit;
    
    struct{
        uint8_t base_init               :1;
        uint8_t ts_status               :2;
        uint8_t mesh_shld_en            :1;
        uint8_t ch_shld_en              :1;
        uint8_t user_filter_onesensing  :1;
        uint8_t user_filter_diffdata    :1;
        uint8_t ref_ch_en               :1;

        uint8_t touch_freq_sel_mode     :1;
        uint8_t touch_sensing_end       :1;
        uint8_t reserved                :6;
    } flag;

    uint16_t (*fluct_sum)[2];
    uint16_t (*one_sensing_pre)[2];
    uint16_t (*one_sensing)[2];
    uint16_t (*one_sensing_compen)[2];
    uint16_t (*one_sensing_filter)[2];
    uint16_t (*one_sensing_filter_pre)[2];
    uint16_t (*raw_data)[2];
    uint16_t (*baseline)[2];    
    uint32_t (*raw_sum_buf)[2];
    uint16_t (*raw_avrg_buf)[2];
};

struct ts_reg
{
    uint16_t SCO[TS_MAX_KEY];
    uint16_t tsReg[SFR_TS_MAX+2];
} ;


extern uint8_t  g_actv_ch_num[];
extern uint8_t  g_actv_ch_num_ref;
extern uint16_t g_zero_sco[];
extern uint8_t  g_rc_filter[];
extern uint8_t  g_touch_sum_count[];
extern uint16_t g_one_sensing[][2];
extern uint16_t g_one_sensing_pre[][2]; 
extern uint16_t g_ref_ch_gain[];
extern uint16_t g_one_sensing_compen[][2];
extern uint16_t g_one_sensing_filter[][2];
extern uint16_t g_one_sensing_filter_pre[][2];
extern uint8_t  g_iir_offset_cnt[];
extern uint16_t g_raw_data[][2];
extern uint16_t g_baseline[][2]; 
extern uint32_t g_base_sample_sum_data[][2];
extern uint16_t g_base_sample_avg_data[][2];
extern uint8_t  g_noise_thd_rate[];
extern uint8_t  g_base_track_slow_gain[];
extern uint8_t  g_base_track_fast_gain[];
extern uint8_t  g_reverse_rate[];
extern uint16_t g_diff_data[]; 
extern uint16_t g_diff_data_filter[];
extern uint16_t g_fluct_sum[][2];   
extern uint8_t  g_fluct_cnt[];
extern uint8_t  g_fluct_frq_sel[];
extern uint16_t g_thd_detect[];
extern uint8_t  g_release_rate[];
extern uint8_t  g_debounce_cnt_detect[];
extern uint8_t  g_debounce_cnt_release[];
extern uint8_t  g_debounce_cnt[];
extern uint8_t  g_base_hold_cnt[];

extern struct ts_param ts;
extern struct ts_reg ts_reg;

extern void (*func_p_Touch_Freq_Sel)(void);
extern void (*func_p_user_filter_onesensingdata)(void);
extern void (*func_p_user_filter_diffdata)(void);

void Touch_ISR(void);
void Touch_Start_Touch(uint8_t indx);
void Touch_Set_Shield(uint8_t ch_shld, uint8_t mesh_shld);
void Touch_Do_Task(void);
extern void Library_Data_Assign(void);
extern void Set_Touch_Config (void);
extern void Set_TS_IO (void);
extern void Init_Touch (void);


//--------------------------------------------------------------------------------------------------


#endif  // __TOUCH_H__
