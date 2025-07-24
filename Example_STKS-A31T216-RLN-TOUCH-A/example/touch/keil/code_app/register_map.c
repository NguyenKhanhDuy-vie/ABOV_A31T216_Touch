/**
 *******************************************************************************
 * @file        register_map.c
 * @author      ABOV R&D Division
 * @brief       A31T21x I2C debug register map code
 *
 * Copyright 2017 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#include "register_map.h"
#include "user_function.h"

#if (I2C_DEBUG_EN == 1)
uint8_t RegMap[REG_MAX];
#endif
