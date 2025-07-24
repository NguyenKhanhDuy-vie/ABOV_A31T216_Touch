/**
*******************************************************************************
* @file         a31t21x_pwr.h
* @author       ABOV R&D Division
* @brief        A31T21x PWR Driver Header
*
* Copyright 2017 ABOV Semiconductor Co.,Ltd. All rights reserved.
*
* This file is licensed under terms that are found in the LICENSE file
* located at Document directory.
* If this file is delivered or shared without applicable license terms,
* the terms of the BSD-3-Clause license shall be applied.
* Reference: https://opensource.org/licenses/BSD-3-Clause
******************************************************************************/


#ifndef _A31T21X_PWR_H_
#define _A31T21X_PWR_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "a31t21x.h"
#include "a31t21x_driver_common.h"

/**
 * low voltage level selection
 */
enum pwr_lv_level
{
    PWR_LV_1_60V    = (0x0uL << 0),
    PWR_LV_1_69V    = (0x1uL << 0),
    PWR_LV_1_78V    = (0x2uL << 0),
    PWR_LV_1_90V    = (0x3uL << 0),
    PWR_LV_1_99V    = (0x4uL << 0),
    PWR_LV_2_12V    = (0x5uL << 0),
    PWR_LV_2_30V    = (0x6uL << 0),
    PWR_LV_2_47V    = (0x7uL << 0),
    PWR_LV_2_67V    = (0x8uL << 0),
    PWR_LV_3_04V    = (0x9uL << 0),
    PWR_LV_3_18V    = (0xAuL << 0),
    PWR_LV_3_59V    = (0xBuL << 0),
    PWR_LV_3_72V    = (0xCuL << 0),
    PWR_LV_4_03V    = (0xDuL << 0),
    PWR_LV_4_20V    = (0xEuL << 0),
    PWR_LV_4_48V    = (0xFuL << 0),
};

/**
 * low voltage interrupt configuration
 */
struct lvi_cfg
{
    enum pwr_lv_level   level;          /**< LVI level */
    uint8_t             irq_prio;       /**< interrupt priority */
};

/**
 * low voltage reset configuration
 */
struct lvr_cfg
{
    enum pwr_lv_level   level;          /**< LVR level */
};

/**
 * low voltage event type
 */
enum pwr_event
{
    PWR_EVENT_LVI,                      /**< low voltage interrupt */
};

/**
 *******************************************************************************
 * @brief       PWR interrupt callback function type
 * @param[in]   event event type ::pwr_event
 * @param[in]   context context provided during initialization
 ******************************************************************************/
typedef void (*pwr_irq_handler_t)(uint32_t event, void *context);

/**
 *******************************************************************************
 * @brief       Initialize LVI.
 *              By initializing, LVI gets enabled.
 * @param[in]   cfg configuration
 * @param[in]   handler callback function to receive event
 * @param[in]   context caller's context
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_LVI_Init(struct lvi_cfg *cfg,
                     pwr_irq_handler_t handler,
                     void *context);

/**
 *******************************************************************************
 * @brief       Deinitialize LVI
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_LVI_Deinit(void);

/**
 *******************************************************************************
 * @brief       Initialize LVR.
 *              By initializing, LVR gets enabled.
 * @param[in]   cfg configuration
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_LVR_Init(struct lvr_cfg *cfg);

/**
 *******************************************************************************
 * @brief       Deinitialize LVR
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_LVR_Deinit(void);

/**
 *******************************************************************************
 * @brief       SleepMode
 * @return      none
 ******************************************************************************/
void HAL_PWR_EnterSleepMode(void);

/**
 *******************************************************************************
 * @brief       DeepSleep Mode
 * @return      none
 ******************************************************************************/
void HAL_PWR_EnterDeepSleepMode(void);

/**
 *******************************************************************************
 * @brief       Power System Configure
 * @return      none
 ******************************************************************************/
void PWR_SYSTEM_Configure(uint8_t stop_sel, uint8_t aon_sel);


#ifdef __cplusplus
}
#endif

#endif /* _A31T21X_PWR_H_ */

/** @} */
/** @} */


