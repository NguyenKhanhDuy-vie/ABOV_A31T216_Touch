/**
*******************************************************************************
* @file         a31t21x_system.c
* @author       ABOV R&D Division
* @brief        System configuration Code
*
* Copyright 2017 ABOV Semiconductor Co.,Ltd. All rights reserved.
*
* This file is licensed under terms that are found in the LICENSE file
* located at Document directory.
* If this file is delivered or shared without applicable license terms,
* the terms of the BSD-3-Clause license shall be applied.
* Reference: https://opensource.org/licenses/BSD-3-Clause
******************************************************************************/


#include <stdint.h>
#include "a31t21x.h"

uint32_t SystemCoreClock;		/*!< System Clock Frequency - (Core Clock) HLCK */
uint32_t SystemPeriClock;		/*!< System Clock Frequency - (Peri Clock) PCLK */
uint32_t SystemDelayCount;

void SystemInit(void)
{
	/* Disable Global Interrupt */
	__disable_irq();

	/* WDT Disable */
	WDT->CR = (0x5A69<<16)
		|(0x25<<10)
		|(0x1A<<4);

	/* GPIO Access Enable */
	PORTEN->EN = 0x15;
	PORTEN->EN = 0x51;

	/* Flash Access Time Configure */
	FMC->MR = 0x81;
	FMC->MR = 0x28;
	FMC->CFG = (0x7858 << 16) | (3 << 8);		// Flash Access in 4 cycles (3-wait)
	FMC->MR = 0;
}

void SystemDelay(uint32_t ms)
{
	uint32_t i;

	for(i=0; i<(SystemDelayCount*ms); i++)
	{
		__NOP();
	}
}

