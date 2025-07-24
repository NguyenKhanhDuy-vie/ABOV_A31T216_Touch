/**
*******************************************************************************
* @file         a31t21x_scu.c
* @author       ABOV R&D Division
* @brief        A31T21x SCU Driver Code
*
* Copyright 2017 ABOV Semiconductor Co.,Ltd. All rights reserved.
*
* This file is licensed under terms that are found in the LICENSE file
* located at Document directory.
* If this file is delivered or shared without applicable license terms,
* the terms of the BSD-3-Clause license shall be applied.
* Reference: https://opensource.org/licenses/BSD-3-Clause
******************************************************************************/


#include "a31t21x_scu.h"


struct scu_ctrl_block
{
    scu_clk_fail_handler_t          mclk_handler;
    void *                          mclk_context;
    scu_clk_fail_handler_t          lse_handler;
    void *                          lse_context;
    scu_clk_fail_handler_t          hse_handler;
    void *                          hse_context;
    uint8_t                         pll_configured;
    enum scu_clock                  mclk;
    enum scu_pll_clock_src          pclk_src;
};

static struct scu_ctrl_block g_scb =
{
    .mclk_handler = NULL,
    .mclk_context = NULL,
    .lse_handler = NULL,
    .lse_context = NULL,
    .hse_handler = NULL,
    .hse_context = NULL,
    .pll_configured = 0,
};

static void SCU_FlashLatencySet(uint32_t hclk)
{
    if (hclk <= 20000000)
    {
				FMC->MR = 0x81;
				FMC->MR = 0x28;
				FMC->CFG = (0x7858 << 16) | (0 << 8);		// Flash Access in 1 cycles (0-wait)
				FMC->MR = 0;
    }
    else if (hclk <= 40000000)
    {
				FMC->MR = 0x81;
				FMC->MR = 0x28;
				FMC->CFG = (0x7858 << 16) | (1 << 8);		// Flash Access in 2 cycles (1-wait)
				FMC->MR = 0;
    }
    else if (hclk <= 48000000)
    {
				FMC->MR = 0x81;
				FMC->MR = 0x28;
				FMC->CFG = (0x7858 << 16) | (2 << 8);		// Flash Access in 3 cycles (2-wait)
				FMC->MR = 0;
    }
    else
    {
				FMC->MR = 0x81;
				FMC->MR = 0x28;
				FMC->CFG = (0x7858 << 16) | (3 << 8);		// Flash Access in 4 cycles (3-wait)
				FMC->MR = 0;
    }
}

static void SCU_ClockEnable(enum scu_clock mclk_sel, enum scu_clock_div mclk_div)
{
		uint32_t reg_val;
		uint32_t delay_1ms;
		volatile uint32_t i;

		delay_1ms = 21*(SystemPeriClock/500000);
		
		switch (mclk_sel)
    {
        case SCU_CLOCK_LSI:
						reg_val = SCU->CSCR;
						reg_val &= ~(0x0F << 8);
						reg_val |= (mclk_div << 8);
						SCU->CSCR = reg_val | (0xA507UL << 16);

						for(i=0;i<delay_1ms;i++);
				
						reg_val = SCU->SCCR;
						reg_val &= ~(0x07 << 0);
						reg_val |= mclk_sel;
						SCU->SCCR = (0x570AUL << 16) | reg_val;
            break;
        case SCU_CLOCK_LSE:
						reg_val = SCU->CSCR;
						reg_val &= ~(0x0F << 12);
						reg_val |= (mclk_div << 12);
						SCU->CSCR = reg_val | (0xA507UL << 16);

						for(i=0;i<delay_1ms*500;i++);
				
						reg_val = SCU->SCCR;
						reg_val &= ~(0x07 << 0);
						reg_val |= mclk_sel;
						SCU->SCCR = (0x570AUL << 16) | reg_val;
            break;
        case SCU_CLOCK_HSI:
						reg_val = SCU->CSCR;
						reg_val &= ~(0x0F << 4);
						reg_val |= (mclk_div << 4);
						SCU->CSCR = reg_val | (0xA507UL << 16);

						for(i=0;i<delay_1ms;i++);
				
						reg_val = SCU->SCCR;
						reg_val &= ~(0x07 << 0);
						reg_val |= mclk_sel;
						SCU->SCCR = (0x570AUL << 16) | reg_val;
            break;
        case SCU_CLOCK_HSE:
						reg_val = SCU->CSCR;
						reg_val &= ~(0x0F << 0);
						reg_val |= (mclk_div << 0);
						SCU->CSCR = reg_val | (0xA507UL << 16);

						for(i=0;i<delay_1ms*10;i++);

						reg_val = SCU->SCCR;
						reg_val &= ~(0x07 << 0);
						reg_val |= mclk_sel;
						SCU->SCCR = (0x570AUL << 16) | reg_val;
            break;
        case SCU_CLOCK_PLL:
						if(g_scb.pclk_src == SCU_PLL_CLOCK_SRC_HSI)
						{
							reg_val = SCU->SCCR;
							reg_val &= ~(0x07 << 0);
							reg_val |= 0x03;
							SCU->SCCR = (0x570AUL << 16) | reg_val;
						}
						else
						{
							reg_val = SCU->SCCR;
							reg_val &= ~(0x07 << 0);
							reg_val |= mclk_sel;
							SCU->SCCR = (0x570AUL << 16) | reg_val;
						}
            break;
        default:
            DRIVER_ASSERT(0);
            break;
    }
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_SCU_UpdateMainClock(struct scu_clock_cfg *cfg)
{
		/* Check PLL parameter is ready */
    if (cfg->mclk_sel == SCU_CLOCK_PLL && g_scb.pll_configured != 1)
    {
        DRIVER_ASSERT(0);
    }


		/* Set SystemClock Parameter */
    g_scb.mclk = cfg->mclk_sel;
    SystemCoreClock = cfg->mclk_hz;
    SystemPeriClock = cfg->mclk_hz;
    SystemDelayCount = (uint32_t)(((uint32_t)SystemCoreClock * 18) / 1000000);

		/* Clock Source Enable then Change System Clock */
		SCU_ClockEnable(cfg->mclk_sel, cfg->mclk_div);
		SCU_FlashLatencySet(100000000);

    return DRIVER_ERROR_OK;
}

int32_t HAL_SCU_SetPLLConfig(struct scu_pll_clock_cfg *cfg)
{
		uint32_t startup_counter = 0; 
		uint32_t reg_val;
		//uint32_t delay_1ms;

		volatile uint32_t i;

		//delay_1ms = 21*(SystemPeriClock/500000);
	
		/* Before PLL Run, Change SystemClock to HSE */
		if(cfg->pclk_src == SCU_PLL_CLOCK_SRC_HSE)
		{
			reg_val = SCU->CSCR;
			reg_val &= ~(0xFF << 0);
			reg_val |= (0x88 << 0);
			SCU->CSCR = reg_val | (0xA507UL << 16);	// Enable Both HSI and HSE
		
			//for(i=0;i<delay_1ms*10;i++);
			/* stable time */
			for(i=0;i<100;i++);
			
			reg_val = SCU->SCCR;
			reg_val &= ~(0x07 << 0);
			reg_val |= 0x06;
			SCU->SCCR = (0x570AUL << 16) | reg_val;	// Change SystemClock to HSE
		}
		/* Before PLL Run, Change SystemClock to HSI */
		else
		{
			reg_val = SCU->CSCR;
			reg_val &= ~(0xFF << 0);
			reg_val |= (0xA8 << 0);
			SCU->CSCR = reg_val | (0xA507UL << 16);	// Enable Both HSI and HSE

//			for(i=0;i<delay_1ms*10;i++);
				/* stable time */
			for(i=0;i<100;i++);

			reg_val = SCU->SCCR;
			reg_val &= ~(0x07 << 0);
			reg_val |= 0x02;
			SCU->SCCR = (0x570AUL << 16) | reg_val;	// Change SystemClock to HSI
		}

		/* Configure PLL */
			SCU->PLLCON=(1<<22);     // PLL reset is negated		
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP();    // Delay for stabilization		
		
		/* Configure PLL */
		SCU->PLLCON=0
		|(6<<24)	// CTRLOPT  01:Current option = 10uA / 10:VCO bias = x1
		|(1<<23)   // PLL reset is asserted
		|(1<<22)  // PLLEn		
		|(1<<21)     //BYPASS 0:FOUT is bypassed as FIN, 1:FOUT is PLL output
		|((cfg->p_mode & 0x1) << 20)		// PLLMODE	(D)		// 0: VCO is the same with FOUT , 1: VCO frequency is x2 of FOUT
		|((cfg->pre_div & 0x7) << 16)		// PREDIV		(R)		// FIN = PLLINCLK / (R+1) // Recommanded FIN = 2MHz
		|((cfg->post_div1 & 0xff) << 8)	// POSTDIV1	(N1)	// Fvco = FIN * (N1+1) // Fvco <= 200MHz
		|((cfg->post_div2 & 0xf) << 4)	// POSTDIV2	(N2)
		|((cfg->out_div & 0xf) << 0)		// OUTDIV		(P)
			;
		
		/* wait over 500us */
		startup_counter = PLL_STARTUP_TIMEOUT;
		while(--startup_counter);
		/* Check the PLLCON PLLLOCK State */
		while((SCU->PLLCON & (1UL<<31)) == 0 )
			__NOP();
		/* Check the PLLCON PLLRDY State */		
		while((SCU->PLLCON & (1UL<<30)) == 0 )
			__NOP();
		
		/* Set global parameter */
    g_scb.pll_configured = 1;
    g_scb.pclk_src = cfg->pclk_src;
	
		/* Check pll timeout */
		return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_SCU_EnableClock(enum scu_clock clk)
{
		uint32_t reg_val;

		/* Check alredy clock enable */
    if (clk == g_scb.mclk)
    {
        return DRIVER_ERROR_OK;
    }

		/* Enable clock source */
		reg_val = SCU->CSCR;
    switch (clk)
    {
        case SCU_CLOCK_LSI:
						reg_val &= ~(0x0F << 8);
						reg_val |= (0x08 << 8);
            break;
        case SCU_CLOCK_LSE:
						reg_val &= ~(0x0F << 12);
						reg_val |= (0x08 << 12);
            break;
        case SCU_CLOCK_HSI:
						reg_val &= ~(0x0F << 4);
						reg_val |= (0x08 << 4);
            break;
        case SCU_CLOCK_HSE:
						reg_val &= ~(0x0F << 0);
						reg_val |= (0x08 << 0);
            break;
        default:
            DRIVER_ASSERT(0);
            break;	
    }
		SCU->CSCR = reg_val | (0xA507UL << 16);
		
    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_SCU_DisableClock(enum scu_clock clk)
{
		uint32_t reg_val;

		/* Disable clock source */
		reg_val = SCU->CSCR;
    switch (clk)
    {
        case SCU_CLOCK_LSI:
						reg_val &= ~(0x0F << 8);
            break;
        case SCU_CLOCK_LSE:
						reg_val &= ~(0x0F << 12);
            break;
        case SCU_CLOCK_HSI:
						reg_val &= ~(0x0F << 4);
            break;
        case SCU_CLOCK_HSE:
						reg_val &= ~(0x0F << 0);
            break;
        default:
            DRIVER_ASSERT(0);
            break;	
    }
		SCU->CSCR = reg_val | (0xA507UL << 16);
		
    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_SCU_EnableClockMonitor(enum scu_clock_mon clk,
                                   scu_clk_fail_handler_t handler,
                                   void *context,
                                   uint8_t irq_prio)
{
    switch (clk)
    {
        case SCU_MON_CLOCK_MCLK:
            SCU->CMR |= (3 << SCU_MON_CLOCK_MCLK);	// Enable mclk monitoring & interrupt
            g_scb.mclk_handler = handler;
            g_scb.mclk_context = context;
            break;
        case SCU_MON_CLOCK_LSE:
            SCU->CMR |= (3 << SCU_MON_CLOCK_LSE);		// Enable lse monitoring & interrupt
            g_scb.lse_handler = handler;
            g_scb.lse_context = context;
            break;
        case SCU_MON_CLOCK_HSE:
						SCU->CMR |= (3 << SCU_MON_CLOCK_HSE);		// Enable hse monitoring & interrupt
            g_scb.hse_handler = handler;
            g_scb.hse_context = context;
            break;
    }

		/* Set NVIC */
		NVIC_ClearPendingIRQ(SYSCLKFAIL_IRQn);
		NVIC_SetPriority(SYSCLKFAIL_IRQn, irq_prio);
		NVIC_EnableIRQ(SYSCLKFAIL_IRQn);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_SCU_DisableClockMonitor(enum scu_clock_mon clk)
{
    switch (clk)
    {
        case SCU_MON_CLOCK_MCLK:
            SCU->CMR &= ~(3 << SCU_MON_CLOCK_MCLK);	// Disable mclk monitoring & interrupt
            break;
        case SCU_MON_CLOCK_LSE:
            SCU->CMR &= ~(3 << SCU_MON_CLOCK_LSE);	// Disable lse monitoring & interrupt
            break;
        case SCU_MON_CLOCK_HSE:
						SCU->CMR &= ~(3 << SCU_MON_CLOCK_HSE);	// Disable hse monitoring & interrupt
            break;
    }

		/* Clear NVIC */
    if ((SCU->CMR) == 0)
    {
        NVIC_DisableIRQ(SYSCLKFAIL_IRQn);
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_SCU_EnableClockOut(uint8_t clk_div)
{
		uint32_t reg_val;
	
		reg_val = SCU->COR;
		reg_val &= ~(0x1F << 0);

		reg_val |= ((clk_div << 0) & 0x0F);
		SCU->COR = (1 << 4)| reg_val;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_SCU_DisableClockOut(void)
{
    SCU->COR &= ~(1 << 4);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_SCU_EnableWakeupSource(uint32_t src)
{
    SCU->WUER |= src;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_SCU_DisableWakeupSource(uint32_t src)
{
    SCU->WUER &= ~src;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_SCU_EnableResetSource(uint32_t src)
{
    SCU->RSER |= src;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_SCU_DisableResetSource(uint32_t src)
{
    SCU->RSER &= src;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_SCU_GetResetReason(uint32_t *reason)
{
    uint32_t value;

    value = SCU->RSSR;
    *reason = value;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_SCU_GetWakeupReason(uint32_t *reason)
{
    uint32_t value;

    value = SCU->WUSR;
    *reason = value;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_SCU_Timer1n_ClockConfig(uint32_t t1nclk)
{
		uint32_t reg_val;

		reg_val = SCU->PPCLKSR;
		reg_val &= ~(1 << 22);
		reg_val |= (t1nclk << 22);
		SCU->PPCLKSR = reg_val;

    return DRIVER_ERROR_OK;
}

int32_t HAL_SCU_Timer20_ClockConfig(uint32_t t20clk)
{
		uint32_t reg_val;
	
		reg_val = SCU->PPCLKSR;
		reg_val &= ~(1<<20);
		reg_val |= (t20clk<<20);
		SCU->PPCLKSR = reg_val;	      

    return DRIVER_ERROR_OK;
}

int32_t HAL_SCU_Timer30_ClockConfig(uint32_t t30clk)
{
		uint32_t reg_val;
	
		reg_val = SCU->PPCLKSR;
		reg_val &= ~(1<<17);
		reg_val |= (t30clk<<17);
		SCU->PPCLKSR = reg_val;	      

    return DRIVER_ERROR_OK;
}

int32_t HAL_SCU_LED_ClockConfig(uint32_t ledclk)
{
		uint32_t reg_val;
	
		reg_val = SCU->PPCLKSR;
		reg_val &= ~(1<<10);
		reg_val |= (ledclk<<10);
		SCU->PPCLKSR = reg_val;	      

    return DRIVER_ERROR_OK;
}

int32_t HAL_SCU_WT_ClockConfig(uint32_t wtclk)
{
		uint32_t reg_val;
	
		reg_val = SCU->PPCLKSR;
		reg_val &= ~(3<<3);
		reg_val |= (wtclk<<3);
		SCU->PPCLKSR = reg_val;	      

    return DRIVER_ERROR_OK;
}

int32_t HAL_SCU_WDT_ClockConfig(uint32_t wdtclk)
{
		uint32_t reg_val;
	
		reg_val = SCU->PPCLKSR;
		reg_val &= ~(1<<0);
		reg_val |= (wdtclk<<0);
		SCU->PPCLKSR = reg_val;	      

    return DRIVER_ERROR_OK;
}

int32_t HAL_SCU_LCD_ClockConfig(uint32_t lcdclk)
{
		uint32_t reg_val;

		reg_val = SCU->PPCLKSR;
		reg_val &= ~(1<<6);
		reg_val |= (lcdclk<<6);
		SCU->PPCLKSR = reg_val;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
void SYSCLKFAIL_IRQHandler(void)
{
		/* Clear Clock Fail Flags */
		SCU->CMR |= 1 << 5;	// Clear MCLK Fail
		SCU->CMR |= 1 << 1;	// Clear HSE Fail
		SCU->CMR |= 1 << 9;	// Clear LSE Fail


    if (g_scb.mclk_handler)
    {
        g_scb.mclk_handler(SCU_CLOCK_FAIL_EVENT_MCLK, g_scb.mclk_context);
    }
		else if (g_scb.hse_handler)
    {
        g_scb.hse_handler(SCU_CLOCK_FAIL_EVENT_HSE, g_scb.hse_context);
    }
		else if (g_scb.lse_handler)
    {
        g_scb.lse_handler(SCU_CLOCK_FAIL_EVENT_HSE, g_scb.lse_context);
    }
}

