/**
 *******************************************************************************
 * @file        i2c.c
 * @author      ABOV R&D Division
 * @brief       A31T21x I2C Function Code For Application
 *
 * Copyright 2017 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#include <stdio.h>
#include <stdlib.h>

#include "touch_lib.h"
#include "user_function.h"
#include "i2c.h"
#include "register_map.h"


#if (I2C_DEBUG_EN == 1)

enum{
    RXACK   = 0x01,
    TMODE   = 0x02,
    BUSY    = 0x04,
    MLOST   = 0x08,
    SSEL    = 0x10,
    STOPD   = 0x20,
    TEND    = 0x40,
    GCALL   = 0x80,
};

#define I2C_SLAVE_TX_SEL_ACK        0x17    //(SSEL||BUSY||TMODE||RXACK)
#define I2C_SLAVE_TX_DATA_NOACK     0x46    //(TEND||BUSY||TMODE)
#define I2C_SLAVE_TX_DATA_ACK       0x47    //(TEND||BUSY||TMODE||RXACK)
#define I2C_SLAVE_TX_GC_ACK         0x97    //(GCALL||SSEL||BUSY||TMODE||RXACK)
#define I2C_SLAVE_RX_SEL_ACK        0x15    //(SSEL||BUSY||RXACK)
#define I2C_SLAVE_RX_DATA_ACK       0x45    //(TEND||BUSY||RXACK)
#define I2C_SLAVE_RX_GC_ACK         0x95    //(GCALL||SSEL||BUSY||RXACK)

#define I2C_TX_COMPLETE             0x22    //(STOPD||TMODE)
#define I2C_RX_COMPLETE             0x20    //(STOPD)

#define I2C_REG_OFFSET              0x100


uint8_t I2C_SCL_HOLD_FLG;

static __INLINE I2C_Type *I2C_REG(enum i2c_id id)
{
    return (I2C_Type *)(I2C0_BASE + (I2C_REG_OFFSET * id));
}

static __INLINE IRQn_Type I2C_IRQ_NUM(enum i2c_id id)
{
    if (id == 0)
    {
        return (IRQn_Type)((uint8_t)I2C0_IRQn);
    }
    else
    {
        return (IRQn_Type)((uint8_t)I2C0_IRQn + id + 3);
    }

}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_I2C_SetOwnAddress(enum i2c_id id, uint8_t own_addr, uint8_t general_call )
{
    I2C_Type *i2c;

    DRIVER_ASSERT(id < I2C_ID_MAX);

        /* Get I2C Register */
    i2c = I2C_REG(id);

        /* Set Slave Own Address */
    i2c->SAR1 = (own_addr << 1) | (general_call << 0);
    i2c->SAR2 = (own_addr << 1) | (general_call << 0);

    return DRIVER_ERROR_OK;
}
//////////////////////////////////////////////////////////////////////////////////////
void HAL_I2C_Init(enum i2c_id id)
{
    //struct i2c_ctrl_block *icb;
    I2C_Type *i2c;
    uint16_t scll = 0;
    uint16_t sclh = 0;

    DRIVER_ASSERT(id < I2C_ID_MAX);

    /* Get I2C Register */
    i2c = I2C_REG(id);

    /* Enable I2C Block */
    SCU->PER2 &= ~(1 << (4 + id));
    SCU->PCER2 &= ~(1 << (4 + id));
    SCU->PER2 |= (1 << (4 + id));
    SCU->PCER2 |= (1 << (4 + id));

    HAL_I2C_SetOwnAddress(I2C_ID_1, SLAVE_ADDR, I2C_GENERAL_CALL);

    /* Configure I2C Parameter */

    I2C_SCL_HOLD_FLG = 0;
    i2c->SLTPDR = 0x9C3F;   // tpclk(1/32Mhz)*4*(0x9C3F+1) = 5ms
    i2c->SLTCR |= 0x03;     // i2C SCL hold timeout interrupt enable

    i2c->CR |= (1 << 7);    // <EN>
    i2c->CR |= (1 << 3);    // <ACKEN>
    i2c->CR |= (1 << 5);    // <IEN>

    switch (I2C_FREQ)
    {
        case I2C_FREQ_400KHZ:
            scll = ((SystemPeriClock/800000) - 2) / 4;
            sclh = ((SystemPeriClock/800000) - 2) / 4;
            break;
        case I2C_FREQ_200KHZ:
            scll = ((SystemPeriClock/400000) - 2) / 4;
            sclh = ((SystemPeriClock/400000) - 2) / 4;
            break;
        case I2C_FREQ_100KHZ:
            scll = ((SystemPeriClock/200000) - 2) / 4;
            sclh = ((SystemPeriClock/200000) - 2) / 4;
            break;
        default:
            DRIVER_ASSERT(0);
    }

    i2c->SDHR = 3;
    i2c->SCHR = sclh;
    i2c->SCLR = scll;

    /* Set NVIC */
    NVIC_ClearPendingIRQ(I2C_IRQ_NUM(id));
    NVIC_SetPriority(I2C_IRQ_NUM(id), I2C_IRQ_PRIO);
    NVIC_EnableIRQ(I2C_IRQ_NUM(id));    
}

//////////////////////////////////////////////////////////////////////////////////////
static uint8_t I2C_SubAddr = 0;
static uint8_t SubAddrFlg = 0;
static uint8_t SubAddrPrevFlg = 0;

static void I2C_SlaveIRQHandler(enum i2c_id id)
{
    I2C_Type *i2c;
    volatile uint32_t status;
    i2c = I2C_REG(id);
    status = i2c->ST;

    if(status & 0x80000000)
    {
        I2C_SCL_HOLD_FLG = 1;   // err flag set
        i2c->ST = 0x800000FF;   // I2C Flag clear
        return;
    }

    if(status & SSEL)   // slave address matched 
    {
        SubAddrPrevFlg = 0;
        
        if(status & TMODE)
        {
            i2c->DR = RegMap[I2C_SubAddr];
            I2C_SubAddr++;
        }
        else
        {
            SubAddrFlg = 0;
        }
    }
    else if(status & STOPD) // I2C Stop
    {
        if(SubAddrPrevFlg)
        {
            SubAddrPrevFlg = 0;
        }
        else
        {
            ;
        }
    }
    else
    {
        SubAddrPrevFlg = 0;
        
        if(status & TMODE)
        {
            if(status & RXACK)
            {
                i2c->DR = RegMap[I2C_SubAddr];
                I2C_SubAddr++;
            }
        }
        else
        {
            if(SubAddrFlg == 0)
            {
                I2C_SubAddr = i2c->DR;
                SubAddrFlg = 1;
                SubAddrPrevFlg = 1;
            }
            else
            {
                RegMap[I2C_SubAddr] = i2c->DR;
                I2C_SubAddr++;
            }
        }
    }
    
    i2c->ST = 0x800000FF;   // I2C Flag clear
}


//////////////////////////////////////////////////////////////////////////////////////
void I2C0_IRQHandler(void)
{
    I2C_SlaveIRQHandler(I2C_ID_0);
}

void I2C1_IRQHandler(void)
{
    I2C_SlaveIRQHandler(I2C_ID_1);
}

//////////////////////////////////////////////////////////////////////////////////////
void DBG_Do_Task_I2C(void)
{
    if(I2C_SCL_HOLD_FLG == 1)   // I2C SCL hold err
    {
        HAL_I2C_Init(I2C_ID_1);
    }

    RegMap[REG_KEY_ID_1] = ts.detect_key & 0xFF;
    RegMap[REG_KEY_ID_2] = (ts.detect_key>>8) & 0xFF;
    RegMap[REG_KEY_ID_3] = (ts.detect_key>>16) & 0xFF;

    // subaddress 0x10~
    user.dataBuff[0] = ts.raw_data[CH_00][0];
    user.dataBuff[1] = ts.raw_data[CH_01][0];
    user.dataBuff[2] = ts.raw_data[CH_02][0];
    user.dataBuff[3] = ts.raw_data[CH_03][0];
    user.dataBuff[4] = ts.raw_data[CH_04][0];
    user.dataBuff[5] = ts.raw_data[CH_05][0];
    user.dataBuff[6] = ts.raw_data[CH_06][0];
    
    user.dataBuff[7] = ts.baseline[CH_00][0];
    user.dataBuff[8] = ts.baseline[CH_01][0];
    user.dataBuff[9] = ts.baseline[CH_02][0];
    user.dataBuff[10] = ts.baseline[CH_03][0];
    user.dataBuff[11] = ts.baseline[CH_04][0];
    user.dataBuff[12] = ts.baseline[CH_05][0];
    user.dataBuff[13] = ts.baseline[CH_06][0];
    
    user.dataBuff[14] = ts.diff_data_filter[CH_00];
    user.dataBuff[15] = ts.diff_data_filter[CH_01];
    user.dataBuff[16] = ts.diff_data_filter[CH_02];
    user.dataBuff[17] = ts.diff_data_filter[CH_03];
    user.dataBuff[18] = ts.diff_data_filter[CH_04];
    user.dataBuff[19] = ts.diff_data_filter[CH_05];
    user.dataBuff[20] = ts.diff_data_filter[CH_06];
}

#endif

