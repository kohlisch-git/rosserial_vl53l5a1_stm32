/**
  ******************************************************************************
  * @file           : user_results.h
  * @brief          : Header for user_results.c file.
  *                   This file contains the definitions
  ******************************************************************************
 *  Created on: Feb 19, 2022
 *      Author: niklas
  ******************************************************************************
  */

#ifndef INC_USER_RESULTS_H_
#define INC_USER_RESULTS_H_

#ifdef __cplusplus
extern "C" {
#endif
/* import api */
#include "vl53l5cx_api.h"
#include "inttypes.h"
#include "53l5a1_ranging_sensor.h"
#include "53l5a1_conf.h"
#include "vl53l5cx_api.h"

/* @biref Custom structure for measurement results per cell */
typedef struct
{
  uint8_t NumberOfTargets;
  uint16_t Distance[VL53L5CX_NB_TARGET_PER_ZONE]; /*!< millimeters */
  uint8_t Status[VL53L5CX_NB_TARGET_PER_ZONE];   /*!< OK: 0, NOK: !0 */
  uint32_t Ambient[VL53L5CX_NB_TARGET_PER_ZONE];   /*!< kcps / spad */
  uint32_t Signal[VL53L5CX_NB_TARGET_PER_ZONE];    /*!< kcps / spad */
  uint32_t Spads[VL53L5CX_NB_TARGET_PER_ZONE];
  uint16_t Sigma[VL53L5CX_NB_TARGET_PER_ZONE];
  uint8_t Reflectance[VL53L5CX_NB_TARGET_PER_ZONE];
} VL53L5CX_ZoneResult_User_t;

/* @biref Custom structure for measurement results */
typedef struct
{
  uint32_t NumberOfZones;
  VL53L5CX_ZoneResult_User_t ZoneResult[VL53L5CX_MAX_NB_ZONES];
} VL53L5CX_Result_User_t;

/* function prototypes */
int32_t vl53l5cx_get_result_user(VL53L5CX_Object_t *pObj, VL53L5CX_Result_User_t *pResult);
uint8_t vl53l5cx_map_target_status_user(uint8_t status);

#ifdef __cplusplus
}
#endif

#endif /* INC_USER_RESULTS_H_ */
