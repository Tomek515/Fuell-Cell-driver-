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



static bool LoadFLAG = false;
static bool previousLoadFlag = false;
static bool softStartActive = false;
static uint8_t soft_pwm = 100;
static bool safetyLockout = false;
#define ALPHA 0.3f


float CapacitorVoltage = 25.0f;
float CellVoltage = 0.0f;
float CellCurrent = 0.0f;
void Controlsystem(void){
	Read_ADC_Channels();

    //float temperature = Convert_ADC_to_Temperature(adc_vals[0]);
	float rawCapacitorVoltage = Convert_ADC_to_CapacitorVoltage(adc_vals[3]);
	float rawCellVoltage = Convert_ADC_to_CellVoltage(adc_vals[1]);
	float rawCellCurrent = Convert_ADC_to_CellCurrent(adc_vals[2]);

	CapacitorVoltage = ALPHA * rawCapacitorVoltage + (1.0f - ALPHA) * CapacitorVoltage;
	CellVoltage = ALPHA * rawCellVoltage + (1.0f - ALPHA) * CellVoltage;
	CellCurrent = ALPHA * rawCellCurrent + (1.0f - ALPHA) * CellCurrent;

	pt100isOK = Max31865_readTempC(&pt100,&t);
	pt100Temp = Max31865_Filter(t,pt100Temp,0.1);

	bool manualOverride = HAL_GPIO_ReadPin(MANUAL_GPIO_Port, MANUAL_Pin) == GPIO_PIN_SET;

	float POWER_CELL = CellVoltage * CellCurrent;

	if (pt100Temp > MAX_SAFE_TEMP || POWER_CELL > MAX_SAFE_POWER) {
	    safetyLockout = true;
	} else {
	    safetyLockout = false;
	}

	if (CapacitorVoltage < CAPACITOR_LOW_LEVEL)
	{
		LoadFLAG = true;
	}
	else if (CapacitorVoltage > CAPACITOR_HIGH_LEVEL)
	{
		LoadFLAG = false;
	}
	bool controlactive = (LoadFLAG || manualOverride) && !safetyLockout;
	if (controlactive && !previousLoadFlag) {
	    softStartActive = true;
	    soft_pwm = 100;

	    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);
	    HAL_GPIO_WritePin(H_VALVE_GPIO_Port, H_VALVE_Pin, GPIO_PIN_SET);
	    //HAL_GPIO_WritePin(HUMIDIFIER_GPIO_Port, HUMIDIFIER_Pin, GPIO_PIN_SET);
	    HAL_GPIO_WritePin(PURGE_GPIO_Port, PURGE_Pin, GPIO_PIN_SET);

	    BLOWER(100);
	}
	previousLoadFlag = controlactive;

	    if (controlactive) {
	        if (softStartActive) {
	            if (soft_pwm > 0) {
	                soft_pwm -= 4;
	                SetCellPWM(soft_pwm);
	                HAL_Delay(1);
	            } else {
	                softStartActive = false;
	            }
	        } else {
	            SetCellPWM(0);
	        }
	    } else {
	        SetCellPWM(100);
	        BLOWER(0);
	        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_RESET);
	        HAL_GPIO_WritePin(H_VALVE_GPIO_Port, H_VALVE_Pin, GPIO_PIN_RESET);
	        //HAL_GPIO_WritePin(HUMIDIFIER_GPIO_Port, HUMIDIFIER_Pin, GPIO_PIN_RESET);
		    HAL_GPIO_WritePin(PURGE_GPIO_Port, PURGE_Pin, GPIO_PIN_RESET);

	    }
}

void ControlsystemInit(void){

	SetCellPWM(100);
	BLOWER(0);
	HAL_GPIO_WritePin(H_VALVE_GPIO_Port, H_VALVE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(HUMIDIFIER_GPIO_Port, HUMIDIFIER_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PURGE_GPIO_Port, PURGE_Pin, GPIO_PIN_RESET);

}
