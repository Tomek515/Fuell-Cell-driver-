/*
 * Parameterscheck.c
 *
 *  Created on: May 9, 2025
 *      Author: dvixe
 */

#include "Parameterscheck.h"


//float Convert_ADC_to_Temperature(uint16_t adc_value){

//}
#define CAPACITOR_DIVIDER 16.0f
float Convert_ADC_to_CapacitorVoltage(uint16_t adc_value){
	float CapacitorVoltage=((adc_value / ADC_MAX) * VREF)*CAPACITOR_DIVIDER;
	return CapacitorVoltage;
}


#define CELL_DIVIDER 9.5f
float Convert_ADC_to_CellVoltage(uint16_t adc_value){
	float CellVoltage=((adc_value / ADC_MAX) * VREF)*CELL_DIVIDER;
	return CellVoltage;
}

#define ACS_SENS 0.0203f
float Convert_ADC_to_CellCurrent(uint16_t adc_value){
	float voltage = ((adc_value / ADC_MAX) * VREF);
	float CellCurrent = (1.65f-voltage) / ACS_SENS;
	return CellCurrent;
}



