/*
 * measurments.h
 *
 *  Created on: May 7, 2025
 *      Author: dvixe
 */

#ifndef INC_MEASURMENTS_H_
#define INC_MEASURMENTS_H_

#include "main.h"

extern float adc_vals[4];
//adc_vals[0] = temp
//adc_vals[1] = Vogniwo
//adc_vals[2] = Iogniwo
//adc_vals[3] = Vcapy
void Read_ADC_Channels(void);

#endif /* INC_MEASURMENTS_H_ */
