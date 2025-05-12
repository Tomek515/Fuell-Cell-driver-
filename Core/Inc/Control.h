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

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim1;

#define CAPACITOR_LOW_LEVEL 15.0f
#define CAPACITOR_HIGH_LEVEL 25.0f
#define TEMP_MAX 55.0f
#define POWER_MIN 200.0f
#define POWER_MAX 250.0f

void BLOWER(uint8_t duty_percent);
void ControlSystem(void);
void SetCellPWM(uint8_t duty_percent);

#endif /* INC_CONTROL_H_ */
