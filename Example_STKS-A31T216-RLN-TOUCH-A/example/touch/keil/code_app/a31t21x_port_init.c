/**
*******************************************************************************
* @file         a31t21x_port_init.c
* @author       ABOV R&D Division
* @brief        Application specific port configuration
*
* Copyright 2017 ABOV Semiconductor Co.,Ltd. All rights reserved.
*
* This file is licensed under terms that are found in the LICENSE file
* located at Document directory.
* If this file is delivered or shared without applicable license terms,
* the terms of the BSD-3-Clause license shall be applied.
* Reference: https://opensource.org/licenses/BSD-3-Clause
******************************************************************************/


#include "main.h"
#include "a31t21x_gpio.h"
#include "i2c.h"

void Port_Init(void)
{
		HAL_GPIO_Init();
}

void Port_Configure_CLKO(void)
{
    /* Configure CLKO - PD0 */
    HAL_GPIO_SetFunction(PORTD, PIN0, Pn_MUX_AF3);
    HAL_GPIO_SetMode(PORTD, PIN0, GPIO_MODE_FUNC);
	HAL_SCU_EnableClockOut(0);
}

void Port_Configure_I2C(uint8_t id)
{
    if(id == I2C_ID_0)
    {
    
        /* Configure I2C0 SCL pin to be used */
        HAL_GPIO_SetFunction(PORTD, PIN0, Pn_MUX_AF1);
        HAL_GPIO_SetMode(PORTD, PIN0, GPIO_MODE_FUNC);
        HAL_GPIO_SetPull(PORTD, PIN0, GPIO_PULL_UP);
        
        /* Configure I2C0 SDA pin to be used */
        HAL_GPIO_SetFunction(PORTD, PIN1, Pn_MUX_AF1);
        HAL_GPIO_SetMode(PORTD, PIN1, GPIO_MODE_FUNC);
        HAL_GPIO_SetPull(PORTD, PIN1, GPIO_PULL_UP);
    }
    else if(id == I2C_ID_1)
    {
        /* Configure I2C1 SCL pin to be used */
        HAL_GPIO_SetFunction(PORTA, PIN6, Pn_MUX_AF1);
        HAL_GPIO_SetMode(PORTA, PIN6, GPIO_MODE_FUNC);
        HAL_GPIO_SetPull(PORTA, PIN6, GPIO_PULL_UP);

        /* Configure I2C1 SDA pin to be used */
        HAL_GPIO_SetFunction(PORTA, PIN5, Pn_MUX_AF1);
        HAL_GPIO_SetMode(PORTA, PIN5, GPIO_MODE_FUNC);
        HAL_GPIO_SetPull(PORTA, PIN5, GPIO_PULL_UP);
    }
}

void Port_Configure_UART(void)
{
    /* Configure UART TXD pin to be used */
    HAL_GPIO_SetFunction(PORTB, PIN4, Pn_MUX_AF1);
    HAL_GPIO_SetMode(PORTB, PIN4, GPIO_MODE_FUNC);
    HAL_GPIO_SetPull(PORTB, PIN4, GPIO_PULL_UP);

    /* Configure UART RXD pin to be used */
    HAL_GPIO_SetFunction(PORTB, PIN5, Pn_MUX_AF1);
    HAL_GPIO_SetMode(PORTB, PIN5, GPIO_MODE_FUNC);
    HAL_GPIO_SetPull(PORTB, PIN5, GPIO_PULL_UP);
}

