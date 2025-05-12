/*
 * measurments.c
 *
 *  Created on: May 7, 2025
 *      Author: dvixe
 */

#ifndef SRC_MEASURMENTS_C_
#define SRC_MEASURMENTS_C_
#include "measurments.h"
extern ADC_HandleTypeDef hadc1;

float adc_vals[4];

void Read_ADC_Channels(void)
{
    HAL_ADC_Start(&hadc1);

    for (int i = 0; i < 4; i++)
    {
        HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
        adc_vals[i] = HAL_ADC_GetValue(&hadc1);
    }

    HAL_ADC_Stop(&hadc1);
}

#endif /* SRC_MEASURMENTS_C_ */
