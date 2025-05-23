/*
 * Control.c
 *
 *  Created on: May 9, 2025
 *      Author: dvixe
 */
#include "Control.h"


void SetCellPWM(uint8_t duty_percent)
{
    if (duty_percent > 100) duty_percent = 100;

    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(&htim1);
    uint32_t compare = duty_percent * (arr + 1) / 100;

    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, compare);

    if (duty_percent == 0)
    {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
        HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
        HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);
    }
    else
    {
        HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
        HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
    }
}

void BLOWER(uint8_t duty_percent){


	    if (duty_percent > 100) duty_percent = 100;
	    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, duty_percent * (__HAL_TIM_GET_AUTORELOAD(&htim3) + 1) / 100);

}

void Controlsystem(void){
	Read_ADC_Channels();
	static bool LoadFLAG;
    //float temperature = Convert_ADC_to_Temperature(adc_vals[0]);
	float CapacitorVoltage = Convert_ADC_to_CapacitorVoltage(adc_vals[3]);
	float CellVoltage = Convert_ADC_to_CellVoltage(adc_vals[1]);
	float CellCurrent = Convert_ADC_to_CellCurrent(adc_vals[2]);

	float POWER_CELL = CellVoltage * CellCurrent;


	if (CapacitorVoltage < CAPACITOR_LOW_LEVEL)
	{
		LoadFLAG = true;
	}
	else if (CapacitorVoltage > CAPACITOR_HIGH_LEVEL)
	{
		LoadFLAG = false;
	}

	 bool manualOverride = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0) == GPIO_PIN_SET;

	    if (LoadFLAG || manualOverride)
	    {

	        BLOWER(30);
	        SetCellPWM(100);
	        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET); // hydrogenvalve

	    }
	    else
	    {

	    	SetCellPWM(0);
	    	BLOWER(0);
	    	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);


	    }


}

void ControlsystemInit(void){

	SetCellPWM(0);
	BLOWER(0);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);

}
