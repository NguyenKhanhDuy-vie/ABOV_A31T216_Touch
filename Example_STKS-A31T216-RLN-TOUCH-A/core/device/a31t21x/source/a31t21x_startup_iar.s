;/******************************************************************************
; * @file     startup_a31t21x.s
; * @brief    CMSIS Core Device Startup File for A31T21x
; * @author   AE Team, ABOV Semiconductor Co., Ltd.
; * @version  V0.0.1
; * @date     30. Jul. 2018
; *
; * @note
; *
; ******************************************************************************/
;/* Copyright (c) 2011 - 2012 ARM LIMITED
;
;   All rights reserved.
;   Redistribution and use in source and binary forms, with or without
;   modification, are permitted provided that the following conditions are met:
;   - Redistributions of source code must retain the above copyright
;     notice, this list of conditions and the following disclaimer.
;   - Redistributions in binary form must reproduce the above copyright
;     notice, this list of conditions and the following disclaimer in the
;     documentation and/or other materials provided with the distribution.
;   - Neither the name of ARM nor the names of its contributors may be used
;     to endorse or promote products derived from this software without
;     specific prior written permission.
;   *
;   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
;   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
;   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
;   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
;   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
;   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
;   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
;   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
;   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
;   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
;   POSSIBILITY OF SUCH DAMAGE.
;   ---------------------------------------------------------------------------*/
;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/


        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)
        EXTERN  SystemInit
        EXTERN  __iar_program_start
        PUBLIC  __vector_table

        DATA
__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler                    ; Reset Handler
        DCD     NMI_Handler                      ; NMI Handler
        DCD     HardFault_Handler                ; Hard Fault Handler
        DCD     0                                ; Reserved
        DCD     0                                ; Reserved
        DCD     0                                ; Reserved
        DCD     0                                ; Reserved
        DCD     0                                ; Reserved
        DCD     0                                ; Reserved
        DCD     0                                ; Reserved
        DCD     SVC_Handler                      ; SVCall Handler
        DCD     0                                ; Reserved
        DCD     0                                ; Reserved
        DCD     PendSV_Handler                   ; PendSV Handler
        DCD     SysTick_Handler                  ; SysTick Handler

; External Interrupts
        DCD     LVI_IRQHandler                   ; #0 LVI Interrupt
        DCD     SYSCLKFAIL_IRQHandler            ; #1 System Clock Fail
        DCD     WDT_IRQHandler                   ; #2 Watch-dog Timer Interrupt
        DCD     GPIOAB_IRQHandler                ; #3 GPIO A, B Interrupt
        DCD     GPIOCD_IRQHandler                ; #4 GPIO C, D Interrupt
        DCD     GPIOE_IRQHandler                 ; #5 GPIO E Interrupt
        DCD     GPIOF_IRQHandler                 ; #6 GPIO F Interrupt
        DCD     TIMER10_IRQHandler               ; #7 16-bit timer 10 Interrupt
        DCD     TIMER11_IRQHandler               ; #8 16-bit timer 11 Interrupt
        DCD     TIMER12_IRQHandler               ; #9 16-bit timer 12 Interrupt
        DCD     I2C0_IRQHandler                  ; #10 I2C 0 Interrupt
        DCD     USART10_IRQHandler               ; #11 USART10 Interrupt
        DCD     WT_IRQHandler                    ; #12 Watch-Timer Interrupt
        DCD     TIMER30_IRQHandler               ; #13 32-bit timer 30 Interrupt
        DCD     I2C1_IRQHandler                  ; #14 I2C 1 Interrupt
        DCD     TIMER20_IRQHandler               ; #15 32-bit Timer 20 Interrupt
        DCD     TIMER21_IRQHandler               ; #16 32-bit Timer 21 Interrupt
        DCD     USART11_IRQHandler               ; #17 USART 11 Interrupt
        DCD     ADC_IRQHandler                   ; #18 ADC Interrupt
        DCD     UART0_IRQHandler                 ; #19 UART 0 Interrupt
        DCD     UART1_IRQHandler                 ; #20 UART 1 Interrupt
        DCD     TIMER13_IRQHandler               ; #21 16-bit Timer 13 Interrupt
        DCD     NULL_IRQHandler                  ; #22 Reserved
        DCD     NULL_IRQHandler                  ; #23 Reserved
        DCD     NULL_IRQHandler                  ; #24 Reserved
        DCD     SPI20_IRQHandler                 ; #25 SPI20 Interrupt
        DCD     SPI21_IRQHandler                 ; #26 SPI21 Interrupt
        DCD     TSENSE_IRQHandler                ; #27 Temp Sensor Interrupt
        DCD     LED_IRQHandler                   ; #28 LED Driver Interrupt
        DCD     TOUCH_IRQHandler                 ; #29 Touch Interrupt
        DCD     NULL_IRQHandler                  ; #30 Reserved
        DCD     CRC_IRQHandler                   ; #31 CRC Interrupt


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB
        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler
        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        B NMI_Handler

        PUBWEAK HardFault_HandlerC
        SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_HandlerC
        B .

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
HardFault_Handler
        MOVS    R0, #4
        MOV     R1, LR
        TST     R0, R1
        BEQ     __HardFault_MSP
        MRS     R0, PSP
        LDR     R1,=HardFault_HandlerC
        BX      R1
__HardFault_MSP
        MRS     R0, MSP
        LDR     R1,=HardFault_HandlerC
        BX      R1

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B SVC_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B SysTick_Handler

; External Interrupt Handlers.
        PUBWEAK LVI_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
LVI_IRQHandler
        B LVI_IRQHandler

        PUBWEAK SYSCLKFAIL_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SYSCLKFAIL_IRQHandler
        B SYSCLKFAIL_IRQHandler

        PUBWEAK WDT_IRQHandlerC
        SECTION .text:CODE:REORDER:NOROOT(1)
WDT_IRQHandlerC
        B .

        PUBWEAK WDT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
WDT_IRQHandler
        MOVS    R0, #4
        MOV     R1, LR
        TST     R0, R1
        BEQ     __WDT_MSP
        MRS     R0, PSP
        LDR     R1,=WDT_IRQHandlerC
        BX      R1
__WDT_MSP
        MRS     R0, MSP
        LDR     R1,=WDT_IRQHandlerC
        BX      R1

        PUBWEAK GPIOAB_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOAB_IRQHandler
        B GPIOAB_IRQHandler

        PUBWEAK GPIOCD_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOCD_IRQHandler
        B GPIOCD_IRQHandler

        PUBWEAK GPIOE_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOE_IRQHandler
        B GPIOE_IRQHandler

        PUBWEAK GPIOF_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOF_IRQHandler
        B GPIOF_IRQHandler

        PUBWEAK TIMER10_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER10_IRQHandler
        B TIMER10_IRQHandler

        PUBWEAK TIMER11_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER11_IRQHandler
        B TIMER11_IRQHandler

        PUBWEAK TIMER12_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER12_IRQHandler
        B TIMER12_IRQHandler

        PUBWEAK I2C0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C0_IRQHandler
        B I2C0_IRQHandler

        PUBWEAK USART10_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
USART10_IRQHandler
        B USART10_IRQHandler

        PUBWEAK WT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
WT_IRQHandler
        B WT_IRQHandler

        PUBWEAK TIMER30_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER30_IRQHandler
        B TIMER30_IRQHandler

        PUBWEAK I2C1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C1_IRQHandler
        B I2C1_IRQHandler

        PUBWEAK TIMER20_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER20_IRQHandler
        B TIMER20_IRQHandler

        PUBWEAK TIMER21_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER21_IRQHandler
        B TIMER21_IRQHandler

        PUBWEAK USART11_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
USART11_IRQHandler
        B USART11_IRQHandler

        PUBWEAK ADC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
ADC_IRQHandler
        B ADC_IRQHandler

        PUBWEAK UART0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART0_IRQHandler
        B UART0_IRQHandler

        PUBWEAK UART1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART1_IRQHandler
        B UART1_IRQHandler

        PUBWEAK TIMER13_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER13_IRQHandler
        B TIMER13_IRQHandler

        PUBWEAK SPI20_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPI20_IRQHandler
        B SPI20_IRQHandler

        PUBWEAK SPI21_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPI21_IRQHandler
        B SPI21_IRQHandler

        PUBWEAK TSENSE_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TSENSE_IRQHandler
        B TSENSE_IRQHandler

        PUBWEAK LED_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
LED_IRQHandler
        B LED_IRQHandler

        PUBWEAK TOUCH_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TOUCH_IRQHandler
        B TOUCH_IRQHandler

        PUBWEAK NULL_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
NULL_IRQHandler
        B NULL_IRQHandler

        PUBWEAK CRC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
CRC_IRQHandler
        B CRC_IRQHandler

        END
