/**
 *******************************************************************************
 * @file        i2c.h
 * @author      ABOV R&D Division
 * @brief       A31T21x I2C Function Header For Application
 *
 * Copyright 2017 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#ifndef _I2C_H_
#define _I2C_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "a31t21x.h"
#include "a31t21x_driver_common.h"

/**
 * I2C instance id
 */
enum i2c_id
{
    I2C_ID_0    = 0,                        /**< I2C 0 */
    I2C_ID_1    = 1,                        /**< I2C 1 */
    I2C_ID_MAX  = 2,                        /**< I2C max */
};

/**
 * I2C master clock frequency
 */
enum i2c_freq
{
    I2C_FREQ_400KHZ = 0,                    /**< frequency 400 KHz */
    I2C_FREQ_200KHZ = 1,                    /**< frequency 200 KHz */
    I2C_FREQ_100KHZ = 2,                    /**< frequency 100 KHz */
};

/**
 * I2C event
 */
enum i2c_event
{
    I2C_EVENT_TRASMIT_DONE      = BIT(0),   /**< transmit is complete */
    I2C_EVENT_RECEIVE_DONE      = BIT(1),   /**< reception is complete */
    I2C_EVENT_MASTERSHIP_LOSS   = BIT(2),   /**< master detected another master */
    I2C_EVENT_NO_ACK            = BIT(3),   /**< no acknowledgement received */
    I2C_EVENT_HW_ERR            = BIT(4),   /**< hardware error */
};

/**
 *******************************************************************************
 * @brief       Initialize I2C.
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
void HAL_I2C_Init(enum i2c_id id);
void DBG_Do_Task_I2C(void);

extern uint8_t I2C_SCL_HOLD_FLG;

#ifdef __cplusplus
}
#endif

#endif /* _A31T21X_I2C_H_ */

/** @} */
/** @} */
