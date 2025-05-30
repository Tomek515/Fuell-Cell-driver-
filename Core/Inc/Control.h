/*
 * Control.h
 *
 *  Created on: May 9, 2025
 *      Author: dvixe
 */

#ifndef INC_CONTROL_H_
#define INC_CONTROL_H_


#include "measurments.h"
#include "Parameterscheck.h"
#include "stdbool.h"
#include "main.h"
#include "MAX31865.h"
extern Max31865_t  pt100;
extern bool        pt100isOK;
extern float       pt100Temp;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim1;
extern float t;
//extern TIM_HandleTypeDef htim1;

#define CAPACITOR_LOW_LEVEL 25.0f
#define CAPACITOR_HIGH_LEVEL 42.5f
#define MAX_SAFE_TEMP      60.0f
#define MAX_SAFE_POWER     1000.0f

void BLOWER(uint8_t duty_percent);
void Controlsystem(void);
void SetCellPWM(uint8_t duty_percent);
void ControlsystemInit(void);

#endif /* INC_CONTROL_H_ */
