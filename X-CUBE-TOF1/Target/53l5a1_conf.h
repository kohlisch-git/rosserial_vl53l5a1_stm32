
/**
 ******************************************************************************
 * @file    53l5a1_conf.h
 * @author  IMG SW Application Team
 * @brief   This file contains definitions for the ToF components bus interfaces
 *          when using the X-NUCLEO-53L5A1 expansion board
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_bus.h"
#include "stm32f4xx_nucleo_errno.h"

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef VL53L5A1_CONF_H
#define VL53L5A1_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/*
 * the 53L5A1 BSP driver uses this symbol to allocate a structure for each device
 * if you are only using the on-board sensor without break-out boards
 * change its to (1U) in order to save space in RAM memory
 */
#define RANGING_SENSOR_INSTANCES_NBR    (3U)

#define VL53L5A1_hi2c                   (hi2c1)

#define VL53L5A1_I2C_SCL_GPIO_PORT      BUS_I2C1_SCL_GPIO_PORT
#define VL53L5A1_I2C_SCL_GPIO_PIN       BUS_I2C1_SCL_GPIO_PIN
#define VL53L5A1_I2C_SDA_GPIO_PORT      BUS_I2C1_SDA_GPIO_PORT
#define VL53L5A1_I2C_SDA_GPIO_PIN       BUS_I2C1_SDA_GPIO_PIN

#define VL53L5A1_I2C_Init               BSP_I2C1_Init
#define VL53L5A1_I2C_DeInit             BSP_I2C1_DeInit
#define VL53L5A1_I2C_WriteReg           BSP_I2C1_WriteReg16
#define VL53L5A1_I2C_ReadReg            BSP_I2C1_ReadReg16
#define VL53L5A1_GetTick                BSP_GetTick

#ifdef __cplusplus
}
#endif

#endif /* VL53L5A1_CONF_H*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

