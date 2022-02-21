/**
  ******************************************************************************
  * @file           : user_results.c
  * @brief          : Source for user_results.c file.
  *                   This file contains the function implementation
  ******************************************************************************
 *  Created on: Feb 19, 2022
 *      Author: niklas
  ******************************************************************************
  */
#include "user_results.h"

/**
 * @brief  vl53l5cx_get_result_user(): customized version of the function
 * vl53l5cx_get_result_user) in Drivers/BSP/Components/vl53l5cx/vl53l5cx.c
 * @details
 * Added additional diagnostic data at the end of the function
 */
int32_t vl53l5cx_get_result_user(VL53L5CX_Object_t *pObj, VL53L5CX_Result_User_t *pResult)
{
  int32_t ret;
  uint8_t i, j;
  uint8_t resolution;
  uint8_t target_status;
  VL53L5CX_ResultsData data;

  if ((pObj == NULL) || (pResult == NULL))
  {
    ret = VL53L5CX_INVALID_PARAM;
  }
  else if (vl53l5cx_get_resolution(&pObj->Dev, &resolution) != VL53L5CX_STATUS_OK)
  {
    ret = VL53L5CX_ERROR;
  }
  else if (vl53l5cx_get_ranging_data(&pObj->Dev, &data) != VL53L5CX_STATUS_OK)
  {
    ret = VL53L5CX_ERROR;
  }
  else
  {
    pResult->NumberOfZones = resolution;

    for (i = 0; i < resolution; i++)
    {
      pResult->ZoneResult[i].NumberOfTargets = data.nb_target_detected[i];

      for (j = 0; j < data.nb_target_detected[i]; j++)
      {
        pResult->ZoneResult[i].Distance[j] = data.distance_mm[(VL53L5CX_NB_TARGET_PER_ZONE * i) + j];

        /* return Ambient value if ambient rate output is enabled */
        if (pObj->IsAmbientEnabled == 1U)
        {
          /* apply ambient value to all targets in a given zone */
          pResult->ZoneResult[i].Ambient[j] = data.ambient_per_spad[i];
        }
        else
        {
          pResult->ZoneResult[i].Ambient[j] = 0;
        }

        /* return Signal value if signal rate output is enabled */
        if (pObj->IsSignalEnabled == 1U)
        {
          pResult->ZoneResult[i].Signal[j] =
            data.signal_per_spad[(VL53L5CX_NB_TARGET_PER_ZONE * i) + j];
        }
        else
        {
          pResult->ZoneResult[i].Signal[j] = 0;
        }

        target_status = data.target_status[(VL53L5CX_NB_TARGET_PER_ZONE * i) + j];
        pResult->ZoneResult[i].Status[j] = vl53l5cx_map_target_status_user(target_status);
        pResult->ZoneResult[i].Spads[j] = data.nb_spads_enabled[i];
        pResult->ZoneResult[i].Sigma[j] = data.range_sigma_mm[(VL53L5CX_NB_TARGET_PER_ZONE * i) + j];
        pResult->ZoneResult[i].Reflectance[j] = data.reflectance[(VL53L5CX_NB_TARGET_PER_ZONE * i) + j];
      }
    }

    ret = VL53L5CX_OK;
  }

  return ret;
}

/**
 * @brief  vl53l5cx_map_target_status_user(): copyof the function
 * vl53l5cx_map_target_status_user() in Drivers/BSP/Components/vl53l5cx/vl53l5cx.c
 * @details
 * Had to be copied since it is not accessible in the driver
 */
uint8_t vl53l5cx_map_target_status_user(uint8_t status)
{
  uint8_t ret;

  if ((status == 5U) || (status == 9U))
  {
    ret = 0U; /* ranging is OK */
  }
  else if (status == 0U)
  {
    ret = 255U; /* no update */
  }
  else
  {
    ret = status; /* return device status otherwise */
  }

  return ret;
}
