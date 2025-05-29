/*
 * Control.c
 *
 *  Created on: May 9, 2025
 *      Author: dvixe
 */
#include "Control.h"




void SetCellPWM(uint8_t duty_percent){
if (duty_percent > 100) duty_percent = 100;

   uint32_t pulse = duty_percent * (__HAL_TIM_GET_AUTORELOAD(&htim1) + 1) / 100;

   __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, pulse);
}
void BLOWER(uint8_t duty_percent){


	    if (duty_percent > 100) duty_percent = 100;
	    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, duty_percent * (__HAL_TIM_GET_AUTORELOAD(&htim3) + 1) / 100);

}

void Controlsystem(void){
	Read_ADC_Channels();

    //float temperature = Convert_ADC_to_Temperature(adc_vals[0]);
	float CapacitorVoltage = Convert_ADC_to_CapacitorVoltage(adc_vals[3]);
	float CellVoltage = Convert_ADC_to_CellVoltage(adc_vals[1]);
	float CellCurrent = Convert_ADC_to_CellCurrent(adc_vals[2]);
	pt100isOK = Max31865_readTempC(&pt100,&t);
	pt100Temp = Max31865_Filter(t,pt100Temp,0.1);
	static bool LoadFLAG;
	static bool previousLoadFlag;
	float POWER_CELL = CellVoltage * CellCurrent;


	if (CapacitorVoltage < CAPACITOR_LOW_LEVEL)
	{
		LoadFLAG = true;
	}
	else if (CapacitorVoltage > CAPACITOR_HIGH_LEVEL)
	{
		LoadFLAG = false;
	}

	 bool manualOverride = HAL_GPIO_ReadPin(MANUAL_GPIO_Port, MANUAL_Pin) == GPIO_PIN_SET;
	 bool controlactive = LoadFLAG || manualOverride;
	    if (controlactive)
	    {

	    	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);
	        BLOWER(100);
	        SetCellPWM(0);
	        HAL_GPIO_WritePin(H_VALVE_GPIO_Port, H_VALVE_Pin, GPIO_PIN_SET); // hydrogenvalve
	        HAL_GPIO_WritePin(HUMIDIFIER_GPIO_Port, HUMIDIFIER_Pin, GPIO_PIN_SET);

	    }
	    else
	    {

	    	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_RESET);
	    	SetCellPWM(100);
	    	BLOWER(0);
	    	HAL_GPIO_WritePin(H_VALVE_GPIO_Port, H_VALVE_Pin, GPIO_PIN_RESET);
	    	HAL_GPIO_WritePin(HUMIDIFIER_GPIO_Port, HUMIDIFIER_Pin, GPIO_PIN_RESET);

	    }


}

void ControlsystemInit(void){

	//SetCellPWM(0);
	BLOWER(0);
	HAL_GPIO_WritePin(H_VALVE_GPIO_Port, H_VALVE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(HUMIDIFIER_GPIO_Port, HUMIDIFIER_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PURGE_GPIO_Port, PURGE_Pin, GPIO_PIN_RESET);

}
