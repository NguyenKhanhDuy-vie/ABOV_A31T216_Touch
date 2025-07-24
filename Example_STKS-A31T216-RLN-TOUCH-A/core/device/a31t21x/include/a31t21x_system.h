/**
*******************************************************************************
* @file         a31t21x_system.h
* @author       ABOV R&D Division
* @brief        System configuration Header
*
* Copyright 2017 ABOV Semiconductor Co.,Ltd. All rights reserved.
*
* This file is licensed under terms that are found in the LICENSE file
* located at Document directory.
* If this file is delivered or shared without applicable license terms,
* the terms of the BSD-3-Clause license shall be applied.
* Reference: https://opensource.org/licenses/BSD-3-Clause
******************************************************************************/


#ifndef _A31T21X_SYSTEM_H_
#define _A31T21X_SYSTEM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * Global Chipset ID macro
 */
#ifndef A31T21X
#define A31T21X
#endif

extern uint32_t SystemCoreClock;        /**< System Clock Frequency */
extern uint32_t SystemPeriClock;        /**< System Peripheral clock Frequency */
extern uint32_t SystemDelayCount;       /**< System delay count used for the delay */

/**
 *******************************************************************************
 * @brief       Setup the microcontroller system.
 *              Initialize the System.
 *              This function is normally called from the startup before
 *              program jumps to the application's main function.
 * @return      none
 ******************************************************************************/
void SystemInit(void);

/**
 *******************************************************************************
 * @brief       Delay using loop
 *              This function is based on _NOP instruction and does not provide
 *              accurate delay.
 * @param[in]   ms delay in millisecond
 * @return      none
 ******************************************************************************/
void SystemDelay(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif /* _A31T21X_SYSTEM_H_ */

/** @} */
/** @} */

