/*
 * MyAdc.c
 *
 *  Created on: Jun 25, 2022
 *      Author: Lenovo
 */

#include "MyAdc.h"
#include "adc.h"

MyAdc_t MyAdc = { 0 };

void MyAdcInit(void) {
	MX_ADC1_Init();
	MyAdc.tick = HAL_GetTick();
	//HAL_ADC_Start_DMA(&hadc1, &MyAdc.adc_val, 1);

}
void MyAdcLoop(void) {
	if (HAL_GetTick() - MyAdc.tick > 1000) {
		MyAdc.tick = HAL_GetTick();

	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hdadc)
{
	MyAdc.temperature = __HAL_ADC_CALC_TEMPERATURE(TEMPSENSOR_CAL_VREFANALOG,MyAdc.adc_val,ADC_RESOLUTION_12B);
}

