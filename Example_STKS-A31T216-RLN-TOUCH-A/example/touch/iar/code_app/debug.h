/**
 *******************************************************************************
 * @file        debug.h
 * @author      ABOV R&D Division
 * @brief       A31T21x Data Debugging Header
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

#ifndef __DEBUG_H__
#define __DEBUG_H__


enum dbg_smartkey
{
    DBG_DATA_1  = 0x01,
    DBG_DATA_2  = 0x02,
    DBG_DATA_3  = 0x04,
    DBG_DATA_4  = 0x08,
    DBG_DATA_5  = 0x10,
    DBG_DATA_6  = 0x20,
    DBG_DATA_7  = 0x40,
    DBG_DATA_8  = 0x80
};

#define DBG_MSK_ITEM    (DBG_DATA_1+DBG_DATA_2+DBG_DATA_3+DBG_DATA_4+DBG_DATA_5+DBG_DATA_6+DBG_DATA_7+DBG_DATA_8)
#define DBG_START_IDX   0
#define DBG_CH_CNT      1

struct dbg_cfg
{
    uint8_t start_idx;
    uint8_t ch_cnt;
    uint8_t mask;
    uint32_t baud;
    
    struct{
        uint8_t busy        :1;
        uint8_t fla_dbg     :1;
        uint8_t rsvd        :6;
    } flag;
} ;

#define DBG_SMARTKEY_GET_STX    0x3A
#define DBG_SMARTKEY_GET_CR     0x0D
#define DBG_SMARTKEY_GET_LF     0x0A
#define DBG_SMARTKEY_SET_STX    0x02
#define DBG_SMARTKEY_SET_ETX    0x03
#define DBG_RX_BUF_SIZE         100
#define DBG_TX_BUF_SIZE         200
#define DBG_RX_MSG_SIZE         25

struct dbg_uart
{   
    uint16_t rx_write_idx;
    uint16_t rx_read_idx;
    uint8_t rx_queue[DBG_RX_BUF_SIZE];
    uint16_t tx_write_idx;
    uint16_t tx_read_idx;
    uint16_t tx_checksum;
    uint8_t tx_queue[DBG_TX_BUF_SIZE];
};
enum dbg_rx_step
{
    DBG_RX_STEP_STX = 0,
    DBG_RX_STEP_CMD,
    DBG_RX_STEP_LENGTH,
    DBG_RX_STEP_DATA,
    DBG_RX_STEP_CHKSUM,
    DBG_RX_STEP_ETX,
    DBG_RX_STEP_ERR = 0xff
};
#define DBG_RX_READY        0
#define DBG_RX_COMPLETE     1
#define DBG_RX_ERR          2

extern struct dbg_cfg dbg;
extern struct dbg_uart uart;
void DBG_Do_Task(void);
void Make_Packet_Byte(uint8_t dat);
void Make_Packet_Word(uint16_t dat);
void DBG_Set_Baudrate(uint32_t dbgBaud);
void USI0RX_ISR(void);
void USI0TX_ISR(void);

#endif  // __DEBUG_H__
