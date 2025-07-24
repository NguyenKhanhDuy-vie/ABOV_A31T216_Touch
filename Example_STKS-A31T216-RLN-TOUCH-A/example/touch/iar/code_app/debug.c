/**
 *******************************************************************************
 * @file        debug.c
 * @author      ABOV R&D Division
 * @brief       A31T21x Data Debugging Code
 *
 * Copyright 2017 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************
 *  : USI0-UART/500000bps
 *  : DBG_Send_Data() protocol
 *    - 3A(STX)
 *    - xx(LEN): MODE ~ MSG
 *    - 01 10 (MODE)
 *    - xx(KEY)
 *    - 00(Cycle)
 *    - MSG
 *    - xx xx(CheckSum)
 *    - 0D 0A(CR/LF)
 *  : DBG_RX_Check() protocol
 *    - 02(STX)
 *    - xx(CMD) : 'R'(Get), 'M'(Frequency), 'T'(write), 'w', 'S'
 *    - MSG
 *    - 03(ETX)
 ******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include "A31T21x.h"
#include "a31t21x_uart.h"
#include "A31T21x_usart1n.h"
#include "A31T21x_gpio.h"
#include "a31t21x_driver_common.h"

#include "touch_lib.h"
#include "user_function.h"
#include "debug.h"
#if (UL60730_SELF_TEST_EN == 1)
#include "ul60730_lib_common_V01.h"
#endif
#if ((SLIDE_FUNCTION_EN | WHEEL_FUNCTION_EN) == 1) 
#include "gesture.h"
#endif


#if (UART_DEBUG_EN == 1)

#define DBG_TS_REG  1

uint8_t app_uart_rx_buf[8];
uint8_t app_uart_tx_buf[8];
static volatile uint8_t app_uart_tx_done;
static volatile uint8_t app_uart_rx_done;

struct dbg_cfg dbg;
struct dbg_uart uart;
static uint8_t s8_step = 0;
static uint8_t s8_buff[DBG_RX_MSG_SIZE];
static uint8_t s8_index = 0;

void app_uart_irq_handler(uint32_t event, void *context);


/** *******************************************************************
 * @brief       UART Interrupt call back
 * @param[in]   
 * @return
 *
 *********************************************************************/
void app_uart_irq_handler(uint32_t event, void *context)
{
    if (event & UART_EVENT_RECEIVE_DONE)
    {
        app_uart_rx_done = 1;
        uart.rx_queue[uart.rx_write_idx++] = app_uart_rx_buf[0];
        uart.rx_write_idx %= DBG_RX_BUF_SIZE;
        
        /* Additional Receive */
        app_uart_rx_done = 0;
        HAL_UART_Receive(UART_ID_1, app_uart_rx_buf, 1, UART_IO_TYPE_NON_BLOCK);
    }

    if (event & UART_EVENT_TRANSMIT_DONE)
    {
        app_uart_tx_done = 1;
        dbg.flag.busy = 0;
    }
}


/** *******************************************************************
 * @brief       DBG_Set_Baudrate
 * @param[in]   dbgBaud : uart baudrate                
 * @return      void
 *
 *********************************************************************/
void DBG_Set_Baudrate(uint32_t dbgBaud)
{
#if (0)
    uint32_t numerator;
    uint32_t denominator;
    uint32_t bdr, bfr;
    uint32_t fd;
    uint32_t cal_index1, cal_index2;

    /* Set Baudrate */
    numerator = SystemCoreClock / 2;
    denominator = 16 * dbgBaud;

    cal_index1 = (numerator / (denominator / 10));
    cal_index2 = (numerator / denominator) * 10;
    if ((cal_index1 - cal_index2) < 6)
    {
        bdr = (numerator / denominator);
    }
    else
    {
        bdr = (numerator / denominator) + 1;
    }

    fd = numerator - (bdr * denominator);
    bfr = (fd * 256) / denominator;

    UART1->BDR = bdr & 0xFFFF;
    UART1->BFR = bfr & 0xFF;    

    t_sysbit.dbg_en = 1;
#else
    dbg.baud = dbgBaud;
    t_sysbit.dbg_en = 1;
#endif
}


#if (0)
/***************************************** printf start *******************************************/
#ifdef __GNUC__
    /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf  
     set to 'Yes') calls __io_putchar() */
    #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
    #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
PUTCHAR_PROTOTYPE
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART */
    #if (1) // �׽�Ʈ �ʿ� 
    //while (!(UART1->LSR & UART_LSR_THRE)){}
    /////HAL_UART_WriteBuffer(UART1, ch);   
    UART1->THR = ch;
    #endif

    return ch;
}
#endif


/** *******************************************************************
 * @brief       UART Port Initial
 * @param[in]   
 * @return
 *
 *********************************************************************/
void DBG_Init_UART(void)    
{
    struct uart_cfg cfg;

    /* Configure UART TXD pin to be used */
    HAL_GPIO_SetFunction(PORTF, PIN4, Pn_MUX_AF1);
    HAL_GPIO_SetMode(PORTF, PIN4, GPIO_MODE_FUNC);
    HAL_GPIO_SetPull(PORTF, PIN4, GPIO_PULL_UP);

    /* Configure UART RXD pin to be used */
    HAL_GPIO_SetFunction(PORTF, PIN5, Pn_MUX_AF1);  
    HAL_GPIO_SetMode(PORTF, PIN5, GPIO_MODE_FUNC);
    HAL_GPIO_SetPull(PORTF, PIN5, GPIO_PULL_UP);
       
    cfg.baudrate = dbg.baud;
    cfg.data_bit = UART_DATA_BIT_8,
    cfg.parity_bit = UART_PARITY_BIT_NONE,
    cfg.stop_bit = UART_STOP_BIT_1,
    cfg.irq_prio = DRIVER_PRIORITY_LOWEST,

    HAL_UART_Init(UART_ID_1, &cfg, app_uart_irq_handler, NULL);
    HAL_UART_Receive(UART_ID_1, app_uart_rx_buf, 1, UART_IO_TYPE_NON_BLOCK);
    __enable_irq();
}


/** *******************************************************************
 * @brief       UART data buffer initial
 * @param[in]   
 * @return
 *
 *********************************************************************/
uint8_t DBG_Init_UART_Param (void)
{
    uint8_t i;

    s8_step = DBG_RX_STEP_STX;
    s8_index = 0;
    uart.rx_write_idx = 0;
    uart.rx_read_idx = 0; 
    for (i = 0; i < DBG_RX_BUF_SIZE; i++)
    {
        uart.rx_queue[i] = 0;
    }
    for (i = 0; i < DBG_RX_MSG_SIZE; i++)
    {
        s8_buff[i] = 0;
    }
    dbg.flag.busy = 0;

    return (1);
}


/** *******************************************************************
 * @brief       Initialize UART variables
 * @param[in]   "1A"(0x31,0x41) -> 0x1A
 * @return      void
 *
 *********************************************************************/
uint8_t Hexstr_To_Binary(uint8_t* mp_str)
{
    uint8_t i;
    uint8_t temp = 0;

    for (i = 0; i < 2; i++)
    {
        temp <<= 4;
        
        if ((*mp_str) >= '0' && (*mp_str) <= '9')
        {
            temp |= ((*mp_str) - '0');
        }
        else if ((*mp_str) >= 'a' && (*mp_str) <= 'f')
        {
            temp |= ((*mp_str) + 10 - 'a');
        }
        else
        {
            return 0xff;
        }
        mp_str++;
    }

    return temp;
}

/** *******************************************************************
 * @brief       check received data fromm smartkey
 * @param[in]   ptr : pointer of pointer of message buffer
 * @return      result : return rx-packet's complete state
 *
 *********************************************************************/
//#define DATA_LEN  8       // | 02 | 52 30 31 30 30 30 32 0D | 03 |
uint8_t DBG_RX_Check(uint8_t** ptr)
{
    uint8_t temp = 0;
    uint8_t result;

    result = DBG_RX_READY;

    while (uart.rx_write_idx != uart.rx_read_idx)   
    {
        temp = uart.rx_queue[uart.rx_read_idx++]; 
        uart.rx_read_idx %= DBG_RX_BUF_SIZE;
        
        switch (s8_step)
        {
            case DBG_RX_STEP_STX :                      /* STX */
                if (temp == DBG_SMARTKEY_SET_STX)
                {
                    s8_index = 0;
                    s8_step = DBG_RX_STEP_DATA;
                }
                break;                      
            case DBG_RX_STEP_DATA :                     /* MSG */
                s8_buff[s8_index++] = temp;
                if (temp == DBG_SMARTKEY_SET_ETX)
                {
                    s8_step = DBG_RX_STEP_STX;
                    result = DBG_RX_COMPLETE;
                    *ptr = &s8_buff[0];                 /* from cmd exclude len */
                }
                break;
            default :
                s8_step = DBG_RX_STEP_STX;
                break;
        }   

        if (result != DBG_RX_READY)
        {
            break;
        }
    }

    return (result);
}


/** *******************************************************************
 * @brief       Initialize debug parameters
 * @param[in]   none
 * @return      void
 *
 *********************************************************************/
void DBG_Init_Variable(void)
{   
    dbg.start_idx   = DBG_START_IDX;
    dbg.ch_cnt      = DBG_CH_CNT;
    dbg.mask        = DBG_MSK_ITEM; 
}


/** *******************************************************************
 * @brief       move one byte to tx-buffer
 * @param[in]   dat : data to move
 * @return      void
 *
 *********************************************************************/
void Make_Packet_Byte(uint8_t dat)
{
    uart.tx_queue[uart.tx_write_idx++] = dat;
    uart.tx_checksum += (uint16_t)(0x00ff & dat);   
}


/** *******************************************************************
 * @brief       move two byte to tx-buffer
 * @param[in]   dat : data to move
 * @return      void
 *
 *********************************************************************/
void Make_Packet_Word(uint16_t dat)
{
    uart.tx_queue[uart.tx_write_idx++] = (uint16_t)(dat>>8);
    uart.tx_queue[uart.tx_write_idx++] = (uint16_t)(dat>>0);
    uart.tx_checksum += (uint16_t)(0x00ff & (dat>>8));
    uart.tx_checksum += (uint16_t)(0x00ff & (dat>>0));
}


/** *******************************************************************
 * @brief       UART transmit debug data to smartkey gui
 * @param[in]   none
 * @return      void
 *
 *********************************************************************/
void DBG_Send_Data(void)
{
    uint8_t i;
    uint8_t len;         
    uint8_t shift_bit;
    uint8_t mask_cnt;

    uart.tx_write_idx = 0;
    uart.tx_read_idx = 0;
    uart.tx_checksum = 0;
    uart.tx_queue[uart.tx_write_idx++] = DBG_SMARTKEY_GET_STX;  /* header[1] */

    /* Length Calc */                   
    mask_cnt = 0;
    shift_bit = 0x01;
    for (i = 0; i < 8; i++)
    {
        if(dbg.mask & shift_bit)
        {
            mask_cnt++;
        }
        shift_bit<<=1;
    }

    #if (DBG_TS_REG == 1)
    len = 4+dbg.ch_cnt*mask_cnt*2+(48+38); 
    #else
    len = 4+dbg.ch_cnt*mask_cnt*2;
    #endif
    len = (len/2)|0x80;     
    Make_Packet_Byte(len);                                  /* length[1] */
    Make_Packet_Byte(0x00);                                 /* mode[2] */
    Make_Packet_Byte(0x10);
    Make_Packet_Byte(ts.detect_key);                        /* key[1] */
    Make_Packet_Byte(ts.touch_lib_ver);                     /* cyle[1] */
   
#if 1       /* for STKS Touch-A Board - Key */
    for (i = dbg.start_idx; i < dbg.start_idx+dbg.ch_cnt; i++)  
    {   
        if (dbg.mask & DBG_DATA_1)  // DATA 1             
        {
            Make_Packet_Word(ts.raw_data[i][0]);                
        }
        if (dbg.mask & DBG_DATA_2)  // DATA 2
        {               
            Make_Packet_Word(ts.baseline[i][0]);                
        }
        if (dbg.mask & DBG_DATA_3)  // DATA 3                       
        {
            Make_Packet_Word(ts.raw_data[i][1]);          
        }
        if (dbg.mask & DBG_DATA_4)  // DATA 4                           
        {
            Make_Packet_Word(ts.baseline[i][1]);           
        }
        if (dbg.mask & DBG_DATA_5)  // DATA 5                       
        {           
            Make_Packet_Word(ts.diff_data_filter[i]);
        }
        if (dbg.mask & DBG_DATA_6)  // DATA 6              
        {
            Make_Packet_Word(ts.thd_detect[i]);
        }
        if (dbg.mask & DBG_DATA_7)  // DATA 7               
        {
            #if 0
            Make_Packet_Word(ts.thd_detect[i]*ts.release_rate[i]/100);
            #else
            Make_Packet_Word(ts.total_cap[i]);         
            #endif
        }
        if (dbg.mask & DBG_DATA_8)  // DATA 8               
        {           
           Make_Packet_Word(ts.detect_key);
        }               
    }
#endif

#if 0       /* for STKS Touch-B Board - Wheel & Slide Gesture */
    for (i = dbg.start_idx; i < dbg.start_idx+dbg.ch_cnt; i++)  
    {   
        if (dbg.mask & DBG_DATA_1)  // DATA 1                       
        {
            Make_Packet_Word(ts.raw_data[i][0]);                
        }
        if (dbg.mask & DBG_DATA_2)  // DATA 2                           
        {
            Make_Packet_Word(ts.baseline[i][0]);           
        }
        if (dbg.mask & DBG_DATA_3)  // DATA 3                       
        {           
            Make_Packet_Word(ts.diff_data_filter[i]);
        }
        if (dbg.mask & DBG_DATA_4)  // DATA 4               
        {
            Make_Packet_Word(ts.detect_key);
        }
        if (dbg.mask & DBG_DATA_5)  // DATA 5                       
        {           
            Make_Packet_Word(ges.wheel.chperSum[i]);             
        }
        if (dbg.mask & DBG_DATA_6)  // DATA 6               
        {
            Make_Packet_Word(ges.wheel.positionCurr);             
        }
        if (dbg.mask & DBG_DATA_7)  // DATA 7               
        {
            Make_Packet_Word(ges.wheel.stepCurr);             
        }
        if (dbg.mask & DBG_DATA_8)  // DATA 8               
        {           
            Make_Packet_Word(ges.wheel.Direction);
        }               
    }
#endif

#if 0       /* for UL60730 */
    for (i = dbg.start_idx; i < dbg.start_idx+dbg.ch_cnt; i++)  
    {   
        if (dbg.mask & DBG_DATA_1)  // DATA 1                       
        {
            Make_Packet_Word(ts.raw_data[i][0]);                
        }
        if (dbg.mask & DBG_DATA_2)  // DATA 2                           
        {
            Make_Packet_Word(ts.baseline[i][0]);           
        }
        if (dbg.mask & DBG_DATA_3)  // DATA 3                       
        {           
            Make_Packet_Word(ts.diff_data_filter[i]);
        }
        if (dbg.mask & DBG_DATA_4)  // DATA 4               
        {
            Make_Packet_Word(ts.detect_key);
        }
        if (dbg.mask & DBG_DATA_5)  // DATA 5                       
        {           
            Make_Packet_Word(UL60730_g.rom_check_result>>16);             
        }
        if (dbg.mask & DBG_DATA_6)  // DATA 6               
        {
            Make_Packet_Word(UL60730_g.rom_check_result);             
        }
        if (dbg.mask & DBG_DATA_7)  // DATA 7               
        {
            Make_Packet_Word(user.ul60730_post_result);             
        }
        if (dbg.mask & DBG_DATA_8)  // DATA 8               
        {           
            Make_Packet_Word(user.ul60730_bist_result);             
        }               
    }
#endif
    
    #if (DBG_TS_REG == 1)
    /* SCO */
    for (i = 0; i < TS_MAX_KEY; i++)        /* 24 * 2 */
    {
        Make_Packet_Word(ts_reg.SCO[i]);
    }
    /* touch Register */
    for (i = 0; i < SFR_TS_MAX+2; i++)      /* (17+2)*2 */  
    {
        Make_Packet_Word(ts_reg.tsReg[i]);  
    }
    #endif

    uart.tx_checksum = ((~uart.tx_checksum)&0x00ffff)-1;    
    uart.tx_queue[uart.tx_write_idx++] = (uint8_t)(uart.tx_checksum>>8);
    uart.tx_queue[uart.tx_write_idx++] = (uint8_t)(uart.tx_checksum>>0);
    uart.tx_queue[uart.tx_write_idx++] = DBG_SMARTKEY_GET_CR;
    uart.tx_queue[uart.tx_write_idx++] = DBG_SMARTKEY_GET_LF;

    HAL_UART_Transmit(UART_ID_1, uart.tx_queue, uart.tx_write_idx, UART_IO_TYPE_NON_BLOCK);  
}


/** *******************************************************************
 * @brief       debug task
 * @param[in]   none
 * @return      void
 *
 *********************************************************************/
void DBG_Do_Task(void)
{
    static uint8_t stStep = 0;
    uint8_t* ptr;
    uint8_t temp;
    uint8_t bin1;
    uint8_t bin2;
    uint8_t bin3;
    
    if (t_sysbit.dbg_en == 0)
    {
        return;
    }
    
    switch (stStep)
    {
        case 0 :
            DBG_Init_UART();
            DBG_Init_UART_Param();
            DBG_Init_Variable();
            stStep++;
            break;
        case 1 :
            temp = DBG_RX_Check(&ptr);  // |02| ~ |0D|03|   
            if (temp == DBG_RX_COMPLETE)
            {
                switch (*ptr)
                {
                    case 'R':   /* 0x52 */
                        dbg.mask        = Hexstr_To_Binary(ptr+1);              
                        dbg.start_idx   = Hexstr_To_Binary(ptr+3);  
                        dbg.ch_cnt      = Hexstr_To_Binary(ptr+5);  //printf("mask=%02x, start_idx=%02x, ch_cnt=%02x \r\n", (u16)dbg.mask, (u16)dbg.start_idx, (u16)dbg.ch_cnt);    
                        break;
                    #if (0)
                    case 'M':   // 0x4D FREQ
                        temp = Hexstr_To_Binary(ptr+1);                     
                        switch (temp)
                        {
                            case 0x10 :
                                //g8_sfrIndx = 0;
                                break;
                            case 0x11 :
                                //g8_sfrIndx = 1;
                                break;
                            case 0x12 :
                                //g8_sfrIndx = 2;
                                break;
                        }   
                        break;
                    #endif                      
                    case 'T':   /* 0x54 : SFR */
                        bin1 = Hexstr_To_Binary(ptr+1); //
                        bin2 = Hexstr_To_Binary(ptr+3); 
                        bin3 = Hexstr_To_Binary(ptr+5);
                        if ((bin1>=0x50)&&(bin1<=0x77))         /* SCO */
                        {
                            bin1 -= 0x50;
                            bin1 = bin1/2;
                            ts_reg.SCO[bin1] =  (((uint16_t)bin2<<8) + bin3)&0x1fff; 
                            TS_SCO(bin1) = ts_reg.SCO[bin1]; 
                        }
                        else if ((bin1>=0x80)&&(bin1<=0x91))    /* REG */
                        {
                            bin1 -= 0x80;
                            ts_reg.tsReg[bin1] = bin2;
                            TS(bin1) = ts_reg.tsReg[bin1];
                        }
                        break;
                    case 'w':   
                        bin1 = Hexstr_To_Binary(ptr+1);
                        bin2 = Hexstr_To_Binary(ptr+3);                         
                        break;
                    case 'Z':
                        break;
                    default :
                        break;
                }
            }
            stStep++;
            break;
        case 2 :
            if ((dbg.flag.busy == 0)&&(t_sysbit.dbg_trigger == 1))
            {           
                t_sysbit.dbg_trigger = 0;                   
                dbg.flag.busy = 1;
                DBG_Send_Data();
            }           
            stStep = 1;
            break;
        default :
            stStep = 0;
            break;
    }
}
#endif
