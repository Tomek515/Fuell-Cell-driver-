/*
 * Parameterscheck.h
 *
 *  Created on: May 7, 2025
 *      Author: dvixe
 */

#ifndef INC_PARAMETERSCHECK_H_
#define INC_PARAMETERSCHECK_H_

#include "main.h"







#define ADC_MAX 4095.0f
#define VREF    3.3f



float Convert_ADC_to_Temperature(uint16_t adc_value);
float Convert_ADC_to_CapacitorVoltage(uint16_t adc_value);
float Convert_ADC_to_CellVoltage(uint16_t adc_value);
float Convert_ADC_to_CellCurrent(uint16_t adc_value);

#endif /* INC_PARAMETERSCHECK_H_ */
